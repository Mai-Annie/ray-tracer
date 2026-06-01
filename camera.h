#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

/*
Two jobs:
1. Construct and dispatch rays into the world
2. Use the results of these rays to construct the rendered image
*/
class camera{
    public: 
         // Image 
        double aspect_ratio = 1; // ratio of image width over height
        int image_width = 100; // rendered image width in pixels
        int samples_per_pixel = 1; // number of rays to sample per pixel for anti-aliasing
        int max_depth = 10; // maximum nb of ray bounces into a scene

        double vfov = 90.0; // vertical field of view in degrees, determines the vertical size of the viewport
        point3 lookfrom = point3(0,0,0); // camera position in world space
        point3 lookat = point3(0,0,-1); // point in world
        vec3 vup = vec3(0,1,0); // "view up" vector, used to determine the camera's orientation in world space

        double defocus_angle = 0; //variation angle of rays through each pixel
        double focus_dist = 10; // distance from the camera to the focal plane, used for depth of field effects


        void render(const hittable& world){
            initialize();

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for (int j = 0; j < image_height; j++) {
                // Progress indicator - tracks the progress of rendering the image
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

                for (int i = 0; i < image_width; i ++){
                    color pixel_color(0,0,0); // initialize the pixel color to black
                    for (int sample = 0; sample < samples_per_pixel; sample++){
                        ray r = get_ray(i, j); // get a ray for the current pixel and sample
                        pixel_color += ray_color(r, max_depth, world); // accumulate the color contribution from the ray
                    }
                    write_color(std::cout, pixel_samples_scale * pixel_color); // write the final color for the pixel to the output stream, scaling by the number of samples per pixel
                }
            }

            std::clog << "\rDone.                    \n ";
        }

    private:
        int image_height;           // rendered image height
        double pixel_samples_scale; // Color scale factor for a sum of pixel samples
        point3 center;              // camera center
        point3 pixel00_loc;         // location of pixel 0, 0
        vec3 pixel_delta_u;         // horizontal delta vector from pixel to pixel
        vec3 pixel_delta_v;         // vertical delta vector from pixel to pixel
        vec3 u,v,w;                 // camera coordinate system basis vectors
        vec3 defocus_disk_u;        // defocus disk horizontal radius
        vec3 defocus_disk_v;        // defocus disk vertical radius 
    
        void initialize(){
            //Calculate the image height, must be at least 1
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height; // if the calculated image height is less than 1, set it to 1

            pixel_samples_scale = 1.0 / samples_per_pixel; // scale factor for averaging the color contributions from multiple samples per pixel
            center = lookfrom;

            // Determine viewport dimensions
            auto theta = degrees_to_radians(vfov);
            auto h = std::tan(theta/2);
            auto viewport_height = 2 * h * focus_dist;
            auto viewport_width = viewport_height * (double(image_width) / image_height);

            //calculate the u,v,w unit basis vectors for the camera coordinate frame
            w = unit_vector(lookfrom - lookat); // w points from the lookat point to the camera center
            u = unit_vector(cross(vup, w)); // u is perpendicular to both v
            v = cross(w, u); // v is perpendicular to both w and u

            // Calculate the vectors across the horizontal and down the vertical viewport edges
            auto viewport_u = viewport_width * u;
            auto viewport_v = viewport_height * -v; // negative because 
                                                    // we want the y-axis to point up in the image, but down in the viewport

            // Calculate the horizontal and vertical delta vectors from pixel to pixel
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // Calculate the location of the upper left pixel
            auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v); // add half a pixel to get to the center of the pixel

            // Calculate the camera defocus disk basis vectors
            auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
            defocus_disk_u = defocus_radius * u;
            defocus_disk_v = defocus_radius * v;

        }

        ray get_ray(int i, int j) const {
            // construct a camera ray originating from the origin and directed at randomly sampled
            // point aournd the pixel location i, j.
            auto offset = sample_square();
            auto pixel_sample = pixel00_loc 
                                + ((i + offset.x()) * pixel_delta_u)
                                + ((j + offset.y()) * pixel_delta_v); // calculate the location of the randomly sampled point around the pixel location
            
            auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample(); 
            auto ray_direction = pixel_sample - ray_origin; // ray direction is from the camera center

            return ray(ray_origin, ray_direction);
        }

        vec3 sample_square() const{
            // returns the vector to a random point in the [-0.5, 0.5] x [-0.5, 0.5] unit square, used for anti-aliasing by randomly sampling around the pixel location
            return vec3(random_double() - 0.5, random_double() - 0.5,0); // sample a random point in the unit square centered at the origin
        }

        // generate random point on the camera defocus disk
        point3 defocus_disk_sample() const{
            auto p = random_in_unit_disk(); // sample a random point in the unit disk
            return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v); // scale the random point by the defocus disk basis vectors and translate to the camera center
        }

        // Returns color for a given scene ray
        color ray_color(const ray& r, int depth, const hittable& world) const{
            // if we've exceeded the ray bounce limit, no more light is gathered
            if (depth <= 0) return color(0, 0, 0);

            hit_record rec;

            if (world.hit(r, interval(0.001, infinity), rec)) {
                ray scattered;
                color attenuation;

                // if the ray is scattered, recursively trace the scattered ray and attenuate its color contribution
                if (rec.mat->scatter(r, rec, attenuation, scattered))
                    return attenuation * ray_color(scattered, depth-1, world);
                return color(0, 0, 0);
            }

            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5 * (unit_direction.y() + 1.0);
            return (1.0-a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
        }
};

#endif