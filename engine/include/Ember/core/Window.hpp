#ifndef EE_Window_HPP
#define EE_Window_HPP

#include <string>

struct GLFWwindow;

namespace Ember
{
    class Window
    {
        private: // -- Window Utilities -- //
            GLFWwindow* window = nullptr;
        private: // -- Window Properties -- // 
            unsigned int width;
            unsigned int height;
            std::string title;
            bool vsync;
        public: // -- Singleton Accessor -- //
            static Window& GetInstance();
        private: // -- Singleton Guards -- //
            Window(const Window&) = delete;
            Window& operator=(const Window&) = delete;
        private: // -- Constructor -- //
            Window();
        public: // -- Destructor -- //
            ~Window();
        public: // -- Static Methods -- //
            static void Create(unsigned int width, unsigned int height, const std::string& title);
            static void Shutdown();
            static void PollEvents();
            static void SwapBuffers();
            static void Clear();
            static bool ShouldClose();

    };
}

#endif