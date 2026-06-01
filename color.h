#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"

using color = vec3; // RGB color

// Apply gamma correction to convert a linear color component to a gamma-corrected value for display
inline double linear_to_gamma(double linear_component){
    if (linear_component > 0)
        return std::sqrt(linear_component);

    return 0;
}

// Write the translated [0,255] value of each color component in the output stream
void write_color(std::ostream& out, const color& pixel_color){
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // apply linear to gamma transform for gamma 2
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Translate the color values from [0,1] to [0,255] (bytes)
    static const interval intensity (0.000, 0.999); // clamp the color values to the range [0, 0.999] to avoid overflow when converting to bytes
    int rbyte = int(255.999 * intensity.clamp(r));
    int gbyte = int(255.999 * intensity.clamp(g));
    int bbyte = int(255.999 * intensity.clamp(b));

    // Write out the pixel color components
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}


#endif