#include <Ember/input/KeyListener.hpp>

#include <GLFW/glfw3.h>

namespace Ember
{
    // Constructor
    //-------------------------------------------------------------------------
    KeyListener::KeyListener()
    {
        this->keys.resize(512);
    }

    // Singleton Instance
    //-------------------------------------------------------------------------
    KeyListener& KeyListener::getInstance()
    {
        static KeyListener instance;
        return instance;
    }

    // Callbacks
    //-------------------------------------------------------------------------
    void KeyListener::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if(action == GLFW_PRESS) {
            KeyListener::getInstance().keys[key] = true;
        } else if(action == GLFW_RELEASE) {
            KeyListener::getInstance().keys[key] = false;
        }
    }

    // Getters
    //-------------------------------------------------------------------------
    std::vector<bool> KeyListener::getKeys() const
    {
        return this->keys;
    }
}