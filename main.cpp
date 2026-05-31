#include "rtweekend.h"

#include "hittable_list.h"
#include "sphere.h"
#include "hittable.h" 
#include "camera.h"

int main(){
    // World
    hittable_list world;

    world.add(make_shared<sphere>(point3(0,0,-1), 0.5)); // add a sphere centered at (0,0,-1) with radius 0.5 to the world
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100)); // add a large sphere centered at (0,-100.5,-1) with radius 100 to the world to serve as the ground plane
    
    camera cam;

    cam.aspect_ratio = 16.0 / 9.0; // set the aspect ratio of the camera to 16:9
    cam.image_width = 400; // set the rendered image width to 400 pixels
    cam.samples_per_pixel = 100; // set the number of samples per pixel for anti-aliasing to 100

    cam.render(world); // render the world using the camera
}
