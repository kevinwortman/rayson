
///////////////////////////////////////////////////////////////////////////////
// rayson_test.cpp
//
// Unit tests for code declared in rayson.hpp.
//
///////////////////////////////////////////////////////////////////////////////

#include "gtest/gtest.h"

#include "rayson.hpp"

TEST(vector3, ConstructorSettersAndGetters) {

  rayson::vector3 zero, one_two_three(1.0, 2.0, 3.0);

  EXPECT_DOUBLE_EQ(0.0, zero.x());
  EXPECT_DOUBLE_EQ(0.0, zero.y());
  EXPECT_DOUBLE_EQ(0.0, zero.z());

  EXPECT_DOUBLE_EQ(1.0, one_two_three.x());
  EXPECT_DOUBLE_EQ(2.0, one_two_three.y());
  EXPECT_DOUBLE_EQ(3.0, one_two_three.z());
}

TEST(vector3, is_normalized) {

  // clearly not normalized
  EXPECT_FALSE(rayson::vector3(10, -5, 8).is_normalized());
  EXPECT_FALSE(rayson::vector3(100, 0, 0).is_normalized());

  // positive orthogonal
  EXPECT_TRUE(rayson::vector3(1.0, 0.0, 0.0).is_normalized());
  EXPECT_TRUE(rayson::vector3(0.0, 1.0, 0.0).is_normalized());
  EXPECT_TRUE(rayson::vector3(0.0, 0.0, 1.0).is_normalized());

  // negative orthogonal
  EXPECT_TRUE(rayson::vector3(-1.0, 0.0, 0.0).is_normalized());
  EXPECT_TRUE(rayson::vector3(0.0, -1.0, 0.0).is_normalized());
  EXPECT_TRUE(rayson::vector3(0.0, 0.0, -1.0).is_normalized());

  // within 1%
  EXPECT_TRUE(rayson::vector3(0.999, 0.0, 0.0).is_normalized());
  EXPECT_TRUE(rayson::vector3(0.0, 0.999, 0.0).is_normalized());
  EXPECT_TRUE(rayson::vector3(0.0, 0.0, 0.999).is_normalized());

  // not within 1%
  EXPECT_FALSE(rayson::vector3(0.98, 0.0, 0.0).is_normalized());
  EXPECT_FALSE(rayson::vector3(0.0, 0.98, 0.0).is_normalized());
  EXPECT_FALSE(rayson::vector3(0.0, 0.0, 0.98).is_normalized());

  // normalized example from http://www.fundza.com/vectors/normalize/
  EXPECT_TRUE(rayson::vector3(0.802, 0.267, 0.534).is_normalized());
}

TEST(color, ConstructorSettersAndGetters) {

  rayson::color black,
                r(1.0, 0.0, 0.0),
                g(0.0, 1.0, 0.0),
                b(0.0, 0.0, 1.0),
                other(.2, .3, .4);

  // invalid negative argument
  EXPECT_DEATH(rayson::color(-1.0, 0.0, 0.0), "");
  EXPECT_DEATH(rayson::color(0.0, -1.0, 0.0), "");
  EXPECT_DEATH(rayson::color(0.0, 0.0, -1.0), "");

  // invalid positive argument
  EXPECT_DEATH(rayson::color(1.1, 0.0, 0.0), "");
  EXPECT_DEATH(rayson::color(0.0, 1.1, 0.0), "");
  EXPECT_DEATH(rayson::color(0.0, 0.0, 1.1), "");

  // accessors
  EXPECT_DOUBLE_EQ(0.0, black.r());
  EXPECT_DOUBLE_EQ(0.0, black.g());
  EXPECT_DOUBLE_EQ(0.0, black.b());
  EXPECT_DOUBLE_EQ(1.0, r.r());
  EXPECT_DOUBLE_EQ(0.0, r.g());
  EXPECT_DOUBLE_EQ(0.0, r.b());
  EXPECT_DOUBLE_EQ(0.0, g.r());
  EXPECT_DOUBLE_EQ(1.0, g.g());
  EXPECT_DOUBLE_EQ(0.0, g.b());
  EXPECT_DOUBLE_EQ(0.0, b.r());
  EXPECT_DOUBLE_EQ(0.0, b.g());
  EXPECT_DOUBLE_EQ(1.0, b.b());
  EXPECT_DOUBLE_EQ(0.2, other.r());
  EXPECT_DOUBLE_EQ(0.3, other.g());
  EXPECT_DOUBLE_EQ(0.4, other.b());
}

TEST(camera, ConstructorSettersAndGetters) {

  rayson::vector3 x(1.0, 0.0, 0.0),
                  y(0.0, 1.0, 0.0),
                  z(0.0, 0.0, 1.0),
                  not_normal(3.0, 0.0, 0.0);

  rayson::camera cam(x, y, z);

  EXPECT_DOUBLE_EQ(x.x(), cam.eye().x());
  EXPECT_DOUBLE_EQ(x.y(), cam.eye().y());
  EXPECT_DOUBLE_EQ(x.z(), cam.eye().z());
  EXPECT_DOUBLE_EQ(y.x(), cam.up().x());
  EXPECT_DOUBLE_EQ(y.y(), cam.up().y());
  EXPECT_DOUBLE_EQ(y.z(), cam.up().z());
  EXPECT_DOUBLE_EQ(z.x(), cam.view().x());
  EXPECT_DOUBLE_EQ(z.y(), cam.view().y());
  EXPECT_DOUBLE_EQ(z.z(), cam.view().z());
}

TEST(viewport, ConstructorSettersAndGetters) {

  const unsigned x = 40, y = 30;
  const double left = -1.0, top = 2.0, right = 3.0, bottom = -4.0;

  rayson::viewport vp(x, y, left, top, right, bottom);

  EXPECT_EQ(x, vp.x_resolution());
  EXPECT_EQ(y, vp.y_resolution());

  EXPECT_DOUBLE_EQ(left, vp.left());
  EXPECT_DOUBLE_EQ(right, vp.right());
  EXPECT_DOUBLE_EQ(top, vp.top());
  EXPECT_DOUBLE_EQ(bottom, vp.bottom());

  EXPECT_DEATH(rayson::viewport(0, y, left, top, right, bottom), "");
  EXPECT_DEATH(rayson::viewport(x, 0, left, top, right, bottom), "");
  EXPECT_DEATH(rayson::viewport(x, y, +1.0, top, right, bottom), "");
  EXPECT_DEATH(rayson::viewport(x, y, left, -1.0, right, bottom), "");
  EXPECT_DEATH(rayson::viewport(x, y, left, top, -1.0, bottom), "");
  EXPECT_DEATH(rayson::viewport(x, y, left, top, right, +1.0), "");
}

TEST(ortho_projection, ConstructorSettersAndGetters) {

  // only thing to test is the default constructor
  rayson::ortho_projection proj;
}

TEST(persp_projection, ConstructorSettersAndGetters) {

  rayson::persp_projection proj(2.0);

  EXPECT_DOUBLE_EQ(2.0, proj.focal_length());

  EXPECT_DEATH(rayson::persp_projection(0.0), "");
  EXPECT_DEATH(rayson::persp_projection(-1.0), "");
}

