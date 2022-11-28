#ifndef TINYGL_WINDOW_H
#define TINYGL_WINDOW_H

#include "tinygl/input.h"
#include "tinygl/keyboard.h"
#include "tinygl/mouse.h"
#include <functional>
#include <memory>
#include <string>

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
        std::tuple<double, double> getCursorPos();
        std::tuple<int, int> getWindowSize();

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

#endif // TINYGL_WINDOW_H
