#ifndef TINYGL_WINDOW_H
#define TINYGL_WINDOW_H

#include "tinygl/input.h"
#include "tinygl/keyboard.h"
#include "tinygl/mouse.h"
#include <functional>
#include <memory>
#include <string>
#include <concepts>

namespace tinygl
{
    class Window
    {
    public:
        typedef std::function<void(
            tinygl::keyboard::Key, int, tinygl::input::Action, tinygl::input::Modifier)> KeyCallback;
        typedef std::function<void(
            tinygl::mouse::Button, tinygl::input::Action, tinygl::input::Modifier)> MouseButtonCallback;

        Window(int width, int height, const std::string& title, bool vsync = false);
        virtual ~Window();

        Window(Window&& other) noexcept;
        Window& operator=(Window&& other) noexcept;

        void run();

        tinygl::keyboard::KeyState getKey(tinygl::keyboard::Key key);

        template<std::floating_point T>
        std::tuple<T, T> getCursorPos();

        std::tuple<int, int> getWindowSize();
        template<std::floating_point T>
        T aspectRatio();

        void setTitle(const std::string& title);

        void setShouldClose(bool shouldClose);

        void setKeyCallback(KeyCallback callback);
        void setMouseButtonCallback(MouseButtonCallback callback);

        float deltaTime() const;

    protected:
        virtual void init() {};
        virtual void processInput() {};
        virtual void draw() {};
        virtual void drawUi() {};

    private:
        struct WindowPrivate;
        std::unique_ptr<WindowPrivate> p;
    };
}

extern template std::tuple<float, float> tinygl::Window::getCursorPos<float>();
extern template std::tuple<double, double> tinygl::Window::getCursorPos<double>();
extern template std::tuple<long double, long double> tinygl::Window::getCursorPos<long double>();

extern template float tinygl::Window::aspectRatio<float>();
extern template double tinygl::Window::aspectRatio<double>();
extern template long double tinygl::Window::aspectRatio<long double>();

#endif // TINYGL_WINDOW_H
