#include <EmberEngine/input/KeyInput.hpp>

#include <GLFW/glfw3.h>

namespace EmberEngine
{
    KeyInput& KeyInput::getInstance()
    {
        static KeyInput instance;
        return instance;
    }

    KeyInput::KeyInput()
    {
        for(int i = 0; i < 350; i++) {
            this->keys[i] = false;
        }
    }

    void KeyInput::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        KeyInput& keyInput = KeyInput::getInstance();
        if(action == GLFW_PRESS) {
            keyInput.keys[key] = true;
        } else if(action == GLFW_RELEASE) {
            keyInput.keys[key] = false;
        }
    }

    bool KeyInput::isKeyPressed(Key key)
    {
        return KeyInput::getInstance().keys[static_cast<int>(key)];
    }

    bool KeyInput::isKeyJustPressed(Key key)
    {
        KeyInput& keyInput = KeyInput::getInstance();
        if(keyInput.keys[static_cast<int>(key)]) {
            keyInput.keys[static_cast<int>(key)] = false;
            return true;
        }
        return false;
    }
}