TEST(projection, ConstructorSettersAndGetters) {

  // variant behavior
  rayson::projection proj;
  proj = rayson::ortho_projection();
  proj = rayson::persp_projection(2.0);
}

TEST(flat_shader, ConstructorSettersAndGetters) {

  // only thing to test is the default constructor
  rayson::flat_shader fs;
}

TEST(phong_shader, ConstructorSettersAndGetters) {

  rayson::color red(1.0, 0.0, 0.0);
  rayson::phong_shader fs(1.0, 2.0, 3.0, red);
  EXPECT_DOUBLE_EQ(1.0, fs.ambient_coeff());
  EXPECT_DOUBLE_EQ(2.0, fs.diffuse_coeff());
  EXPECT_DOUBLE_EQ(3.0, fs.specular_coeff());
  EXPECT_DOUBLE_EQ(red.r(), fs.ambient_color().r());
  EXPECT_DOUBLE_EQ(red.g(), fs.ambient_color().g());
  EXPECT_DOUBLE_EQ(red.b(), fs.ambient_color().b());

  EXPECT_DEATH(rayson::phong_shader(-1.0, 2.0, 3.0, red), "");
  EXPECT_DEATH(rayson::phong_shader(1.0, -2.0, 3.0, red), "");
  EXPECT_DEATH(rayson::phong_shader(1.0, 2.0, -3.0, red), "");
}

TEST(shader, ConstructorSettersAndGetters) {

  // variant behavior
  rayson::shader s;
  s = rayson::flat_shader();
  s = rayson::phong_shader(1, 2, 3, rayson::color());
}

TEST(material, ConstructorSettersAndGetters) {

  rayson::color white(1.0, 1.0, 1.0);

  // pass name by reference
  const std::string name = "linen";
  rayson::material m1(name, 4.0, white);
  EXPECT_EQ("linen", m1.name());
  EXPECT_DOUBLE_EQ(4.0, m1.shininess());
  EXPECT_DOUBLE_EQ(white.r(), m1.color().r());
  EXPECT_DOUBLE_EQ(white.g(), m1.color().g());
  EXPECT_DOUBLE_EQ(white.b(), m1.color().b());

  // pass name by rvalue reference
  rayson::material m2(std::move("paper"), 5.0, white);
  EXPECT_EQ("paper", m2.name());
  EXPECT_DOUBLE_EQ(5.0, m2.shininess());
  EXPECT_DOUBLE_EQ(white.r(), m2.color().r());
  EXPECT_DOUBLE_EQ(white.g(), m2.color().g());
  EXPECT_DOUBLE_EQ(white.b(), m2.color().b());

  // invalid shininess
  EXPECT_DEATH(rayson::material(name, 0.0, white), "");
  EXPECT_DEATH(rayson::material(name, -1.0, white), "");
}

TEST(sphere, ConstructorSettersAndGetters) {

  auto mat = rayson::material("paper", 2, rayson::color(1, 1, 1));
  rayson::sphere s(&mat, rayson::vector3(3, 4, 5), 6);
  EXPECT_EQ(&mat, &s.material());
  EXPECT_DOUBLE_EQ(3, s.center().x());
  EXPECT_DOUBLE_EQ(4, s.center().y());
  EXPECT_DOUBLE_EQ(5, s.center().z());
  EXPECT_DOUBLE_EQ(6, s.radius());

  EXPECT_DEATH(rayson::sphere(nullptr, s.center(), 6), "");
  EXPECT_DEATH(rayson::sphere(&mat, s.center(), 0.0), "");
  EXPECT_DEATH(rayson::sphere(&mat, s.center(), -1.0), "");
}

TEST(triangle, ConstructorSettersAndGetters) {
  auto mat = rayson::material("paper", 2, rayson::color(1, 1, 1));
  rayson::triangle tri(&mat,
                       rayson::vector3(1, 2, 3),
                       rayson::vector3(4, 5, 6),
                       rayson::vector3(7, 8, 9));
  EXPECT_EQ(&mat, &tri.material());
  EXPECT_EQ(1, tri.a().x());
  EXPECT_EQ(2, tri.a().y());
  EXPECT_EQ(3, tri.a().z());
  EXPECT_EQ(4, tri.b().x());
  EXPECT_EQ(5, tri.b().y());
  EXPECT_EQ(6, tri.b().z());
  EXPECT_EQ(7, tri.c().x());
  EXPECT_EQ(8, tri.c().y());
  EXPECT_EQ(9, tri.c().z());

  // null material
  EXPECT_DEATH(rayson::triangle(nullptr, tri.a(), tri.b(), tri.c()), "");
  // duplicated vertices
  EXPECT_DEATH(rayson::triangle(&mat, tri.a(), tri.a(), tri.c()), "");
  EXPECT_DEATH(rayson::triangle(&mat, tri.a(), tri.b(), tri.a()), "");
  EXPECT_DEATH(rayson::triangle(&mat, tri.b(), tri.b(), tri.c()), "");
  EXPECT_DEATH(rayson::triangle(&mat, tri.a(), tri.b(), tri.b()), "");
  EXPECT_DEATH(rayson::triangle(&mat, tri.c(), tri.b(), tri.c()), "");
  EXPECT_DEATH(rayson::triangle(&mat, tri.a(), tri.c(), tri.c()), "");
}

TEST(point_light, ConstructorSettersAndGetters) {
  rayson::point_light pl(rayson::vector3(1, 2, 3),
                         rayson::color(1, 1, 1),
                         4);
  EXPECT_DOUBLE_EQ(1, pl.location().x());
  EXPECT_DOUBLE_EQ(2, pl.location().y());
  EXPECT_DOUBLE_EQ(3, pl.location().z());
  EXPECT_DOUBLE_EQ(1, pl.color().r());
  EXPECT_DOUBLE_EQ(1, pl.color().g());
  EXPECT_DOUBLE_EQ(1, pl.color().b());
  EXPECT_DOUBLE_EQ(4, pl.intensity());

  EXPECT_DEATH(rayson::point_light(pl.location(), pl.color(), 0.0), "");
  EXPECT_DEATH(rayson::point_light(pl.location(), pl.color(), -1.0), "");
}

