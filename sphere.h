#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable {
    private:
        point3 center;
        double radius;
        shared_ptr<material> mat; // material of the sphere, used for shading calculations

    public:
        sphere(const point3& center, double radius, shared_ptr<material> material) : center(center), radius(std::fmax(0,radius)), mat(material) {}

        // Override the hit function from the hittable class to determine if a ray hits the sphere
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            vec3 oc = center - r.origin();
            auto a = r.direction().length_squared();
            auto h = dot(r.direction(), oc);
            auto c = oc.length_squared() - radius*radius;

            auto discriminant = h*h - a*c;
            if (discriminant < 0) {
                return false; // no hit
            }

            auto sqrtd = std::sqrt(discriminant);

            // Find the nearest root that lies in the acceptable range
            auto root = (h - sqrtd) / a;
            if (!ray_t.surrounds(root)) {
                root = (h + sqrtd) / a;
                if (!ray_t.surrounds(root)) {
                    return false; // no valid roots within range
                }
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center) / radius; // normal vector at the hit point on the sphere
            rec.set_face_normal(r, outward_normal); // set the hit record normal vector to point against the ray direction
            rec.mat = mat;

            return true;
        }
};

#endif