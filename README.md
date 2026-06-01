# Ray Tracer

A ray tracer built in C++ following [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html).

## Features

- Diffuse (Lambertian) materials
- Metal materials with adjustable fuzziness
- Dielectric (glass) materials with refraction
- Positionable camera with depth of field

## Build & Run

```bash
g++ main.cpp -o raytracer
./raytracer > image.ppm
```

Then open `image.ppm` to view the output.

<img width="1190" height="667" alt="image" src="https://github.com/user-attachments/assets/c57d69b0-b40a-4f38-8a4d-ea50a18c2cbb" />

