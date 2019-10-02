# rayson: raytrace scene object notation

*rayson* is a minimalist
[JSON](http://json.org/)-based
format for simple raytrace scenes, and a C++ parser for that format.

The motivating use case for rayson is as an input format for student raytracing
projects in the
[CPSC 484 Principles of Computer Graphics](https://csufcs.com/cpsc484)
course at
[CSU Fullerton](https://www.fullerton.edu).
Therefore the scope of the format is narrow, covering only features that can
be implemented as part of a class project, yet wide enough to encode
non-trivial scenes.

Our course follows the
[Fundamentals of Computer Graphics](https://www.crcpress.com/Fundamentals-of-Computer-Graphics/Marschner-Shirley/p/book/9781482229394)
textbook by Marschner and Shirley, so our JSON schema corresponds to the
software design and feature set described in chapter 4 of that book.

The format and parser are intended to be extensible, and indeed students
have extended it to render more featureful scenes.

The parser is a single-header library in `rayson.hpp`.

## Dependencies

- C++17 or newer
- [nlohmann::json library](https://github.com/nlohmann/json)
- the unit tests require [googletest](https://github.com/google/googletest)
- tested with `clang++`, but other C++17-compliant compilers should work

## The Format

### Example

The following is the contents of `scene_2spheres_persp_phong.json`.

`
{
  "camera_eye" : [0, 0, 0],
  "camera_up" : [0, -1, 0],
  "camera_view" : [0, 0, 1],
  "x_resolution" : 400,
  "y_resolution" : 400,
  "viewport_left" : -1.0,
  "viewport_top" : 1.0,
  "viewport_right" : 1.0,
  "viewport_bottom" : -1.0,
  "background" : [0.7, 0.7, 0.9],
  "persp_focal_length" : 1.0,
  "phong_shader" : {
    "ambient_coeff" : 0.05,
    "diffuse_coeff" : 0.5,
    "specular_coeff" : 0.25,
    "ambient_color" : [1.0, 1.0, 1.0]
  },
  "materials" : [
    { "name" : "red", "color" : [1.0, 0.0, 0.0], "shininess" : 4.0 },
    { "name" : "blue", "color" : [0.0, 0.0, 1.0], "shininess" : 4.0 }
  ],
  "point_lights" : [
    { "location" : [-2.0, 2.0, -1.0], "intensity" : 1.1, "color" : [1.0, 1.0, 1.0] },
    { "location" : [1.0, 0.0, -1.0], "intensity" : 0.25, "color" : [1.0, 1.0, 1.0] }
  ],
  "spheres" : [
    { "material" : "red", "center" : [-1.0, 0.0, 2.0], "radius" : 0.5},
    { "material" : "blue", "center" : [1.0, 0.0, 8.0], "radius" : 0.5}
  ]
}
`

## Schema

- A rayson scene is one JSON object with several mandatory key-value entries.
- A `vector3` must be an array containing exactly three numbers, corresponding
  to x, y, z respectively.
- A `color` must be an array containing exactly three numbers in the range
  [0, 1] corresponding to red, green, blue respectively, where `[0, 0, 0]`
  is black and `[1, 1, 1]` is white.
- Every scene object must contain:
  - `"camera_eye"`, `"camera_up"`, `"camera_view"` are each a `vector3` and together
    they define a camera basis. A raytracer will need `"camera_up"` and `"camera_view"` to be nonzero and nonequal, but the parser does not enforce this.
  - `"x_resolution"` and `"y_resolution"` are positive integers that define the
    resolution of the output image.
  - `"viewport_left"`, `"viewport_top"`, `"viewport_right"`, and `"viewport_bottom"`
    are numbers defining the extent of the viewport in view volume space.
    `"viewport_left"` and `"viewport_bottom"` must be negative, and
    `"viewport_right"` and `"viewport_top"` must be positive.
  - `"background"` is the color of pixels that do not intersect a scene object.
- A scene must contain exactly one kind of projection:
  - `"ortho_projection"` must be a true Boolean to use orthographic projection.
  - `"persp_focal_length"` must be a positive number to use perspective
    projection with the given focal length.
- A scene must contain exactly one kind of shader:
  - `"flat_shader"` must be a true Boolean to use flat shading.
  - `"phong_shader"` must be an object, with following entries, to use Blinn-Phong shading:
    - `"ambient_coeff"` is the ambient light coefficient and must be a non-negative number.
    - `"diffuse_coeff"` is the diffuse light coefficient and must be a non-negative number.
    - `"specular_coeff"` is the specular light coefficient and must be a non-negative number.
    - `"ambient_color"` is the `color` of ambient light.
- A scene must have `"materials"`,
  an array where each element is an object with the following entries:
  - `"name"` is a unique string identifying this material.
  - `"color"` is a color defining the diffuse color of the material.
  - `"shininess"` is the Phong exponent of the material.
- A scene *may* have `"point_lights"`,
  an array where each element is an object with the following entries:
  - `"location"` is a `vector3` for the center of the light.
  - `"intensity"` is a number for the intensity multiplier of the light.
  - `"color"` is a `color` for the color of the light.
- A scene *may* have `"spheres"`,
  an array where each element is an object with the following entries:
  - `"material"` is a string which is the name of the material to use when
    shading the sphere.
  - `"center"` is a `vector3` of the center of the sphere.
  - `"radius"` is a positive number, the radius of the sphere.
- A scene *may* have `"triangles"`,
  an array where each element is an object with the following entries:
  - `"material"` is a string which is the name of the material to use when
    shading the triangle.
  - `"a"`, `"b"`, and `"c"` is each a `vector3` defining one of the three
    vertices of the triangle. These vectors must all be distinct so the
    triangle is non-degenerate.
