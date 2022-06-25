#include "tinygl/mat.h"
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void compare(const tinygl::Mat<4>& tglMatrix, const glm::mat4& glmMatrix)
{
    for (std::size_t j = 0; j < 4; ++j) {
        for (std::size_t i = 0; i < 4; ++i) {
            REQUIRE(tglMatrix[j,i] == Approx(glmMatrix[j][i]));
        }
    }
}

TEST_CASE("Mat<4> is constructed from initializer list", "[Mat<4>]")
{
    tinygl::Mat<4> tglMatrix {
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

TEST_CASE("Mat<4> is constructed as identity matrix", "[Mat<4>]")
{
    auto tglMatrix = tinygl::Mat<4>::identity();
    auto glmMatrix = glm::mat4(1.0f);
    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat<4> is constructed as translation matrix", "[Mat<4>]")
{
    auto tglMatrix = tinygl::Mat<4>::translation({1.0f, 2.0f, 3.0f});
    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::translate(glmMatrix, glm::vec3(1.0f, 2.0f, 3.0f));
    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat<4> is constructed as scale matrix", "[Mat<4>]")
{
    auto tglMatrix = tinygl::Mat<4>::scale({1.0f, 2.0f, 3.0f});
    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::scale(glmMatrix, glm::vec3(1.0f, 2.0f, 3.0f));
    compare(tglMatrix, glmMatrix);
}
