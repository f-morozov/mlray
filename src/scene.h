#pragma once

#include "objects.h"

struct Scene
{
    Scene();
    ~Scene();

    vector<Object*> objects;
    vector<Light*> lights;
};