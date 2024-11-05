#ifndef EMBER_Window_HPP_
#define EMBER_Window_HPP_

// -- STD -- //
#include <string>

class GLFWwindow;

namespace Ember
{
    class Window
    {
        private: // Properties
            unsigned int width;
            unsigned int height;
            std::string title;
            bool fullscreen;
            bool resizable;
            bool vsync;
            unsigned int targetFPS;
        private: // Utility Variables
            GLFWwindow* window;
        public: // Constructor & Destructor
            Window();
            ~Window();
        public: // Setters
            void setScreenSize(unsigned int width, unsigned int height);
            void setTitle(const std::string& title);
            void setFullscreen(bool fullscreen);
            void setResizable(bool resizable);
            void setVsync(bool vsync);
            void setTargetFPS(unsigned int targetFPS);
        public: // Getters
            unsigned int getWidth() const;
            unsigned int getHeight() const;
            const std::string& getTitle() const;
            bool isFullscreen() const;
            bool isResizable() const;
            bool isVsync() const;
            unsigned int getTargetFPS() const;
            unsigned int getFPS() const;
        public: // Methods
            void init();
            bool shouldWindowClose();
            void swapBuffers();
            void pollEvents();
            void destroy();

    };
}

#endif