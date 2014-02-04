#include "Tracer.h"
#include "atlimage.h"

#include <algorithm>

Ray Tracer::makeRay(uvec2 pixel, uvec2 subpixel)
{
    vec3 right_half = settings.cam_right * tan(settings.view_angle.x / 2);
    vec3 up_half    = settings.cam_up    * tan(settings.view_angle.y / 2);

    float x_norm = (pixel.x + (subpixel.x + 0.5f) / settings.subpixels) / settings.resolution.x * 2.0f - 1.0f;
    float y_norm = (pixel.y + (subpixel.y + 0.5f) / settings.subpixels) / settings.resolution.y * 2.0f - 1.0f;

    vec3 dir = settings.cam_dir + right_half * x_norm + up_half * y_norm;
    return Ray(settings.cam_pos, normalize(dir));
}

vec3 Tracer::traceRay(Ray ray)
{
    return traceLimited(ray, settings.max_depth);
}

float FresnelReflectance(float ci, float n)
{
    float ci2 = ci * ci;
    float si2 = 1.0f - ci2;
    float si4 = si2 * si2;
    float a = ci2 + n * n - 1.0f;
    float sqa = 2 * sqrtf(a) * ci;
    float b = ci2 + a;
    float c = ci2 * a + si4;
    float d = sqa * si2;
    return (b - sqa) / (b + sqa) * (1.0f + (c - d) / (c + d)) * 0.5f;
}

vec3 Tracer::traceLimited(Ray ray, int lives)
{
    if(lives < 0) 
        return vec3(1, 1, 1);

    vec3 color;
    bool intersected = false;
    float min_dist = 0;
    Intersection best;
    int best_obj;

    for(int i = 0; i < scene->objects.size(); i++) {
        Intersection data;
        if(scene->objects[i]->intersect(ray, data)) {
            float dist = length(ray.origin - data.pos);
            if(!intersected || dist < min_dist) {
                intersected = true;
                min_dist = dist;
                best = data;
                best_obj =i;
            }
        }
    }

    if(!intersected) {
        return vec3(1, 1, 0);
    }
    
    Material mat = scene->objects[best_obj]->material;

    if(mat.surface == surface_light) {

        return best.color;

    }else if(mat.surface == surface_diffuse) {

        return lightingPhong(best, mat);

    } else if(mat.surface == surface_mirror) {
            
        Ray reflected(best.pos, normalize(reflect(ray.dir, best.normal)));
        return traceLimited(reflected, lives - 1);

    } else if(mat.surface == surface_glass) {

        Ray reflected(best.pos, reflect(ray.dir, best.normal));
        color = traceLimited(reflected, lives - 1);

        float n = 1.5;
        float eta = 1.0f/n;
        float cos_theta = -dot(ray.dir, best.normal);
        if(cos_theta < 0) {
             cos_theta *= -1.0f;
             best.normal *= -1.0f;
             eta = 1.0f/eta;
        }
        float reflected_energy = FresnelReflectance(cos_theta, n);
        float passed_energy = 1 - reflected_energy;
        
        float k = 1.0f - eta*eta*(1.0-cos_theta*cos_theta);
        if(k >= 0.0f) {
            Ray passed(best.pos, normalize(eta * ray.dir + (eta * cos_theta - sqrt(k)) * best.normal));
            color = traceLimited(reflected, lives - 1) * reflected_energy + traceLimited(passed, lives - 1) * passed_energy;
        } else {
            color = traceLimited(reflected, lives - 1);
        }
        return color;
    }
}

void Tracer::renderImage()
{
    image.resize(settings.resolution.x * settings.resolution.y);
    int total = settings.resolution.x * settings.resolution.y;

    #pragma omp parallel for schedule(dynamic)
    
    for(int px = 0; px < total; px++) {
        int y = px / settings.resolution.x;
        int x = px % settings.resolution.x;
        image[px] = traceRay(makeRay(uvec2(x, y)));
    }
}

void Tracer::saveImage(string file_name)
{   
    CImage save_image;

    int width = settings.resolution.x;
    int height = settings.resolution.y;

    save_image.Create(width, height, 24);
    
    int pitch = save_image.GetPitch();    
    unsigned char* imageBuffer = (unsigned char*)save_image.GetBits();

    if (pitch < 0) {

        imageBuffer += pitch * (height - 1);
        pitch =- pitch;
    }

    int i, j;
    int imageDisplacement = 0;
    int textureDisplacement = 0;

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {

            vec3 color = image[textureDisplacement + j];

            imageBuffer[imageDisplacement + j * 3] = clamp(color.b, 0.0f, 1.0f) * 255.0f;
            imageBuffer[imageDisplacement + j * 3 + 1] = clamp(color.g, 0.0f, 1.0f) * 255.0f;
            imageBuffer[imageDisplacement + j * 3 + 2] = clamp(color.r, 0.0f, 1.0f) * 255.0f;
        }
        imageDisplacement += pitch;
        textureDisplacement += width;
    }

    save_image.Save(file_name.c_str());
    save_image.Destroy();
}

void Tracer::setSettings(Settings s)
{
    settings = s;
}

bool Tracer::loadSettings(string file_name)
{
    XMLSerializerReader reader(file_name);
    reader.open();
    if(!settings.serialize(reader)) {
        reader.close();
        return false;
    }
    reader.close();

    settings.cam_right.x = settings.cam_dir.y;
    settings.cam_right.y =  -settings.cam_dir.x;
    settings.cam_right.z = 0;
    settings.cam_up = cross(settings.cam_right, settings.cam_dir);

    settings.cam_dir = normalize(settings.cam_dir);
    settings.cam_right = normalize(settings.cam_right);
    settings.cam_up = normalize(settings.cam_up);

    settings.view_angle *= 3.1415926f / 180.0f;

    if(settings.resolution.y / settings.resolution.x != tan(settings.view_angle.y) / tan(settings.view_angle.x)) 
        cout << "Warning! Wrong aspect ration in view angle parameter." << endl;

    return true;
}

bool Tracer::saveSettings(string file_name)
{
    XMLSerializerWriter writer(file_name);
    writer.open();
    settings.serialize(writer);
    writer.close();

    return true;
}

vec3 Tracer::lightingPhong(Intersection data, Material mat)
{
    vec3 color(0, 0, 0);
    for(int i = 0; i < scene->lights.size(); i++) {
        vec3 L = normalize(scene->lights[i]->pos - data.pos);   
        vec3 E = normalize(settings.cam_pos - data.pos);
        vec3 R = normalize(-reflect(L, data.normal));

        vec3 Iamb = scene->lights[i]->ambient;
        vec3 Idiff = scene->lights[i]->diffuse * max(dot(data.normal, L), 0.0f);
        Idiff = clamp(Idiff, 0.0, 1.0);
        vec3 Ispec = scene->lights[i]->specular * powf(max(dot(R,E),0.0), 0.3 * mat.shininess);
        Ispec = clamp(Ispec, 0.0, 1.0); 
        color += Iamb + Idiff + Ispec;
    }
    color /= scene->lights.size();
    color += data.color;
    return color;
}