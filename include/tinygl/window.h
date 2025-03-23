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
    class window
    {
    public:
        typedef std::function<void(
            tinygl::keyboard::key, int, tinygl::input::action, tinygl::input::modifier)> key_callback;
        typedef std::function<void(
            tinygl::mouse::button, tinygl::input::action, tinygl::input::modifier)> mouse_button_callback;

        window(int width, int height, std::string_view title, bool vsync = false);
        virtual ~window();

        window(window&& other) noexcept;
        window& operator=(window&& other) noexcept;

        void run();

        tinygl::keyboard::key_state get_key(tinygl::keyboard::key key);

        template<std::floating_point T>
        std::tuple<T, T> get_cursor_pos();

        std::tuple<int, int> get_window_size();
        std::tuple<int, int> get_framebuffer_size();

        template<std::floating_point T>
        T aspect_ratio();

        void set_title(std::string_view title);

        void set_should_close(bool should_close);

        void set_key_callback(key_callback callback);
        void set_mouse_button_callback(mouse_button_callback callback);

        float delta_time() const;

    protected:
        virtual void init() {}
        virtual void process_input() {}
        virtual void draw() {}
        virtual void draw_ui() {}

    private:
        struct window_private;
        std::unique_ptr<window_private> p;
    };
}

extern template std::tuple<float, float> tinygl::window::get_cursor_pos<float>();
extern template std::tuple<double, double> tinygl::window::get_cursor_pos<double>();
extern template std::tuple<long double, long double> tinygl::window::get_cursor_pos<long double>();

extern template float tinygl::window::aspect_ratio<float>();
extern template double tinygl::window::aspect_ratio<double>();
extern template long double tinygl::window::aspect_ratio<long double>();

#endif // TINYGL_WINDOW_H
