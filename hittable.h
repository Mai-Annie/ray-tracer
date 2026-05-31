#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class hit_record{
    public:
        point3 p;
        vec3 normal;
        double t;
};

class hittable{
    public:
        // virtual destructor to ensure proper cleanup of derived classes
        virtual ~hittable() = default;

        // Pure virtual function to determine if a ray hits the object
        virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};

#endif