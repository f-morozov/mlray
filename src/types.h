#pragma once

#include "glm/glm.hpp"
#include "serialize.h"
#include <vector>

using namespace glm;

struct Ray
{
    Ray(vec3 _origin, vec3 _dir) : origin(_origin), dir(_dir) {}

    vec3 origin;
    vec3 dir;
};

struct Settings
{
    vec3 cam_pos;
    vec3 cam_dir;
    vec3 cam_right;
    vec3 cam_up;

    vec2 view_angle;
    uvec2 resolution;

    int max_depth;
    int subpixels;

    bool serialize(ISerializer& inout)
    {
        bool res = true;
        res &= inout.InOut("CameraPosition", cam_pos);
        res &= inout.InOut("ViewDirection", cam_dir);
        res &= inout.InOut("ViewAngle", view_angle);
        res &= inout.InOut("Resolution", resolution);
        res &= inout.InOut("TraceDepth", max_depth);
        res &= inout.InOut("Subpixels", subpixels);
        return res;
    }
};

struct Intersection
{
    vec3 pos;
    vec3 normal;
    vec3 color;
};

struct Light
{
    Light(vec3 _pos, vec3 _ambient, vec3 _diffuse, vec3 _specular):
        pos(_pos),
        ambient(_ambient),
        diffuse(_diffuse),
        specular(_specular)
    {
    }

    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;  
};

enum Surfaces
{
    surface_glass,
    surface_mirror,
    surface_diffuse,
    surface_light
};

struct Material
{
    Material(int _surface, float _shininess) :
        surface(_surface),
        shininess(_shininess)
    {
    }

    int surface;
    float shininess;
};