TEST(scene, ConstructorSettersAndGetters) {

  using rayson::vector3;
  using rayson::color;

  rayson::scene s(rayson::camera(vector3(.1, .2, .3),
                                 vector3(.4, .5, .6),
                                 vector3(.7, .8, .9)),
                  rayson::viewport(640, 480, -.10, .11, .12, -.13),
                  rayson::ortho_projection(),
                  rayson::flat_shader(),
                  color(.14, .15, .16));

  EXPECT_EQ(.1, s.camera().eye().x());
  EXPECT_EQ(.2, s.camera().eye().y());
  EXPECT_EQ(.3, s.camera().eye().z());
  EXPECT_EQ(.4, s.camera().up().x());
  EXPECT_EQ(.5, s.camera().up().y());
  EXPECT_EQ(.6, s.camera().up().z());
  EXPECT_EQ(.7, s.camera().view().x());
  EXPECT_EQ(.8, s.camera().view().y());
  EXPECT_EQ(.9, s.camera().view().z());
  EXPECT_EQ(640, s.viewport().x_resolution());
  EXPECT_EQ(480, s.viewport().y_resolution());
  EXPECT_EQ(-.10, s.viewport().left());
  EXPECT_EQ(.11, s.viewport().top());
  EXPECT_EQ(.12, s.viewport().right());
  EXPECT_EQ(-.13, s.viewport().bottom());
  EXPECT_TRUE(std::holds_alternative<rayson::ortho_projection>(s.projection()));
  EXPECT_TRUE(std::holds_alternative<rayson::flat_shader>(s.shader()));
  EXPECT_EQ(.14, s.background().r());
  EXPECT_EQ(.15, s.background().g());
  EXPECT_EQ(.16, s.background().b());

  EXPECT_TRUE(s.point_lights().empty());
  EXPECT_TRUE(s.materials().empty());
  EXPECT_TRUE(s.spheres().empty());
  EXPECT_TRUE(s.triangles().empty());

  s.emplace_point_light(rayson::point_light(vector3(.17, .18, .19),
                                            color(.20, .21, .22),
                                            .23));
  EXPECT_EQ(1, s.point_lights().size());
  EXPECT_EQ(0, s.materials().size());
  EXPECT_EQ(0, s.spheres().size());
  EXPECT_EQ(0, s.triangles().size());
  EXPECT_EQ(.17, s.point_lights().back().location().x());
  EXPECT_EQ(.18, s.point_lights().back().location().y());
  EXPECT_EQ(.19, s.point_lights().back().location().z());
  EXPECT_EQ(.20, s.point_lights().back().color().r());
  EXPECT_EQ(.21, s.point_lights().back().color().g());
  EXPECT_EQ(.22, s.point_lights().back().color().b());
  EXPECT_EQ(.23, s.point_lights().back().intensity());

  s.emplace_point_light(rayson::point_light(vector3(.24, .25, .26),
                                            color(.27, .28, .29),
                                            .30));
  EXPECT_EQ(2, s.point_lights().size());
  EXPECT_EQ(0, s.materials().size());
  EXPECT_EQ(0, s.spheres().size());
  EXPECT_EQ(0, s.triangles().size());
  EXPECT_EQ(.24, s.point_lights().back().location().x());
  EXPECT_EQ(.25, s.point_lights().back().location().y());
  EXPECT_EQ(.26, s.point_lights().back().location().z());
  EXPECT_EQ(.27, s.point_lights().back().color().r());
  EXPECT_EQ(.28, s.point_lights().back().color().g());
  EXPECT_EQ(.29, s.point_lights().back().color().b());
  EXPECT_EQ(.30, s.point_lights().back().intensity());

  s.emplace_material(rayson::material(".31", .32, color(.33, .34, .35)));
  EXPECT_EQ(2, s.point_lights().size());
  EXPECT_EQ(1, s.materials().size());
  EXPECT_EQ(0, s.spheres().size());
  EXPECT_EQ(0, s.triangles().size());
  EXPECT_EQ(".31", s.materials().back().name());
  EXPECT_EQ(.32, s.materials().back().shininess());
  EXPECT_EQ(.33, s.materials().back().color().r());
  EXPECT_EQ(.34, s.materials().back().color().g());
  EXPECT_EQ(.35, s.materials().back().color().b());

  s.emplace_sphere(rayson::sphere(&s.materials().back(), vector3(.36, .37, .38), .39));
  EXPECT_EQ(2, s.point_lights().size());
  EXPECT_EQ(1, s.materials().size());
  EXPECT_EQ(1, s.spheres().size());
  EXPECT_EQ(0, s.triangles().size());
  EXPECT_EQ(".31", s.spheres().back().material().name());
  EXPECT_EQ(.36, s.spheres().back().center().x());
  EXPECT_EQ(.37, s.spheres().back().center().y());
  EXPECT_EQ(.38, s.spheres().back().center().z());
  EXPECT_EQ(.39, s.spheres().back().radius());

  s.emplace_triangle(rayson::triangle(&s.materials().back(),
                                      vector3(.40, .41, .42),
                                      vector3(.43, .44, .45),
                                      vector3(.46, .47, .48)));
  EXPECT_EQ(2, s.point_lights().size());
  EXPECT_EQ(1, s.materials().size());
  EXPECT_EQ(1, s.spheres().size());
  EXPECT_EQ(1, s.triangles().size());
  EXPECT_EQ(".31", s.triangles().back().material().name());
  EXPECT_EQ(.40, s.triangles().back().a().x());
  EXPECT_EQ(.41, s.triangles().back().a().y());
  EXPECT_EQ(.42, s.triangles().back().a().z());
  EXPECT_EQ(.43, s.triangles().back().b().x());
  EXPECT_EQ(.44, s.triangles().back().b().y());
  EXPECT_EQ(.45, s.triangles().back().b().z());
  EXPECT_EQ(.46, s.triangles().back().c().x());
  EXPECT_EQ(.47, s.triangles().back().c().y());
  EXPECT_EQ(.48, s.triangles().back().c().z());
}

TEST(read_exception, ConstructorSettersAndGetters) {
  // const reference to message
  const std::string message("oops!");
  rayson::read_exception re1(message);
  EXPECT_EQ(message, re1.message());

  // rvalue reference to message
  rayson::read_exception re2(std::move("eek"));
  EXPECT_EQ("eek", re2.message());
}

TEST(read_json, Failures) {
  using rayson::read_json;
  using rayson::read_exception;
  using nlohmann::json;

  // JSON value is not a key-value object
  EXPECT_THROW(read_json(json(0)), read_exception);
  EXPECT_THROW(read_json(json(.123)), read_exception);
  EXPECT_THROW(read_json(json("string")), read_exception);
  EXPECT_THROW(read_json(json(true)), read_exception);
  EXPECT_THROW(read_json(json(nullptr)), read_exception);
}

