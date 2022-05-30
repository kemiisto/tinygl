#ifndef TINYGL_WINDOW_H
#define TINYGL_WINDOW_H

#include "tinygl/keyboard.h"
#include <memory>
#include <string>

namespace tinygl
{
    class Window
    {
    public:
        Window(int width, int height, const std::string& title);
        virtual ~Window();

        Window(Window&& other) noexcept;
        Window& operator=(Window&& other) noexcept;

        void run();

        tinygl::KeyState getKey(tinygl::Key key);

        void setShouldClose(bool shouldClose);

    protected:
        virtual void init() {};
        virtual void processInput() {};
        virtual void draw() {};

    private:
        struct WindowPrivate;
        std::unique_ptr<WindowPrivate> p;
    };
}

#endif // TINYGL_WINDOW_H
