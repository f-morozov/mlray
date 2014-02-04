#pragma once

#include "types.h"

class Object
{
public:
    virtual bool intersect(Ray ray, Intersection& data) = 0;
    Material material;

    Object(Material mat) : material(mat) {}
};

class Rect : public Object
{
public:
    bool intersect(Ray ray, Intersection& data);
    Rect(Material _mat, vec3 _color, vec3 _vmin, vec3 _vmax, vec3 _normal) :
        Object(_mat),
        color(_color),
        vmin(_vmin),
        vmax(_vmax),
        normal(_normal)
    {
    }

protected:
    vec3 color;
    vec3 vmin, vmax;
    vec3 normal;
};

class Sphere : public Object
{
public:
    bool intersect(Ray ray, Intersection& data);
    Sphere(Material _mat, vec3 _color, vec3 _center, float _radius) :
        Object(_mat),
        color(_color),
        center(_center),
        radius(_radius)
    {
    }
protected:
    vec3 color;
    vec3 center;
    float radius;
};

class Model : public Object
{
public:
    bool intersect(Ray ray, Intersection& data);
    
    Model(Material _mat, string _model_file, float scale, vec3 shift, vec3 _color = vec3(1, 1, 1)) :
        Object(_mat),
        model_file(_model_file),
        color(_color)
    {
        generate(scale, shift);
    }

protected:
    vector<vec3> vertices;
    vector<vec3> normals;
    vector<uvec3> faces;
    string model_file;
    vec3 color;
    void generate(float scale, vec3 shift);
};
