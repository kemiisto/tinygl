#include "tinygl/mat.h"
#include "tinygl/util.h"
#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void compare(const tinygl::Mat4& tglMatrix, const glm::mat4& glmMatrix)
{
    for (std::size_t j = 0; j < 4; ++j) {
        for (std::size_t i = 0; i < 4; ++i) {
            REQUIRE(tglMatrix(j,i) == Catch::Approx(glmMatrix[j][i]));
        }
    }
}

TEST_CASE("Mat4 is constructed from initializer list", "[Mat4]")
{
    tinygl::Mat4 tglMatrix {
        0.0f, 0.1f, 0.2f, 0.3f,
        1.0f, 1.1f, 1.2f, 1.3f,
        2.0f, 2.1f, 2.2f, 2.3f,
        3.0f, 3.1f, 3.2f, 3.3f
    };

    float a[16] = {
        0.0f, 0.1f, 0.2f, 0.3f,
        1.0f, 1.1f, 1.2f, 1.3f,
        2.0f, 2.1f, 2.2f, 2.3f,
        3.0f, 3.1f, 3.2f, 3.3f
    };
    glm::mat4 glmMatrix = glm::transpose(glm::make_mat4(a));

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 is constructed as identity matrix", "[Mat4]")
{
    auto tglMatrix = tinygl::Mat4{};
    auto glmMatrix = glm::mat4(1.0f);
    compare(tglMatrix, glmMatrix);
}


TEST_CASE("Mat4 translate", "[Mat4]")
{
    auto tglMatrix = tinygl::Mat4{};
    tglMatrix.translate({1.0f, 2.0f, 3.0f});

    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::translate(glmMatrix, glm::vec3(1.0f, 2.0f, 3.0f));

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 scale", "[Mat4]")
{
    auto tglMatrix = tinygl::Mat4{};
    tglMatrix.scale({1.0f, 2.0f, 3.0f});

    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::scale(glmMatrix, glm::vec3(1.0f, 2.0f, 3.0f));

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 rotate around x-axis", "[Mat4]")
{
    auto tglMatrix = tinygl::Mat4{};
    tglMatrix.rotate(45.0f, {1.0f, 0.0f, 0.0f});

    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::rotate(glmMatrix, tinygl::degreesToRadians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 rotate around y-axis", "[Mat4]")
{
    auto tglMatrix = tinygl::Mat4{};
    tglMatrix.rotate(45.0f, {0.0f, 1.0f, 0.0f});

    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::rotate(glmMatrix, tinygl::degreesToRadians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 rotate around z-axis", "[Mat4]")
{
    auto tglMatrix = tinygl::Mat4{};
    tglMatrix.rotate(45.0f, {0.0f, 0.0f, 1.0f});

    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::rotate(glmMatrix, tinygl::degreesToRadians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("operator*=", "[Mat4]")
{
    const auto zero = tinygl::Mat4 {
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f
    };

    const auto unique = tinygl::Mat4 {
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        9.0f, 10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    };

    const auto identity = tinygl::Mat4 {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    auto m = zero;
    m *= zero;
    REQUIRE(m.closeTo(zero));

    m = zero;
    m *= unique;
    REQUIRE(m.closeTo(zero));

    m = unique;
    m *= zero;
    REQUIRE(m.closeTo(zero));

    m = identity;
    m *= unique;
    REQUIRE(m.closeTo(unique));

    m = unique;
    m *= identity;
    REQUIRE(m.closeTo(unique));
}

TEST_CASE("operator*", "[Mat4]")
{
    const auto zero = tinygl::Mat4 {
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f
    };

    const auto unique = tinygl::Mat4 {
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        9.0f, 10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    };

    const auto identity = tinygl::Mat4 {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    auto m = zero * zero;
    REQUIRE(m.closeTo(zero));

    m = zero * unique;
    REQUIRE(m.closeTo(zero));

    m = unique * zero;
    REQUIRE(m.closeTo(zero));

    m = identity * unique;
    REQUIRE(m.closeTo(unique));

    m = unique * identity;
    REQUIRE(m.closeTo(unique));
}

TEST_CASE("Mat4 data", "[Mat4]")
{
    auto tglMatrix = tinygl::Mat4{};
    tglMatrix.translate({1.0f, 2.0f, 3.0f});

    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::translate(glmMatrix, glm::vec3(1.0f, 2.0f, 3.0f));

    auto* tglData = tglMatrix.data();
    auto* glmData = glm::value_ptr(glmMatrix);

    for (std::size_t i = 0; i < 4*4; ++i) {
        REQUIRE(tglData[i] == Catch::Approx(glmData[i]));
    }
}

int main(int argc, const char* argv[])
{
    return Catch::Session().run(argc, argv);
}

