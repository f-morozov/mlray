#include "scene.h"

Scene::Scene() 
{
    objects.push_back(new Rect(Material(surface_diffuse, 0.5), vec3(0.5, 0, 0), vec3(0, 0, 0), vec3(0, 1000, 1000), vec3(1, 0, 0))); // left
    objects.push_back(new Rect(Material(surface_diffuse, 0.5), vec3(0, 0.5, 0), vec3(1000, 0, 0), vec3(1000, 1000, 1000), vec3(-1, 0, 0))); // right
    objects.push_back(new Rect(Material(surface_diffuse, 0.5), vec3(0, 0, 0.5), vec3(0, 1000, 0), vec3(1000, 1000, 1000), vec3(0, -1, 0))); // back

    objects.push_back(new Rect(Material(surface_diffuse, 0.5), vec3(0.5, 0.5, 0.5), vec3(0, 0, 1000), vec3(1000, 1000, 1000), vec3(0, 0, -1))); // up
    objects.push_back(new Rect(Material(surface_diffuse, 0.5), vec3(0, 0, 0), vec3(0, 0, 0), vec3(1000, 1000, 0), vec3(0, 0, 1))); // down

    objects.push_back(new Rect(Material(surface_mirror, 0.5), vec3(0, 0, 0), vec3(1, 250, 250), vec3(1, 750, 750), vec3(1, 0, 0))); // mirror
    objects.push_back(new Rect(Material(surface_mirror, 0.5), vec3(0, 0, 0), vec3(250, 250, 1), vec3(750, 750, 1), vec3(0, 0, 1))); // down

    objects.push_back(new Sphere(Material(surface_diffuse, 0.5), vec3(0.5, 0, 0.5), vec3(150, 500, 500), 100));

    objects.push_back(new Sphere(Material(surface_light, 0.5), vec3(1, 1, 1), vec3(500, 500, 1000 + 190), 200));
    lights.push_back(new Light(vec3(500, 500, 1000), vec3(0.1, 0.1, 0.1), vec3(0.5, 0.5, 0.5), vec3(0.03, 0.03, 0.03)));

    objects.push_back(new Model(Material(surface_glass, 0.5), "data/crystal.3ds", 15, vec3(300, 300, 5), vec3(1, 1, 1)));
}

Scene::~Scene()
{ 
    for(int i = 0; i < objects.size(); i++) {
        delete objects[i];
    }
    for(int i = 0; i < lights.size(); i++) {
        delete lights[i];
    }
}
