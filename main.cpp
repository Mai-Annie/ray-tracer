#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>

// Returns true if the ray hits a sphere with the given center and radius, false otherwise
bool hit_sphere(const point3& center, double radius, const ray& r){
    vec3 oc = center - r.origin();
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = h*h - a*c;
    
    if (discriminant < 0) {
        return -1.0; // no hit
    } else {
        return (h - std::sqrt(discriminant)) / (2.0*a); // hit, return the nearest root
    }
}

// Returns color for a given scene ray 
color ray_color(const ray& r){
    auto t = hit_sphere(point3(0,0,-1), 0.5, r);
    if (t > 0.0) {
        vec3 N = unit_vector(r.at(t) - vec3(0,0,-1)); // normal vector at the hit point on the sphere
        return 0.5 * color(N.x() + 1, N.y() + 1, N.x() + 1); // scale the normal vector from [-1,1] to [0,1] and return as color

    }
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0); // scale y from [-1,1] to [0,1]
    return (1.0-a) * color(1.0, 1.0, 1.0) + a * color (1.0, 0.7, 0.8); // linear blend of white and light pink based on the y value of the ray direction
}

int main(){
    // Image 
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    //Calculate the image height, must be at least 1
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height; // if the calculated image height is less than 1, set it to 1

    // Camera
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    auto camera_center = point3(0,0,0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0); // negative because 
                                            // we want the y-axis to point up in the image, but down in the viewport

    // Calculate the horizontal and vertical delta vectors from pixel to pixel
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel
    auto viewport_upper_left = camera_center - vec3(0,0,focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v); // add half a pixel to get to the center of the pixel   



    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        // Progress indicator - tracks the progress of rendering the image
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

        for (int i = 0; i < image_width; i ++){
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v); // calculate the location of the center of the current pixel
            auto ray_direction = pixel_center - camera_center; // calculate the ray direction from the camera center to the pixel center
            ray r(camera_center, ray_direction); // construct the ray from the camera center in the
            
            color pixel_color = ray_color(r); // get the color for the ray
            write_color(std::cout, pixel_color);
        }

        std::clog << "\rDone.                    \n ";    
    }
}