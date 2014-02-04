    //int light_size = 20;
    int half_size = 10;
    objects.push_back(new Rect(Material(surface_light, 0.5), vec3(1, 1, 1), vec3(500 - half_size, 500 - half_size, 999), vec3(500 + half_size, 500 + half_size, 999), vec3(0, 0, -1))); // light
    
    lights.push_back(new Light(vec3(500, 500, 1000), vec3(0.1, 0.1, 0.1), vec3(0.5, 0.5, 0.5), vec3(0.01, 0.01, 0.01)));

    //int light_num = 3;
    //int dist = light_size / light_num;
    //for(int i = 0; i < light_num; i++) {
    //    for(int j = 0; j < light_num; j++) {
    //        Light *light = new Light(vec3(500 - half_size + i * dist, 500 - half_size + j * dist, 1000), 
    //                                 vec3(0.1, 0.1, 0.1), vec3(0.5, 0.5, 0.5), vec3(0.01, 0.01, 0.01));

    //        lights.push_back(light);
    //    }
    //}