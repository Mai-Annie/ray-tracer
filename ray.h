#ifndef RAY_H
#define RAY_H

#include "vec3.h"

/*
P(t) = A + t*B 
P is the point along the ray at distance t from the origin
A is the origin of the ray
B is the direction of the ray
*/
class ray {
public:
    ray() {}
    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    // Getters - returns immutable reference to the origin and direction of the ray
    const point3& origin() const { return orig; }
    const vec3& direction() const { return dir; }

    // Returns the point along the ray at distance t from the origin
    point3 at(double t) const {
        return orig + t*dir;
    }

private:
    point3 orig;
    vec3 dir;
};

#endif