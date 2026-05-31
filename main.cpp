#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>

// Returns color for a given scene ray 
color ray_color(const ray& r){
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0); // scale y from [-1,1] to [0,1]
    return (1.0-a) * color(1.0, 1.0, 1.0) + a * color (0.5, 0.7, 1.0); // linear blend of white and light blue based on the y value of the ray direction
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