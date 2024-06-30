#ifndef MAIN_H
#define MAIN_H

#define MAIN(major, minor, width, height)          \
int main()                                         \
{                                                  \
    try {                                          \
        tinygl::init(major, minor);                \
        Window window(width, height, NAME, true);  \
        window.run();                              \
    } catch (const std::exception& e) {            \
        tinygl::terminate();                       \
        std::cerr << e.what() << std::endl;        \
        return EXIT_FAILURE;                       \
    }                                              \
    tinygl::terminate();                           \
    return EXIT_SUCCESS;                           \
}                                                  \

#endif // MAIN_H
