#include <algorithm>
#include "objects.h"
#include "l3ds/l3ds.h"

bool Rect::intersect(Ray ray, Intersection& data)
{
    vec3 inv = 1.0f / ray.dir;

    float low = inv.x * (vmin.x - ray.origin.x);
    float hi = inv.x * (vmax.x - ray.origin.x);
    float tmin  = std::min(low, hi);
    float tmax = std::max(low, hi);
    
    low = inv.y * (vmin.y - ray.origin.y);
    hi = inv.y * (vmax.y - ray.origin.y);
    tmin  = std::max(tmin, std::min(low, hi));
    tmax = std::min(tmax, std::max(low, hi));

    low = inv.z * (vmin.z - ray.origin.z);
    hi = inv.z * (vmax.z - ray.origin.z);
    tmin  = std::max(tmin, std::min(low, hi));
    tmax = std::min(tmax, std::max(low, hi));
    
    if(tmin > tmax || tmax <= 1)
        return false;

    float t = (tmin > 1) ? tmin : tmax;    
    data.pos = ray.origin + t * ray.dir;
    data.color = color;
    data.normal = normal;
    return true;
}

bool Sphere::intersect(Ray ray, Intersection& data)
{
    vec3 dir = ray.origin - center;

    float b = dot(dir, ray.dir);
    float c = dot(dir, dir) - radius * radius;
    float D = b * b - c; 

    if(D < 0)
        return false;

    float sqrtfd = sqrtf(D);
    float t1 = -b + sqrtfd;
    float t2 = -b - sqrtfd;
    float min_t  = std::min(t1, t2);
    float max_t = std::max(t1,t2);
    float t = (min_t > 1) ? min_t : max_t;
    if(t <= 1)
        return false;
      
    data.pos = ray.origin + t * ray.dir;
    data.color = color;
    data.normal = normalize(data.pos - center);
}

bool Model::intersect(Ray ray, Intersection& data)
{
    data.color = color;

    bool intersected = false;
    float best_t = 0;

    for(int i = 0; i < faces.size(); i++) {

        vec3 edge1 = vertices[faces[i].y] - vertices[faces[i].x];
        vec3 edge2 = vertices[faces[i].z] - vertices[faces[i].x];

        vec3 T = ray.origin - vertices[faces[i].x];
        vec3 P = cross(ray.dir, edge2);
        vec3 Q = cross(T, edge1);

        float det = dot(edge1, P);
        if(fabs(det) < 1e-5) 
            continue;
        det = 1 / det;

        float u = dot(P, T) * det;
        if(u < 0 || u > 1)
            continue;

        float v = dot(Q, ray.dir) * det;
        if(v < 0 || u + v > 1)
            continue;

        float t = dot(Q, edge2) * det;
        if(t <= 1)
            continue;

        if(!intersected || t < best_t) {
            best_t = t;
            intersected = true;
            data.pos = ray.origin + t * ray.dir;
            data.normal = v * normals[faces[i].x] + (1 - u - v) * normals[faces[i].y] + u * normals[faces[i].z];
        }
    }
    return intersected;
}

void Model::generate(float scale, vec3 shift)
{
    l3ds::L3DS model;
	model.LoadFile("Data/crystal.3ds");
    model.SetOptimizationLevel(l3ds::LOptimizationLevel::oFull);

    for(int i = 0; i < model.GetMesh(0).GetTriangleCount(); i++) {
        l3ds::LTriangle face =  model.GetMesh(0).GetTriangle(i);
        faces.push_back(uvec3(face.a, face.b, face.c));
    }

    vec3 min(1e10, 1e10, 1e10);
    vec3 max(-1e10, -1e10, 1e10);


    for(int i = 0; i < model.GetMesh(0).GetVertexCount(); i++) {
        l3ds::LVector4 vertex =  model.GetMesh(0).GetVertex(i);
        l3ds::LVector3 normal =  model.GetMesh(0).GetNormal(i);

        vertices.push_back(vec3(-vertex.z, -vertex.x, vertex.y));
        normals.push_back(normalize(vec3(normal.x, normal.y, normal.z)));

        if(vertices[i].x < min.x) 
            min.x = vertices[i].x;
        if(vertices[i].y < min.y) 
            min.y = vertices[i].y;
        if(vertices[i].z < min.z) 
            min.z = vertices[i].z;
    }
    for(int i = 0; i < model.GetMesh(0).GetVertexCount(); i++) {
        vertices[i] = (vertices[i] - min) * scale + shift;
    }
}
