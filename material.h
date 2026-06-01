#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

/*
1. produce a scattered ray 
2. if scattered, say how much the ray should be attenuated
*/
class material {
    public:
        virtual ~material() = default; // virtual destructor for proper cleanup of derived material objects

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const {
            return false; // default implementation does not scatter rays, returns false
        }
};

class lambertian: public material{
    private:
        color albedo; // albedo = latin for whiteness
    public:     
        lambertian(const color& albedo) : albedo(albedo) {} 

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            auto scatter_direction = rec.normal + random_unit_vector();

            // Catch degenerate scatter direction
            if (scatter_direction.near_zero())
                scatter_direction = rec.normal;

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }
};

class metal : public material{
    private:
        color albedo;
        double fuzz;

    public:
        metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            reflected = unit_vector(reflected) + (fuzz * random_unit_vector()); // add fuzziness to the reflected ray
                                    // by randomly perturbing it within a unit sphere scaled by the fuzz factor
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0); // only scatter if the reflected ray is in the same hemisphere as the normal
        }
};

class dielectric : public material {
    private:
    // Refractive intex in vacuum or air, or the ratio of the material's refractive index over
    // the refractive index of the enclosing media
        double refraction_index;

        static double reflectance(double cosine, double refraction_index) {
            // Use Schlick's approximation for reflectance
            auto r0 = (1 - refraction_index) / (1 + refraction_index);
            r0 = r0*r0;
            return r0 + (1-r0)*std::pow((1 - cosine),5);
        }

    public:
        dielectric(double refraction_index) : refraction_index(refraction_index) {}

        bool scatter (const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            attenuation = color(1.0, 1.0, 1.0); // no attenuation for dielectric materials (they are transparent)
            double ri = rec.front_face ? (1.0/refraction_index) : refraction_index; // if the ray is outside the material, use the inverse of the refractive index, otherwise use the refractive index
            
            vec3 unit_direction = unit_vector(r_in.direction());
            double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0); // cos(theta) is the angle between the incoming ray and the normal, clamped to a maximum of 1.0
            double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta); // sin(theta) can be derived from cos(theta) using the identity sin^2(theta) + cos

            bool cannot_refract = ri * sin_theta > 1.0; // if the ray cannot refract (total internal reflection), it must reflect
            vec3 direction;

            // if the ray cannot refract or if the reflectance is greater than a random value, reflect the ray
            if (cannot_refract || reflectance(cos_theta, ri) > random_double()) 
                direction = reflect(unit_direction, rec.normal);
            else
                direction = refract(unit_direction, rec.normal, ri);
            
            scattered = ray(rec.p, direction);
            return true; // always scatter for dielectric materials (they can both reflect and refract)
        }
};

#endif