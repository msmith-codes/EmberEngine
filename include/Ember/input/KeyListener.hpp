#ifndef EMBER_KeyListener_HPP_
#define EMBER_KeyListener_HPP_

#include <vector>

class GLFWwindow;

namespace Ember
{
    class KeyListener
    {
        private:
            std::vector<bool> keys;
        private: // Constructor
            KeyListener();
        public: // Singleton Instance
            static KeyListener& getInstance();
        public: // Callbacks
            static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        public: // Getters
            std::vector<bool> getKeys() const;
            
    };
}

#endif