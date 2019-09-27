
#include <iostream>
#include <string>
#include <sstream>
#include <variant>
#include <vector>

#include "rayson.hpp"

const int EXIT_CODE_SUCCESS = 0,
          EXIT_CODE_BAD_USAGE = -1,
          EXIT_CODE_RUNTIME_ERROR = 1;

void print_usage() noexcept {
  std::cout << "usage:" << std::endl
            << std::endl
            << "  rayson-info <JSON-PATH>    print description of rayson file <JSON-PATH>" << std::endl
            << "  rayson-info -h|--help      print this usage information" << std::endl
            << std::endl;
}

std::string vector3_to_string(const rayson::vector3& v) noexcept {
  std::stringstream ss;
  ss << "<x="  << v.x() << ", y=" << v.y() << ", z=" << v.z() << ">";
  return ss.str();
}

std::string color_to_string(const rayson::color& c) noexcept {
  std::stringstream ss;
  ss << "[r="  << c.r() << ", g=" << c.g() << ", b=" << c.b() << "]";
  return ss.str();
}

void print_scene(const std::string& path, const rayson::scene& scene) noexcept {

  const std::string tab = "    ";

  auto print_indented = [&](auto& message) {
    std::cout << tab << message << std::endl;
  };

  auto print_none = [&]() { print_indented("(none)"); };

  std::cout << "path: \"" << path << "\"" << std::endl
            << "camera:" << std::endl
            << tab << "eye = " << vector3_to_string(scene.camera().eye()) << std::endl
            << tab << "up = " << vector3_to_string(scene.camera().up()) << std::endl
            << tab << "view = " << vector3_to_string(scene.camera().view()) << std::endl
            << "viewport:" << std::endl
            << tab << "x_resolution = " << scene.viewport().x_resolution() << std::endl
            << tab << "y_resolution = " << scene.viewport().y_resolution() << std::endl
            << tab << "left = " << scene.viewport().left() << std::endl
            << tab << "top = " << scene.viewport().top() << std::endl
            << tab << "right = " << scene.viewport().right() << std::endl
            << tab << "bottom = " << scene.viewport().bottom() << std::endl
            << "projection:" << std::endl;
  if (auto ortho = std::get_if<rayson::ortho_projection>(&scene.projection())) {
    std::cout << tab << "orthographic" << std::endl;
  } else {
    auto& persp = std::get<rayson::persp_projection>(scene.projection());
    std::cout << tab << "perspective, focal_length = " << persp.focal_length() << std::endl;
  }
  std::cout << "shader:" << std::endl;
  if (auto flat = std::get_if<rayson::flat_shader>(&scene.shader())) {
    print_indented("flat");
  } else {
    auto& phong = std::get<rayson::phong_shader>(scene.shader());
    std::cout << tab << "phong ambient_coeff=" << phong.ambient_coeff()
              << ", diffuse_coeff=" << phong.diffuse_coeff()
              << ", specular_coeff=" << phong.specular_coeff()
              << ", ambient_color=" << color_to_string(phong.ambient_color())
              << std::endl;
  }
  std::cout << "background = " << color_to_string(scene.background()) << std::endl
            << "point_lights:" << std::endl;
  if (scene.point_lights().empty()) {
    print_none();
  } else {
    for (auto& p : scene.point_lights()) {
      std::cout << tab << "location=" << vector3_to_string(p.location())
                << ", color=" << color_to_string(p.color())
                << ", intensity=" << p.intensity()
                << std::endl;
    }
  }
  std::cout << "materials:" << std::endl;
  if (scene.materials().empty()) {
    print_none();
  } else {
    for (auto& m : scene.materials()) {
      std::cout << tab << "name=\"" << m.name() << "\""
                << ", shininess=" << m.shininess()
                << ", color=" << color_to_string(m.color())
                << std::endl;
    }
  }
  std::cout << "spheres:" << std::endl;
  if (scene.spheres().empty()) {
    print_none();
  } else {
    for (auto& s : scene.spheres()) {
      std::cout << tab << "material=\"" << s.material().name() << "\""
                << ", center=" << vector3_to_string(s.center())
                << ", radius=" << s.radius()
                << std::endl;
    }
  }
  std::cout << "triangles:" << std::endl;
  if (scene.triangles().empty()) {
    print_none();
  } else {
    for (auto& t : scene.triangles()) {
      std::cout << tab << "material=\"" << t.material().name() << "\""
                << ", a=" << vector3_to_string(t.a())
                << ", b=" << vector3_to_string(t.b())
                << ", c=" << vector3_to_string(t.c())
                << std::endl;
    }
  }
}

int main(int argc, const char** argv) {

  std::vector<std::string> arguments(argv + 1, argv + argc);

  if (arguments.size() != 1) {
    print_usage();
    return EXIT_CODE_BAD_USAGE;
  }

  const auto& argument = arguments[0];
  if ((argument == "-h") || (argument == "--help")) {
    print_usage();
    return EXIT_SUCCESS;
  }

  const auto& path = argument;
  try {

    auto scene = rayson::read_file(path);
    print_scene(path, scene);

  } catch (rayson::read_exception e) {
    std::cerr << "rayson-info: " << e.message() << std::endl;
    return EXIT_CODE_RUNTIME_ERROR;
  }

  return EXIT_CODE_SUCCESS;
}
