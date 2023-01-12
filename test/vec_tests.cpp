#include "tinygl/vec.h"
#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>
#define GLM_FORCE_SIZE_T_LENGTH
#include <glm/glm.hpp>

constexpr auto glmVecA = glm::vec4{1.0f, 2.0f, 3.0f, 4.0f};
constexpr auto glmVecB = glm::vec4{4.0f, 5.0f, 6.0f, 7.0f};

constexpr auto tglVecA = tinygl::Vec4{1.0f, 2.0f, 3.0f, 4.0f};
constexpr auto tglVecB = tinygl::Vec4{4.0f, 5.0f, 6.0f, 7.0f};

template<std::size_t N, typename T>
void compare(const tinygl::Vec<N,T>& tglVec, const glm::vec<N,T>& glmVec)
{
    for (std::size_t i = 0; i < N; ++i) {
        CAPTURE(i);
        REQUIRE(tglVec[i] == Catch::Approx(glmVec[i]));
    }
}

TEST_CASE("Vec2 size", "[Vec2]")
{
    REQUIRE(sizeof(tinygl::Vec2) == 2 * sizeof(float));
}

TEST_CASE("Vec2 is constructed from initializer list", "[Vec2]")
{
    tinygl::Vec2 tglVec {0.0f, 0.1f};
    glm::vec2 glmVec {0.0f, 0.1f};
    compare(tglVec, glmVec);
}

TEST_CASE("Vec3 is constructed from initializer list", "[Vec3]")
{
    tinygl::Vec3 tglVec {0.0f, 0.1f, 0.2f};
    glm::vec3 glmVec {0.0f, 0.1f, 0.2f};
    compare(tglVec, glmVec);
}

TEST_CASE("Vec4 is constructed from initializer list", "[Vec4]")
{
    tinygl::Vec4 tglVec {0.0f, 0.1f, 0.2f, 0.3f};
    glm::vec4 glmVec {0.0f, 0.1f, 0.2f, 0.3f};
    compare(tglVec, glmVec);
}

TEST_CASE("Vec4 component access", "[Vec4]")
{
    tinygl::Vec4 tglVec {0.0f, 0.1f, 0.2f, 0.3f};
    glm::vec4 glmVec {0.0f, 0.1f, 0.2f, 0.3f};
    REQUIRE(tglVec.x == Catch::Approx(glmVec.x));
    REQUIRE(tglVec.y == Catch::Approx(glmVec.y));
    REQUIRE(tglVec.z == Catch::Approx(glmVec.z));
    REQUIRE(tglVec.w == Catch::Approx(glmVec.w));
}

TEST_CASE("Vec4 data", "[Vec4]")
{
    tinygl::Vec4 tglVec {0.0f, 0.1f, 0.2f, 0.3f};
    float a[4] {0.0f, 0.1f, 0.2f, 0.3f};

    float* data = tglVec.data();
    for (std::size_t i = 0; i < 4; ++i) {
        REQUIRE(data[i] == Catch::Approx(a[i]));
    }
}

TEST_CASE("Vec4 operator+", "[Vec4]")
{
    auto tglVec = tglVecA + tglVecB;
    auto glmVec = glmVecA + glmVecB;
    compare(tglVec, glmVec);
}

TEST_CASE("Vec4 operator-", "[Vec4]")
{
    auto tglVec = tglVecA - tglVecB;
    auto glmVec = glmVecA - glmVecB;
    compare(tglVec, glmVec);
}

TEST_CASE("Vec4 operator*", "[Vec4]")
{
    {
        auto tglVec = 2.0f * tglVecB;
        auto glmVec = 2.0f * glmVecB;
        compare(tglVec, glmVec);
    }

    {
        auto tglVec = tglVecA * 2.0f;
        auto glmVec = glmVecA * 2.0f;
        compare(tglVec, glmVec);
    }

    {
        auto tglVec = tglVecA * tglVecB;
        auto glmVec = glmVecA * glmVecB;
        compare(tglVec, glmVec);
    }
}

TEST_CASE("Vec4 operator/", "[Vec4]")
{
    {
        auto tglVec = tglVecA / 2.0f;
        auto glmVec = glmVecA / 2.0f;
        compare(tglVec, glmVec);
    }

    {
        auto tglVec = tglVecA / tglVecB;
        auto glmVec = glmVecA / glmVecB;
        compare(tglVec, glmVec);
    }
}

TEST_CASE("Vec4 unary operator-", "[Vec4]")
{
    tinygl::Vec4 tglVec {0.0f, -0.1f, 0.2f, -0.3f};
    tglVec = -tglVec;

    glm::vec4 glmVec {0.0f, -0.1f, 0.2f, -0.3f};
    glmVec = -glmVec;

    compare(tglVec, glmVec);
}

int main(int argc, const char* argv[])
{
    return Catch::Session().run(argc, argv);
}
