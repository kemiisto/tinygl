#include "tinygl/vec.h"
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <glm/glm.hpp>

TEST_CASE("Vec<4> is constructed from initializer list", "[Vec<4>]")
{
    tinygl::Vec<4> v {0.0f, 0.1f, 0.2f, 0.3f};
    std::array<float,4> a {0.0f, 0.1f, 0.2f, 0.3f};

    for (std::size_t i = 0; i < 4; ++i) {
        REQUIRE(v[i] == Approx(a[i]));
    }
}

TEST_CASE("Vec<4> component access", "[Vec<4>]")
{
    tinygl::Vec<4> v {0.0f, 0.1f, 0.2f, 0.3f};
    glm::vec4 a {0.0f, 0.1f, 0.2f, 0.3f};

    REQUIRE(v.x() == Approx(a.x));
    REQUIRE(v.y() == Approx(a.y));
    REQUIRE(v.z() == Approx(a.z));
    REQUIRE(v.w() == Approx(a.w));
}
