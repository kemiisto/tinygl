#include "tinygl/mat.h"
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
            REQUIRE(tglMatrix[j,i] == Catch::Approx(glmMatrix[j][i]));
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
    auto tglMatrix = tinygl::Mat4::identity();
    auto glmMatrix = glm::mat4(1.0f);
    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 is constructed as translation matrix", "[Mat4]")
{
    auto tglMatrix = tinygl::Mat4::translation({1.0f, 2.0f, 3.0f});
    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::translate(glmMatrix, glm::vec3(1.0f, 2.0f, 3.0f));
    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 is constructed as scale matrix", "[Mat4]")
{
    auto tglMatrix = tinygl::Mat4::scale({1.0f, 2.0f, 3.0f});
    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::scale(glmMatrix, glm::vec3(1.0f, 2.0f, 3.0f));
    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 data method", "[Mat4]")
{
    auto tglMatrix = tinygl::Mat4::translation({1.0f, 2.0f, 3.0f});
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

