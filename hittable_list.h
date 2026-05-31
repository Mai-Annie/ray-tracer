#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <vector>
#include <memory>
 

using std::make_shared;
using std::shared_ptr;

// hittable_list: Collection of hittable objects. It is itself a hittable object, so it can be used to represent a scene with multiple objects.
class hittable_list : public hittable {
    public:
        // List of shared pointers to hittable objects in the scene
        std::vector<shared_ptr<hittable>> objects;

        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) { 
            objects.push_back(object); 
        }

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override{
            hit_record temp_rec;
            bool hit_anything = false;
            auto closest_so_far = t_max;

            // Check for hits against all objects in the list and keep track of the closest hit
            for (const auto& object : objects) {
                if (object->hit(r, t_min, closest_so_far, temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }

            return hit_anything;
        }
};