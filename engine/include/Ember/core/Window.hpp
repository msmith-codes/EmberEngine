#ifndef EE_Window_HPP
#define EE_Window_HPP

#include <string>

namespace Ember
{
    class Window
    {
        public:
            static Window& get_singleton();
        private:
            std::string title;
            unsigned int width;
            unsigned int height;
        private:
            Window();
        public:
            ~Window();
        public: // -- Static Methods -- //
            static void create(const std::string& title, unsigned int width, unsigned int height);
            static void shutdown();
            static bool shouldClose();
        public: // -- Static Getters -- //
            static std::string& getTitle();
            static unsigned int getWidth();
            static unsigned int getHeight();
        public: // -- Static Setters -- //
            static void setTitle(const std::string& title);
            static void setSize(unsigned int width, unsigned int height);


    };
}

#endif