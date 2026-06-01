#ifndef VEC3_H
#define VEC3_H

#include "rtweekend.h"

/*
* A 3D vector class
* Will be used for colors, locations, directions, offsets, etc. in the ray tracer
* This is BAD practice because we would do something stupid like subtracting position from colour
* But we wanna keep it simple
*/
class vec3 {
    public: 
    double e[3];

    // Constructor overloading
    vec3() : e{0,0,0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    // Getters
    // note: const after the function signature means that this function does not modify the class instance
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    // Operator overloading for vector math
    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(const double t) {
        return *this *= 1/t;
    }

    double length() const {
        return std::sqrt(length_squared());
    }

    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    // Return true if the vector is close to zero in all dimensions
    bool near_zero() const {
        const auto s = 1e-8;
        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    }

    static vec3 random(){
        return vec3(random_double(), random_double(), random_double());
    }

    // Returns a random vector with each component in the range [min, max)
    static vec3 random(double min, double max){
        return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
    }
};

// point 3 is just a nickname for vec3, but useful for geometric clarity in the code
using point3 = vec3;   // 3D point

// Vector Utility Functions

inline std::ostream& operator <<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator +(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator -(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator *(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator *(double t, const vec3 &v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator *(const vec3 &v, double t) {
    return t * v;
}

inline vec3 operator /(vec3 v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0]
            + u.e[1] * v.e[1]
            + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

inline vec3 random_unit_vector(){
    while (true){
        auto p = vec3:: random(-1,1);
        auto lensq = p.length_squared();
        if (1e-160 < lensq && lensq <=1) // reject points that are too close to zero 
                        // length to avoid numerical issues, and reject points outside the unit sphere
        return p /sqrt(lensq);
    }
}

// calculates dot product of surface normal and random unit vector to determine
// if the random unit vector is in the same hemisphere as the normal
// returns a random unit vector in the same hemisphere as the normal
inline vec3 random_on_hemisphere(const vec3& normal){
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0){ // if the random point on the unit sphere is in the same hemisphere as the normal
        return on_unit_sphere;
        } else {
        return -on_unit_sphere; // otherwise, return the opposite point on the unit sphere to ensure it is in the same hemisphere as the normal
    }
}

// reflect the vector v around the normal n using the formula r = v - 2*dot(v,n)*n
inline vec3 reflect(const vec3& v, const vec3& n){
    return v - 2*dot(v,n)*n; 
}

#endif