TEST(read_json, RegressionTests) {
  using rayson::read_json;
  using rayson::read_exception;
  using nlohmann::json;

  // build a minimal-viable scene in JSON
  json valid;
  valid["camera_eye"] = {1.0, 1.1, 1.2};
  valid["camera_up"] = {1.3, 1.4, 1.5};
  valid["camera_view"] = {1.6, 1.7, 1.8};
  valid["x_resolution"] = 640;
  valid["y_resolution"] = 480;
  valid["viewport_left"] = -1.12;
  valid["viewport_top"] = 1.13;
  valid["viewport_right"] = 1.14;
  valid["viewport_bottom"] = -1.15;
  valid["ortho_projection"] = true;
  valid["flat_shader"] = true;
  valid["background"] = {.16, .17, .18};
  valid["point_lights"][0]["location"] = {1.19, 1.20, 1.21};
  valid["point_lights"][0]["color"] = {.22, .23, .24};
  valid["point_lights"][0]["intensity"] = .25;
  valid["materials"][0]["name"] = "my_material";
  valid["materials"][0]["shininess"] = .26;
  valid["materials"][0]["color"] = {.27, .28, .29};
  valid["spheres"][0]["material"] = "my_material";
  valid["spheres"][0]["center"] = {.30, .31, .32};
  valid["spheres"][0]["radius"] = .33;
  valid["triangles"][0]["material"] = "my_material";
  valid["triangles"][0]["a"] = {1.34, 1.35, 1.36};
  valid["triangles"][0]["b"] = {1.37, 1.38, 1.39};
  valid["triangles"][0]["c"] = {1.40, 1.41, 1.42};

  // confirm that it parses properly
  ASSERT_NO_THROW(read_json(valid));
  {
    auto scene = read_json(valid);

    EXPECT_DOUBLE_EQ(1.0, scene.camera().eye().x());
    EXPECT_DOUBLE_EQ(1.1, scene.camera().eye().y());
    EXPECT_DOUBLE_EQ(1.2, scene.camera().eye().z());
    EXPECT_DOUBLE_EQ(1.3, scene.camera().up().x());
    EXPECT_DOUBLE_EQ(1.4, scene.camera().up().y());
    EXPECT_DOUBLE_EQ(1.5, scene.camera().up().z());
    EXPECT_DOUBLE_EQ(1.6, scene.camera().view().x());
    EXPECT_DOUBLE_EQ(1.7, scene.camera().view().y());
    EXPECT_DOUBLE_EQ(1.8, scene.camera().view().z());
    EXPECT_EQ(640, scene.viewport().x_resolution());
    EXPECT_EQ(480, scene.viewport().y_resolution());
    EXPECT_DOUBLE_EQ(-1.12, scene.viewport().left());
    EXPECT_DOUBLE_EQ(1.13, scene.viewport().top());
    EXPECT_DOUBLE_EQ(1.14, scene.viewport().right());
    EXPECT_DOUBLE_EQ(-1.15, scene.viewport().bottom());
    EXPECT_TRUE(std::holds_alternative<rayson::ortho_projection>(scene.projection()));
    EXPECT_TRUE(std::holds_alternative<rayson::flat_shader>(scene.shader()));
    EXPECT_DOUBLE_EQ(.16, scene.background().r());
    EXPECT_DOUBLE_EQ(.17, scene.background().g());
    EXPECT_DOUBLE_EQ(.18, scene.background().b());
    ASSERT_EQ(1, scene.point_lights().size());
    EXPECT_DOUBLE_EQ(1.19, scene.point_lights()[0].location().x());
    EXPECT_DOUBLE_EQ(1.20, scene.point_lights()[0].location().y());
    EXPECT_DOUBLE_EQ(1.21, scene.point_lights()[0].location().z());
    EXPECT_DOUBLE_EQ(.22, scene.point_lights()[0].color().r());
    EXPECT_DOUBLE_EQ(.23, scene.point_lights()[0].color().g());
    EXPECT_DOUBLE_EQ(.24, scene.point_lights()[0].color().b());
    EXPECT_DOUBLE_EQ(.25, scene.point_lights()[0].intensity());
    ASSERT_EQ(1, scene.materials().size());
    EXPECT_EQ("my_material", scene.materials()[0].name());
    EXPECT_DOUBLE_EQ(.26, scene.materials()[0].shininess());
    EXPECT_DOUBLE_EQ(.27, scene.materials()[0].color().r());
    EXPECT_DOUBLE_EQ(.28, scene.materials()[0].color().g());
    EXPECT_DOUBLE_EQ(.29, scene.materials()[0].color().b());
    ASSERT_EQ(1, scene.spheres().size());
    auto& sphere = scene.spheres()[0];
    EXPECT_EQ(&scene.materials()[0], &sphere.material());
    EXPECT_DOUBLE_EQ(.30, sphere.center().x());
    EXPECT_DOUBLE_EQ(.31, sphere.center().y());
    EXPECT_DOUBLE_EQ(.32, sphere.center().z());
    EXPECT_DOUBLE_EQ(.33, sphere.radius());
    ASSERT_EQ(1, scene.triangles().size());
    auto& triangle = scene.triangles()[0];
    EXPECT_EQ(&scene.materials()[0], &triangle.material());
    EXPECT_DOUBLE_EQ(1.34, triangle.a().x());
    EXPECT_DOUBLE_EQ(1.35, triangle.a().y());
    EXPECT_DOUBLE_EQ(1.36, triangle.a().z());
    EXPECT_DOUBLE_EQ(1.37, triangle.b().x());
    EXPECT_DOUBLE_EQ(1.38, triangle.b().y());
    EXPECT_DOUBLE_EQ(1.39, triangle.b().z());
    EXPECT_DOUBLE_EQ(1.40, triangle.c().x());
    EXPECT_DOUBLE_EQ(1.41, triangle.c().y());
    EXPECT_DOUBLE_EQ(1.42, triangle.c().z());
  }

  // now test valid variations

  // perspective instead of ortho
  {
    auto with_persp = valid;
    with_persp.erase("ortho_projection");
    with_persp["persp_focal_length"] = 1.43;
    ASSERT_NO_THROW(read_json(with_persp));
    auto scene = read_json(with_persp);
    ASSERT_TRUE(std::holds_alternative<rayson::persp_projection>(scene.projection()));
    auto proj = std::get<rayson::persp_projection>(scene.projection());
    EXPECT_DOUBLE_EQ(1.43, proj.focal_length());
  }

  // phong shader instead of flat
  {
    auto with_phong = valid;
    with_phong.erase("flat_shader");
    with_phong["phong_shader"]["ambient_coeff"] = 1.43;
    with_phong["phong_shader"]["diffuse_coeff"] = 1.44;
    with_phong["phong_shader"]["specular_coeff"] = 1.45;
    with_phong["phong_shader"]["ambient_color"] = {.46, .47, .48};
    ASSERT_NO_THROW(read_json(with_phong));
    auto scene = read_json(with_phong);
    ASSERT_TRUE(std::holds_alternative<rayson::phong_shader>(scene.shader()));
    auto shader = std::get<rayson::phong_shader>(scene.shader());
    EXPECT_DOUBLE_EQ(1.43, shader.ambient_coeff());
    EXPECT_DOUBLE_EQ(1.44, shader.diffuse_coeff());
    EXPECT_DOUBLE_EQ(1.45, shader.specular_coeff());
    EXPECT_DOUBLE_EQ(.46, shader.ambient_color().r());
    EXPECT_DOUBLE_EQ(.47, shader.ambient_color().g());
    EXPECT_DOUBLE_EQ(.48, shader.ambient_color().b());
  }

  // additional point light
  {
    auto extra_light = valid;
    extra_light["point_lights"][1]["location"] = {.43, .44, .45};
    extra_light["point_lights"][1]["color"] = {.46, .47, .48};
    extra_light["point_lights"][1]["intensity"] = .49;
    ASSERT_NO_THROW(read_json(extra_light));
    auto scene = read_json(extra_light);
    ASSERT_EQ(2, scene.point_lights().size());
    EXPECT_DOUBLE_EQ(1.19, scene.point_lights()[0].location().x());
    EXPECT_DOUBLE_EQ(1.20, scene.point_lights()[0].location().y());
    EXPECT_DOUBLE_EQ(1.21, scene.point_lights()[0].location().z());
    EXPECT_DOUBLE_EQ(.22, scene.point_lights()[0].color().r());
    EXPECT_DOUBLE_EQ(.23, scene.point_lights()[0].color().g());
    EXPECT_DOUBLE_EQ(.24, scene.point_lights()[0].color().b());
    EXPECT_DOUBLE_EQ(.25, scene.point_lights()[0].intensity());
    EXPECT_DOUBLE_EQ(.43, scene.point_lights()[1].location().x());
    EXPECT_DOUBLE_EQ(.44, scene.point_lights()[1].location().y());
    EXPECT_DOUBLE_EQ(.45, scene.point_lights()[1].location().z());
    EXPECT_DOUBLE_EQ(.46, scene.point_lights()[1].color().r());
    EXPECT_DOUBLE_EQ(.47, scene.point_lights()[1].color().g());
    EXPECT_DOUBLE_EQ(.48, scene.point_lights()[1].color().b());
    EXPECT_DOUBLE_EQ(.49, scene.point_lights()[1].intensity());
  }

  // now check that specific format problems trigger exceptions

  // camera_eye
  {
    auto bad_camera_eye = valid;
    bad_camera_eye["camera_eye"] = true; // wrong type
    EXPECT_THROW(read_json(bad_camera_eye), read_exception);
    bad_camera_eye["camera_eye"] = {1.0, 2.0, true}; // wrong element type
    EXPECT_THROW(read_json(bad_camera_eye), read_exception);
    bad_camera_eye["camera_eye"] = {1.0, 2.0}; // too short
    EXPECT_THROW(read_json(bad_camera_eye), read_exception);
    bad_camera_eye["camera_eye"] = {1.0, 2.0, 3.0, 4.0}; // too long
    EXPECT_THROW(read_json(bad_camera_eye), read_exception);
    bad_camera_eye["camera_eye"] = valid["camera_eye"]; // works again
    EXPECT_NO_THROW(read_json(bad_camera_eye));
    bad_camera_eye.erase("camera_eye"); // missing
    EXPECT_THROW(read_json(bad_camera_eye), read_exception);
  }
  // camera_up
  {
    auto bad_camera_up = valid;
    bad_camera_up["camera_up"] = true; // wrong type
    EXPECT_THROW(read_json(bad_camera_up), read_exception);
    bad_camera_up["camera_up"] = {1.0, 2.0, true}; // wrong element type
    EXPECT_THROW(read_json(bad_camera_up), read_exception);
    bad_camera_up["camera_up"] = {1.0, 2.0}; // too short
    EXPECT_THROW(read_json(bad_camera_up), read_exception);
    bad_camera_up["camera_up"] = {1.0, 2.0, 3.0, 4.0}; // too long
    EXPECT_THROW(read_json(bad_camera_up), read_exception);
    bad_camera_up["camera_up"] = valid["camera_eye"]; // works again
    EXPECT_NO_THROW(read_json(bad_camera_up));
    bad_camera_up.erase("camera_up"); // missing
    EXPECT_THROW(read_json(bad_camera_up), read_exception);
  }
  // camera_view
  {
    auto bad_camera_view = valid;
    bad_camera_view["camera_view"] = true; // wrong type
    EXPECT_THROW(read_json(bad_camera_view), read_exception);
    bad_camera_view["camera_view"] = {1.0, 2.0, true}; // wrong element type
    EXPECT_THROW(read_json(bad_camera_view), read_exception);
    bad_camera_view["camera_view"] = {1.0, 2.0}; // too short
    EXPECT_THROW(read_json(bad_camera_view), read_exception);
    bad_camera_view["camera_view"] = {1.0, 2.0, 3.0, 4.0}; // too long
    EXPECT_THROW(read_json(bad_camera_view), read_exception);
    bad_camera_view["camera_view"] = valid["camera_eye"]; // works again
    EXPECT_NO_THROW(read_json(bad_camera_view));
    bad_camera_view.erase("camera_view"); // missing
    EXPECT_THROW(read_json(bad_camera_view), read_exception);
  }
  // x_resolution, should be a positive unsigned
  {
    auto bad_x_resolution = valid;
    bad_x_resolution["x_resolution"] = true; // wrong type
    EXPECT_THROW(read_json(bad_x_resolution), read_exception);
    bad_x_resolution["x_resolution"] = 0; // zero
    EXPECT_THROW(read_json(bad_x_resolution), read_exception);
    bad_x_resolution["x_resolution"] = -1; // negative
    EXPECT_THROW(read_json(bad_x_resolution), read_exception);
    bad_x_resolution["x_resolution"] = 1; // works again
    EXPECT_NO_THROW(read_json(bad_x_resolution));
    bad_x_resolution.erase("x_resolution"); // missing
    EXPECT_THROW(read_json(bad_x_resolution), read_exception);
  }
  // y_resolution, should be a positive unsigned
  {
    auto bad_y_resolution = valid;
    bad_y_resolution["y_resolution"] = true; // wrong type
    EXPECT_THROW(read_json(bad_y_resolution), read_exception);
    bad_y_resolution["y_resolution"] = 0; // zero
    EXPECT_THROW(read_json(bad_y_resolution), read_exception);
    bad_y_resolution["y_resolution"] = -1; // negative
    EXPECT_THROW(read_json(bad_y_resolution), read_exception);
    bad_y_resolution["y_resolution"] = 1; // works again
    EXPECT_NO_THROW(read_json(bad_y_resolution));
    bad_y_resolution.erase("y_resolution"); // missing
    EXPECT_THROW(read_json(bad_y_resolution), read_exception);
  }
  // viewport_left, should be a negative double
  {
    auto bad = valid;
    bad["viewport_left"] = true; // wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["viewport_left"] = 0; // zero
    EXPECT_THROW(read_json(bad), read_exception);
    bad["viewport_left"] = +1; // positive
    EXPECT_THROW(read_json(bad), read_exception);
    bad["viewport_left"] = -1.0; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad.erase("viewport_left"); // missing
    EXPECT_THROW(read_json(bad), read_exception);
  }
  // viewport_right, should be a positive double
  {
    auto bad = valid;
    bad["viewport_right"] = true; // wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["viewport_right"] = 0; // zero
    EXPECT_THROW(read_json(bad), read_exception);
    bad["viewport_right"] = -1; // negative
    EXPECT_THROW(read_json(bad), read_exception);
    bad["viewport_right"] = 1.0; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad.erase("viewport_right"); // missing
    EXPECT_THROW(read_json(bad), read_exception);
  }
  // viewport_top, should be a positive double
  {
    auto bad = valid;
    bad["viewport_top"] = true; // wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["viewport_top"] = 0; // zero
    EXPECT_THROW(read_json(bad), read_exception);
    bad["viewport_top"] = -1; // negative
    EXPECT_THROW(read_json(bad), read_exception);
    bad["viewport_top"] = 1.0; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad.erase("viewport_top"); // missing
    EXPECT_THROW(read_json(bad), read_exception);
  }
  // viewport_bottom, should be a negative double
  {
    auto bad = valid;
    bad["viewport_bottom"] = true; // wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["viewport_bottom"] = 0; // zero
    EXPECT_THROW(read_json(bad), read_exception);
    bad["viewport_bottom"] = +1; // positive
    EXPECT_THROW(read_json(bad), read_exception);
    bad["viewport_bottom"] = -1.0; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad.erase("viewport_bottom"); // missing
    EXPECT_THROW(read_json(bad), read_exception);
  }
  // ortho_projection, should be present and map to true
  {
    auto bad = valid;
    bad["ortho_projection"] = false;
    EXPECT_THROW(read_json(bad), read_exception);
    bad["ortho_projection"] = 1;
    EXPECT_THROW(read_json(bad), read_exception);
    bad["ortho_projection"] = 1.0;
    EXPECT_THROW(read_json(bad), read_exception);
    bad["ortho_projection"] = "str";
    EXPECT_THROW(read_json(bad), read_exception);
    bad.erase("ortho_projection");
    EXPECT_THROW(read_json(bad), read_exception);
    bad["ortho_projection"] = true;
    EXPECT_NO_THROW(read_json(bad));
  }
  // persp_focal_length, should be a positive double
  {
    auto bad = valid;
    bad["persp_focal_length"] = 1.0; // both ortho and persp at the same time
    EXPECT_THROW(read_json(bad), read_exception);
    bad.erase("ortho_projection"); // remove ortho, works again
    EXPECT_NO_THROW(read_json(bad));
    bad["persp_focal_length"] = true; // wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["persp_focal_length"] = 0.0; // zero
    EXPECT_THROW(read_json(bad), read_exception);
    bad["persp_focal_length"] = -1.0; // negative
    EXPECT_THROW(read_json(bad), read_exception);
    bad["persp_focal_length"] = 1.0; // works again
    EXPECT_NO_THROW(read_json(bad));
  }
  // flat_shader, should be present and map to true
  {
    auto bad = valid;
    bad["flat_shader"] = false;
    EXPECT_THROW(read_json(bad), read_exception);
    bad["flat_shader"] = 1;
    EXPECT_THROW(read_json(bad), read_exception);
    bad["flat_shader"] = 1.0;
    EXPECT_THROW(read_json(bad), read_exception);
    bad["flat_shader"] = "str";
    EXPECT_THROW(read_json(bad), read_exception);
    bad.erase("flat_shader");
    EXPECT_THROW(read_json(bad), read_exception);
    bad["flat_shader"] = true;
    EXPECT_NO_THROW(read_json(bad));
  }
  // phong shader, must be an object with ambient_coeff, diffuse_coeff, specular_coeff, ambient_color
  {
    // first change flat_shader to a valid phong
    auto good = valid;
    good.erase("flat_shader");
    EXPECT_THROW(read_json(good), read_exception);
    good["phong_shader"]["ambient_coeff"] = .50;
    good["phong_shader"]["diffuse_coeff"] = .51;
    good["phong_shader"]["specular_coeff"] = .52;
    good["phong_shader"]["ambient_color"] = {.53, .54, .55};
    ASSERT_NO_THROW(read_json(good));
    auto scene = read_json(good);
    ASSERT_TRUE(std::holds_alternative<rayson::phong_shader>(scene.shader()));
    auto shader = std::get<rayson::phong_shader>(scene.shader());
    EXPECT_DOUBLE_EQ(.50, shader.ambient_coeff());
    EXPECT_DOUBLE_EQ(.51, shader.diffuse_coeff());
    EXPECT_DOUBLE_EQ(.52, shader.specular_coeff());
    EXPECT_DOUBLE_EQ(.53, shader.ambient_color().r());
    EXPECT_DOUBLE_EQ(.54, shader.ambient_color().g());
    EXPECT_DOUBLE_EQ(.55, shader.ambient_color().b());

    // now try problems
    auto bad = good;
    // ambient_coeff
    bad["phong_shader"].erase("ambient_coeff"); // absent
    EXPECT_THROW(read_json(bad), read_exception);
    bad["phong_shader"]["ambient_coeff"] = true; // wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["phong_shader"]["ambient_coeff"] = -1.0; // negative
    EXPECT_THROW(read_json(bad), read_exception);
    bad["phong_shader"]["ambient_coeff"] = 1.0; // works again
    EXPECT_NO_THROW(read_json(bad));
    // diffuse_coeff
    bad = good;
    bad["phong_shader"].erase("diffuse_coeff"); // absent
    EXPECT_THROW(read_json(bad), read_exception);
    bad["phong_shader"]["diffuse_coeff"] = true; // wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["phong_shader"]["diffuse_coeff"] = -1.0; // negative
    EXPECT_THROW(read_json(bad), read_exception);
    bad["phong_shader"]["diffuse_coeff"] = 1.0; // works again
    EXPECT_NO_THROW(read_json(bad));
    // specular_coeff
    bad = good;
    bad["phong_shader"].erase("specular_coeff"); // absent
    EXPECT_THROW(read_json(bad), read_exception);
    bad["phong_shader"]["specular_coeff"] = true; // wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["phong_shader"]["specular_coeff"] = -1.0; // negative
    EXPECT_THROW(read_json(bad), read_exception);
    bad["phong_shader"]["specular_coeff"] = 1.0; // works again
    EXPECT_NO_THROW(read_json(bad));
    // ambient_color
    bad = good;
    bad["phong_shader"].erase("ambient_color"); // absent
    EXPECT_THROW(read_json(bad), read_exception);
    bad["phong_shader"]["ambient_color"] = true; // wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["phong_shader"]["ambient_color"] = {0}; // too short
    EXPECT_THROW(read_json(bad), read_exception);
    bad["phong_shader"]["ambient_color"] = {0, 0, 0, 0}; // too long
    EXPECT_THROW(read_json(bad), read_exception);
    bad["phong_shader"]["ambient_color"] = {0, 0, 2}; // out of range
    EXPECT_THROW(read_json(bad), read_exception);
    bad["phong_shader"]["ambient_color"] = good["phong_shader"]["ambient_color"]; // works again
    EXPECT_NO_THROW(read_json(bad));
  }
  // background, must be a color
  {
    auto bad = valid;
    bad.erase("background"); // absent
    EXPECT_THROW(read_json(bad), read_exception);
    bad["background"] = true; // wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["background"] = {0}; // too short
    EXPECT_THROW(read_json(bad), read_exception);
    bad["background"] = {0, 0, 0, 0}; // too long
    EXPECT_THROW(read_json(bad), read_exception);
    bad["background"] = {0, 0, 2}; // out of range
    EXPECT_THROW(read_json(bad), read_exception);
    bad["background"] = valid["background"]; // works again
    EXPECT_NO_THROW(read_json(bad));
  }
  // point_lights
  {
    auto bad = valid;
    bad["point_lights"] = true; // wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["point_lights"] = {0}; // one element of wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["point_lights"][1] = {0}; // includes an element of wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["point_lights"] = valid["point_lights"]; // works again
    EXPECT_NO_THROW(read_json(bad));

    bad["point_lights"][0].erase("location"); // missing location
    EXPECT_THROW(read_json(bad), read_exception);
    bad["point_lights"] = valid["point_lights"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["point_lights"][0]["location"] = true; // bad location
    EXPECT_THROW(read_json(bad), read_exception);
    bad["point_lights"] = valid["point_lights"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["point_lights"][0].erase("color"); // missing color
    EXPECT_THROW(read_json(bad), read_exception);
    bad["point_lights"] = valid["point_lights"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["point_lights"][0]["color"] = true; // bad color
    EXPECT_THROW(read_json(bad), read_exception);
    bad["point_lights"] = valid["point_lights"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["point_lights"][0].erase("intensity"); // missing intensity
    EXPECT_THROW(read_json(bad), read_exception);
    bad["point_lights"] = valid["point_lights"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["point_lights"][0]["intensity"] = true; // intensity wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["point_lights"][0]["intensity"] = 0.0; // intensity zero
    EXPECT_THROW(read_json(bad), read_exception);
    bad["point_lights"][0]["intensity"] = -1.0; // intensity negative
    EXPECT_THROW(read_json(bad), read_exception);
    bad["point_lights"][0]["intensity"] = 1.0; // works again
    EXPECT_NO_THROW(read_json(bad));
  }
  // materials
  {
    auto bad = valid;
    bad["materials"] = true; // wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["materials"] = {0}; // one element of wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["materials"][1] = {0}; // includes an element of wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["materials"] = valid["materials"]; // works again
    EXPECT_NO_THROW(read_json(bad));

    bad["materials"][0].erase("name"); // missing name
    EXPECT_THROW(read_json(bad), read_exception);
    bad["materials"] = valid["materials"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["materials"][0]["name"] = true; // bad name
    EXPECT_THROW(read_json(bad), read_exception);
    bad["materials"] = valid["materials"]; // works again
    EXPECT_NO_THROW(read_json(bad));

    bad["materials"][0].erase("shininess"); // missing shininess
    EXPECT_THROW(read_json(bad), read_exception);
    bad["materials"] = valid["materials"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["materials"][0]["shininess"] = true; // shininess wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["materials"] = valid["materials"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["materials"][0]["shininess"] = -1.0; // shininess negative
    EXPECT_THROW(read_json(bad), read_exception);
    bad["materials"] = valid["materials"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["materials"][0]["shininess"] = 0.0; // shininess zero
    EXPECT_THROW(read_json(bad), read_exception);
    bad["materials"] = valid["materials"]; // works again
    EXPECT_NO_THROW(read_json(bad));

    bad["materials"][0].erase("color"); // missing color
    EXPECT_THROW(read_json(bad), read_exception);
    bad["materials"] = valid["materials"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["materials"][0]["color"] = true; // color wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["materials"] = valid["materials"]; // works again

    // duplicate name
    bad = valid;
    EXPECT_NO_THROW(read_json(bad));
    bad["materials"][1] = bad["materials"][0];
    EXPECT_THROW(read_json(bad), read_exception);
    // changing name resolves the problem
    bad["materials"][1]["name"] = "different name";
    EXPECT_NO_THROW(read_json(bad));
  }
  // spheres
  {
    auto bad = valid;
    bad["spheres"] = true; // wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["spheres"] = {0}; // one element of wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["spheres"][1] = {0}; // includes an element of wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["spheres"] = valid["spheres"]; // works again
    EXPECT_NO_THROW(read_json(bad));

    bad["spheres"][0].erase("material"); // missing material
    EXPECT_THROW(read_json(bad), read_exception);
    bad["spheres"] = valid["spheres"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["spheres"][0]["material"] = true; // material wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["spheres"] = valid["spheres"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["spheres"][0]["material"] = "missing"; // material no such name
    EXPECT_THROW(read_json(bad), read_exception);
    bad["spheres"] = valid["spheres"]; // works again
    EXPECT_NO_THROW(read_json(bad));

    bad["spheres"][0].erase("center"); // missing center
    EXPECT_THROW(read_json(bad), read_exception);
    bad["spheres"] = valid["spheres"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["spheres"][0]["center"] = true; // center wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["spheres"] = valid["spheres"]; // works again
    EXPECT_NO_THROW(read_json(bad));

    bad["spheres"][0].erase("radius"); // missing radius
    EXPECT_THROW(read_json(bad), read_exception);
    bad["spheres"] = valid["spheres"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["spheres"][0]["radius"] = true; // radius wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["spheres"] = valid["spheres"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["spheres"][0]["radius"] = -1.0; // radius negative
    EXPECT_THROW(read_json(bad), read_exception);
    bad["spheres"] = valid["spheres"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["spheres"][0]["radius"] = 0.0; // radius zero
    EXPECT_THROW(read_json(bad), read_exception);
    bad["spheres"] = valid["spheres"]; // works again
    EXPECT_NO_THROW(read_json(bad));
  }
  // triangles
  {
    auto bad = valid;
    bad["triangles"] = true; // wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["triangles"] = {0}; // one element of wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["triangles"][1] = {0}; // includes an element of wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["triangles"] = valid["triangles"]; // works again
    EXPECT_NO_THROW(read_json(bad));

    bad["triangles"][0].erase("material"); // missing material
    EXPECT_THROW(read_json(bad), read_exception);
    bad["triangles"] = valid["triangles"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["triangles"][0]["material"] = true; // material wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["triangles"] = valid["triangles"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["triangles"][0]["material"] = "missing"; // material no such name
    EXPECT_THROW(read_json(bad), read_exception);
    bad["triangles"] = valid["triangles"]; // works again
    EXPECT_NO_THROW(read_json(bad));

    bad["triangles"][0].erase("a"); // missing a
    EXPECT_THROW(read_json(bad), read_exception);
    bad["triangles"] = valid["triangles"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["triangles"][0]["a"] = true; // a wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["triangles"] = valid["triangles"]; // works again
    EXPECT_NO_THROW(read_json(bad));

    bad["triangles"][0].erase("b"); // missing a
    EXPECT_THROW(read_json(bad), read_exception);
    bad["triangles"] = valid["triangles"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["triangles"][0]["b"] = true; // a wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["triangles"] = valid["triangles"]; // works again
    EXPECT_NO_THROW(read_json(bad));

    bad["triangles"][0].erase("c"); // missing a
    EXPECT_THROW(read_json(bad), read_exception);
    bad["triangles"] = valid["triangles"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["triangles"][0]["c"] = true; // a wrong type
    EXPECT_THROW(read_json(bad), read_exception);
    bad["triangles"] = valid["triangles"]; // works again
    EXPECT_NO_THROW(read_json(bad));

    bad["triangles"] = valid["triangles"];
    bad["triangles"][0]["b"] = bad["triangles"][0]["a"]; // a, a, c
    EXPECT_THROW(read_json(bad), read_exception);
    bad["triangles"] = valid["triangles"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["triangles"][0]["c"] = bad["triangles"][0]["a"]; // a, b, a
    EXPECT_THROW(read_json(bad), read_exception);
    bad["triangles"] = valid["triangles"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["triangles"][0]["a"] = bad["triangles"][0]["b"]; // b, b, c
    EXPECT_THROW(read_json(bad), read_exception);
    bad["triangles"] = valid["triangles"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["triangles"][0]["c"] = bad["triangles"][0]["b"]; // a, b, b
    EXPECT_THROW(read_json(bad), read_exception);
    bad["triangles"] = valid["triangles"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["triangles"][0]["a"] = bad["triangles"][0]["c"]; // c, b, c
    EXPECT_THROW(read_json(bad), read_exception);
    bad["triangles"] = valid["triangles"]; // works again
    EXPECT_NO_THROW(read_json(bad));
    bad["triangles"][0]["b"] = bad["triangles"][0]["c"]; // a, c, c
    EXPECT_THROW(read_json(bad), read_exception);
    bad["triangles"] = valid["triangles"]; // works again
    EXPECT_NO_THROW(read_json(bad));
  }
}

TEST(read_file, SampleFiles) {
  {
    const std::string path = "scene_2spheres_ortho_flat.json";
    ASSERT_NO_THROW(rayson::read_file(path));
    auto scene = rayson::read_file(path);
    bool is_ortho = std::holds_alternative<rayson::ortho_projection>(scene.projection());
    bool is_flat = std::holds_alternative<rayson::flat_shader>(scene.shader());
    EXPECT_TRUE(is_ortho);
    EXPECT_TRUE(is_flat);
    EXPECT_EQ(2, scene.materials().size());
    EXPECT_EQ(2, scene.point_lights().size());
    EXPECT_EQ(2, scene.spheres().size());
    EXPECT_EQ(0, scene.triangles().size());
  }

  {
    const std::string path = "scene_2spheres_ortho_phong.json";
    ASSERT_NO_THROW(rayson::read_file(path));
    auto scene = rayson::read_file(path);
    bool is_ortho = std::holds_alternative<rayson::ortho_projection>(scene.projection());
    bool is_flat = std::holds_alternative<rayson::flat_shader>(scene.shader());
    EXPECT_TRUE(is_ortho);
    EXPECT_FALSE(is_flat);
    EXPECT_EQ(2, scene.materials().size());
    EXPECT_EQ(2, scene.point_lights().size());
    EXPECT_EQ(2, scene.spheres().size());
    EXPECT_EQ(0, scene.triangles().size());
  }

  {
    const std::string path = "scene_2spheres_persp_flat.json";
    ASSERT_NO_THROW(rayson::read_file(path));
    auto scene = rayson::read_file(path);
    bool is_ortho = std::holds_alternative<rayson::ortho_projection>(scene.projection());
    bool is_flat = std::holds_alternative<rayson::flat_shader>(scene.shader());
    EXPECT_FALSE(is_ortho);
    EXPECT_TRUE(is_flat);
    EXPECT_EQ(2, scene.materials().size());
    EXPECT_EQ(2, scene.point_lights().size());
    EXPECT_EQ(2, scene.spheres().size());
    EXPECT_EQ(0, scene.triangles().size());
  }

  {
    const std::string path = "scene_2spheres_persp_phong.json";
    ASSERT_NO_THROW(rayson::read_file(path));
    auto scene = rayson::read_file(path);
    bool is_ortho = std::holds_alternative<rayson::ortho_projection>(scene.projection());
    bool is_flat = std::holds_alternative<rayson::flat_shader>(scene.shader());
    EXPECT_FALSE(is_ortho);
    EXPECT_FALSE(is_flat);
    EXPECT_EQ(2, scene.materials().size());
    EXPECT_EQ(2, scene.point_lights().size());
    EXPECT_EQ(2, scene.spheres().size());
    EXPECT_EQ(0, scene.triangles().size());
  }

  {
    const std::string path = "scene_gtri_ortho_flat.json";
    ASSERT_NO_THROW(rayson::read_file(path));
    auto scene = rayson::read_file(path);
    bool is_ortho = std::holds_alternative<rayson::ortho_projection>(scene.projection());
    bool is_flat = std::holds_alternative<rayson::flat_shader>(scene.shader());
    EXPECT_TRUE(is_ortho);
    EXPECT_TRUE(is_flat);
    EXPECT_EQ(1, scene.materials().size());
    EXPECT_EQ(2, scene.point_lights().size());
    EXPECT_EQ(0, scene.spheres().size());
    EXPECT_EQ(1, scene.triangles().size());
  }

  {
    const std::string path = "scene_gtri_ortho_phong.json";
    ASSERT_NO_THROW(rayson::read_file(path));
    auto scene = rayson::read_file(path);
    bool is_ortho = std::holds_alternative<rayson::ortho_projection>(scene.projection());
    bool is_flat = std::holds_alternative<rayson::flat_shader>(scene.shader());
    EXPECT_TRUE(is_ortho);
    EXPECT_FALSE(is_flat);
    EXPECT_EQ(1, scene.materials().size());
    EXPECT_EQ(2, scene.point_lights().size());
    EXPECT_EQ(0, scene.spheres().size());
    EXPECT_EQ(1, scene.triangles().size());
  }

  {
    const std::string path = "scene_gtri_persp_flat.json";
    ASSERT_NO_THROW(rayson::read_file(path));
    auto scene = rayson::read_file(path);
    bool is_ortho = std::holds_alternative<rayson::ortho_projection>(scene.projection());
    bool is_flat = std::holds_alternative<rayson::flat_shader>(scene.shader());
    EXPECT_FALSE(is_ortho);
    EXPECT_TRUE(is_flat);
    EXPECT_EQ(1, scene.materials().size());
    EXPECT_EQ(2, scene.point_lights().size());
    EXPECT_EQ(0, scene.spheres().size());
    EXPECT_EQ(1, scene.triangles().size());
  }

  {
    const std::string path = "scene_gtri_persp_phong.json";
    ASSERT_NO_THROW(rayson::read_file(path));
    auto scene = rayson::read_file(path);
    bool is_ortho = std::holds_alternative<rayson::ortho_projection>(scene.projection());
    bool is_flat = std::holds_alternative<rayson::flat_shader>(scene.shader());
    EXPECT_FALSE(is_ortho);
    EXPECT_FALSE(is_flat);
    EXPECT_EQ(1, scene.materials().size());
    EXPECT_EQ(2, scene.point_lights().size());
    EXPECT_EQ(0, scene.spheres().size());
    EXPECT_EQ(1, scene.triangles().size());
  }
}
