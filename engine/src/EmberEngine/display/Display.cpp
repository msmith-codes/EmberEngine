#include <EmberEngine/display/Display.hpp>

#include <raylib.h>

namespace EmberEngine
{
    Display& Display::get_singleton()
    {
        static Display singleton;
        return singleton;
    }

    void Display::create(unsigned int width, unsigned int height, const std::string& title)
    {
        Display& self = Display::get_singleton();
        self.width = width;
        self.height = height;
        self.title = title;

        int flags = 0;
        if(self.resizable) {
            flags |= FLAG_WINDOW_RESIZABLE;
        }

        SetConfigFlags(flags);

        InitWindow(self.width, self.height, self.title.c_str());
        SetExitKey(KEY_NULL);
    }

    void Display::destroy()
    {
        CloseWindow();
    }

    bool Display::should_close()
    {
        return WindowShouldClose();
    }

    void Display::set_resizable(bool resizable)
    {
        Display& self = Display::get_singleton();
        self.resizable = resizable;
    }

    void Display::set_size(unsigned int width, unsigned int height)
    {
        Display& self = Display::get_singleton();
        self.width = width;
        self.height = height;

        if(IsWindowReady()) {
            SetWindowSize(self.width, self.height);
        }
    }

    void Display::set_target_fps(unsigned int target_fps)
    {
        Display& self = Display::get_singleton();
        self.target_fps = target_fps;

        if(IsWindowReady()) {
            SetTargetFPS(self.target_fps);
        }
    }

    void Display::set_title(const std::string& title)
    {
        Display& self = Display::get_singleton();
        self.title = title;

        if(IsWindowReady()) {
            SetWindowTitle(self.title.c_str());
        }
    }
}