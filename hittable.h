#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class hit_record{
    public:
        point3 p;
        vec3 normal;
        double t;
        bool front_face;

        // Sets the hit record normal vector to point against the ray direction, regardless of whether the ray is inside or outside the surface
        void set_face_normal(const ray& r, const vec3& outward_normal){
            // Sets the hit record normal vector, assume outward_normal is a unit vector
            // NOTE: the parameter 'outward_normal' is assumed to have unit length
            front_face = dot(r.direction(), outward_normal) < 0; // if the ray is outside the surface, the normal should point against the ray direction
            normal = front_face ? outward_normal : -outward_normal; // if the ray is inside the surface, flip the normal to point against the ray direction
        }
};

class hittable{
    public:
        // virtual destructor to ensure proper cleanup of derived classes
        virtual ~hittable() = default;

        // Pure virtual function to determine if a ray hits the object
        virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};

#endif