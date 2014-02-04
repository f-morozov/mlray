#pragma once

#include "glm/glm.hpp"
#include "Types.h"
#include "Scene.h"

#include <string>

using namespace std;

class Tracer
{
public:
    Tracer() 
    {
        scene = new Scene();
    }

    ~Tracer() 
    {
        delete scene;
    }

    void renderImage();
    void saveImage(string file_name);
    void setSettings(Settings s);
    bool loadSettings(string file_name);
    bool saveSettings(string file_name);
protected:
    Settings settings;
    Scene *scene;
    vector<vec3> image;

    vec3 traceRay(Ray ray);
    Ray makeRay(uvec2 pixel, uvec2 subpixel = uvec2(0, 0));
    vec3 traceLimited(Ray ray, int lives);
    vec3 lightingPhong(Intersection data, Material mat);
};