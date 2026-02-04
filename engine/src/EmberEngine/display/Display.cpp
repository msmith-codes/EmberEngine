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
        InitWindow(width, height, title.c_str());
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

    void Display::set_size(unsigned int width, unsigned int height)
    {
        Display& self = Display::get_singleton();
    }

    void Display::set_target_fps(unsigned int target_fps)
    {
        Display& self = Display::get_singleton();
        self.target_fps = target_fps;

        if(IsWindowReady()) {
            SetTargetFPS(self.target_fps);
        }
    }
}