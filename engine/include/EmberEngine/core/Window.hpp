#ifndef EE_Window_HPP
#define EE_Window_HPP

#include <string>

class GLFWwindow;

namespace EmberEngine
{
    class Window
    {
        private: // -- Properties -- //
            int width;
            int height;
            std::string title;
            bool resizable;
        private: // -- Utilities -- //
            GLFWwindow* window;
        public:
            static Window& getInstance();
        private:
            Window();
            Window(const Window&) = delete;
            Window& operator=(const Window&) = delete;
        public: //  -- Getters -- //
            static int getWidth();
            static int getHeight();
            static std::string getTitle();
            static bool getResizable();
        public: // -- Setters -- //
            static void setSize(const int width, const int height);
            static void setTitle(const std::string& title);
            static void setResizable(const bool resizable);
        public: // -- Static Methods -- // 
            static void compose();
            static void dispose();
            static void swapBuffers();
            static void pollEvents();
            static bool isOpen();

    };
}

#endif 