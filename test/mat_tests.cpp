#include "tinygl/mat.h"
#include "tinygl/util.h"
#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>
#define GLM_FORCE_SIZE_T_LENGTH
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

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

template<std::size_t N, typename T>
void compare(const tinygl::Mat<N,T>& tglMatrix, const glm::mat<N,N,T>& glmMatrix)
{
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            CAPTURE(i);
            CAPTURE(j);
            REQUIRE(tglMatrix(i, j) == Catch::Approx(glmMatrix[i][j]));
        }
    }
}

TEST_CASE("Mat3 is constructed from initializer list", "[Mat3]")
{
    tinygl::Mat3 tglMatrix {
        0.0f, 0.1f, 0.2f,
        1.0f, 1.1f, 1.2f,
        2.0f, 2.1f, 2.2f
    };

    float a[] = {
        0.0f, 0.1f, 0.2f,
        1.0f, 1.1f, 1.2f,
        2.0f, 2.1f, 2.2f
    };
    glm::mat3 glmMatrix = glm::transpose(glm::make_mat3(a));

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 is constructed from initializer list", "[Mat4]")
{
    tinygl::Mat4 tglMatrix {
        0.0f, 0.1f, 0.2f, 0.3f,
        1.0f, 1.1f, 1.2f, 1.3f,
        2.0f, 2.1f, 2.2f, 2.3f,
        3.0f, 3.1f, 3.2f, 3.3f
    };

    float a[] = {
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

TEST_CASE("Mat4 scaling", "[Mat4]")
{
    auto tglMatrix = tinygl::Mat4::scaling(tinygl::Vec3{2.0f, 3.0f, 4.0f});

    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::scale(glmMatrix, glm::vec3(2.0f, 3.0f, 4.0f));

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 preScale", "[Mat4]")
{
    auto tglMatrix = unique;
    tglMatrix.preScale(tinygl::Vec3{2.0f, 3.0f, 4.0f});

    auto scalingMatrix = glm::mat4(1.0f);
    scalingMatrix = glm::scale(scalingMatrix, glm::vec3(2.0f, 3.0f, 4.0f));
    auto glmMatrix = scalingMatrix * glm::make_mat4(unique.data());

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 postScale", "[Mat4]")
{
    auto tglMatrix = unique;
    tglMatrix.postScale(tinygl::Vec3{2.0f, 3.0f, 4.0f});

    auto glmMatrix = glm::make_mat4(unique.data());
    glmMatrix = glm::scale(glmMatrix, glm::vec3(2.0f, 3.0f, 4.0f));

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 scaling benchmark", "[Mat4]")
{
    BENCHMARK("preScale") {
        auto m = unique;
        m.preScale(tinygl::Vec3{2.0f, 3.0f, 4.0f});
        return m;
    };

    BENCHMARK("preScale by matrix multiplication") {
        auto t = tinygl::Mat4::scaling(tinygl::Vec3{2.0f, 3.0f, 4.0f});
        auto m = unique;
        m = t * m;
        return m;
    };
}

TEST_CASE("Mat4 translation", "[Mat4]")
{
    auto tglMatrix = tinygl::Mat4::translation(tinygl::Vec3{1.0f, 2.0f, 3.0f});

    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::translate(glmMatrix, glm::vec3(1.0f, 2.0f, 3.0f));

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 preTranslate", "[Mat4]")
{
    auto tglMatrix = unique;
    tglMatrix.preTranslate(tinygl::Vec3{1.0f, 2.0f, 3.0f});

    auto translationMatrix = glm::mat4(1.0f);
    translationMatrix = glm::translate(translationMatrix, glm::vec3(1.0f, 2.0f, 3.0f));
    auto glmMatrix = translationMatrix * glm::make_mat4(unique.data());

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 translation benchmark", "[Mat4]")
{
    BENCHMARK("preTranslate") {
        auto m = unique;
        m.preTranslate(tinygl::Vec3{1.0f, 2.0f, 3.0f});
        return m;
    };

    BENCHMARK("preTranslate by matrix multiplication") {
        auto t = tinygl::Mat4::translation(tinygl::Vec3{1.0f, 2.0f, 3.0f});
        auto m = unique;
        m = t * m;
        return m;
    };
}

TEST_CASE("Mat4 postTranslate", "[Mat4]")
{
    auto tglMatrix = tinygl::Mat4{};
    tglMatrix.postTranslate(tinygl::Vec3{1.0f, 2.0f, 3.0f});

    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::translate(glmMatrix, glm::vec3(1.0f, 2.0f, 3.0f));

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 preRotate around x-axis", "[Mat4]")
{
    auto tglMatrix = unique;
    tglMatrix.preRotate(45.0f, {1.0f, 0.0f, 0.0f});

    auto rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, tinygl::degreesToRadians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    auto glmMatrix = rotationMatrix * glm::make_mat4(unique.data());

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 postRotate around x-axis", "[Mat4]")
{
    auto tglMatrix = tinygl::Mat4{};
    tglMatrix.postRotate(45.0f, tinygl::Vec3{1.0f, 0.0f, 0.0f});

    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::rotate(glmMatrix, tinygl::degreesToRadians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 preRotate around y-axis", "[Mat4]")
{
    auto tglMatrix = unique;
    tglMatrix.preRotate(45.0f, {0.0f, 1.0f, 0.0f});

    auto rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, tinygl::degreesToRadians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    auto glmMatrix = rotationMatrix * glm::make_mat4(unique.data());

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 postRotate around y-axis", "[Mat4]")
{
    auto tglMatrix = tinygl::Mat4{};
    tglMatrix.postRotate(45.0f, {0.0f, 1.0f, 0.0f});

    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::rotate(glmMatrix, tinygl::degreesToRadians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 preRotate around z-axis", "[Mat4]")
{
    auto tglMatrix = unique;
    tglMatrix.preRotate(45.0f, {0.0f, 0.0f, 1.0f});

    auto rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, tinygl::degreesToRadians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    auto glmMatrix = rotationMatrix * glm::make_mat4(unique.data());

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 postRotate around z-axis", "[Mat4]")
{
    auto tglMatrix = tinygl::Mat4{};
    tglMatrix.postRotate(45.0f, {0.0f, 0.0f, 1.0f});

    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::rotate(glmMatrix, tinygl::degreesToRadians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("operator*=", "[Mat4]")
{
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
    tglMatrix.postTranslate({1.0f, 2.0f, 3.0f});

    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::translate(glmMatrix, glm::vec3(1.0f, 2.0f, 3.0f));

    auto* tglData = tglMatrix.data();
    auto* glmData = glm::value_ptr(glmMatrix);

    for (std::size_t i = 0; i < 4*4; ++i) {
        REQUIRE(tglData[i] == Catch::Approx(glmData[i]));
    }
}

TEST_CASE("Mat4 perspective", "[Mat4]")
{
    auto tglMatrix = tinygl::Mat4::perspective(60.0f, 1.0f, 0.1f, 1000.0f);
    auto glmMatrix = glm::perspective(tinygl::degreesToRadians(60.0f), 1.0f, 0.1f, 1000.0f);
    compare(tglMatrix, glmMatrix);
}

int main(int argc, const char* argv[])
{
    return Catch::Session().run(argc, argv);
}

