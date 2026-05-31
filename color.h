#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

using color = vec3; // RGB color

// Write the translated [0,255] value of each color component in the output stream
void write_color(std::ostream& out, const color& pixel_color){
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Translate the color values from [0,1] to [0,255] (bytes)
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    // Write out the pixel color components
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}


#endif