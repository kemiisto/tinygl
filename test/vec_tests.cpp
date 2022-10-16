#include "tinygl/vec.h"
#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>
#include <glm/glm.hpp>

TEST_CASE("Vec2 is constructed from initializer list", "[Vec2]")
{
    tinygl::Vec2 v {0.0f, 0.1f};
    std::array<float,2> a {0.0f, 0.1f};

    for (std::size_t i = 0; i < 2; ++i) {
        REQUIRE(v[i] == Catch::Approx(a[i]));
    }
}

TEST_CASE("Vec3 is constructed from initializer list", "[Vec3]")
{
    tinygl::Vec3 v {0.0f, 0.1f, 0.2f};
    std::array<float,3> a {0.0f, 0.1f, 0.2f};

    for (std::size_t i = 0; i < 3; ++i) {
        REQUIRE(v[i] == Catch::Approx(a[i]));
    }
}

TEST_CASE("Vec4 is constructed from initializer list", "[Vec4]")
{
    tinygl::Vec4 v {0.0f, 0.1f, 0.2f, 0.3f};
    std::array<float,4> a {0.0f, 0.1f, 0.2f, 0.3f};

    for (std::size_t i = 0; i < 4; ++i) {
        REQUIRE(v[i] == Catch::Approx(a[i]));
    }
}

TEST_CASE("Vec4 component access", "[Vec4]")
{
    tinygl::Vec<4> v {0.0f, 0.1f, 0.2f, 0.3f};
    glm::vec4 a {0.0f, 0.1f, 0.2f, 0.3f};

    REQUIRE(v.x() == Catch::Approx(a.x));
    REQUIRE(v.y() == Catch::Approx(a.y));
    REQUIRE(v.z() == Catch::Approx(a.z));
    REQUIRE(v.w() == Catch::Approx(a.w));
}

TEST_CASE("Vec4 data", "[Vec4]")
{
    tinygl::Vec<4> v {0.0f, 0.1f, 0.2f, 0.3f};
    float a[4] {0.0f, 0.1f, 0.2f, 0.3f};

    float* data = v.data();
    for (std::size_t i = 0; i < 4; ++i) {
        REQUIRE(data[i] == Catch::Approx(a[i]));
    }
}

int main(int argc, const char* argv[])
{
    return Catch::Session().run(argc, argv);
}
