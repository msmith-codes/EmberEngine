#include <EmberEngine/input/MouseInput.hpp>

#include <GLFW/glfw3.h>

namespace EmberEngine
{
    MouseInput& MouseInput::getInstance()
    {
        static MouseInput instance;
        return instance;
    }

    MouseInput::MouseInput()
    {
        this->scrollX = 0.0;
        this->scrollY = 0.0;
        this->xPos = 0.0;
        this->yPos = 0.0;
        this->lastX = 0.0;
        this->lastY = 0.0;
        this->dragging = false;
        for(int i = 0; i < 3; i++) {
            this->mouseButtonPressed[i] = false;
        }
    }

    void MouseInput::mousePosCallback(GLFWwindow* window, double xPos, double yPos)
    {
        MouseInput& mouse = MouseInput::getInstance();
        mouse.lastX = mouse.xPos;
        mouse.lastY = mouse.yPos;
        mouse.xPos = xPos;
        mouse.yPos = yPos;
        mouse.dragging = mouse.mouseButtonPressed[GLFW_MOUSE_BUTTON_LEFT] || mouse.mouseButtonPressed[GLFW_MOUSE_BUTTON_RIGHT] || mouse.mouseButtonPressed[GLFW_MOUSE_BUTTON_MIDDLE];
    }

    void MouseInput::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        MouseInput& mouse = MouseInput::getInstance();
        if(action == GLFW_PRESS) {
            if(button >= 0 && button < 3) {
                mouse.mouseButtonPressed[button] = true;
            }
        } else if(action == GLFW_RELEASE) {
            mouse.mouseButtonPressed[button] = false;
        }
    }

    void MouseInput::mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
    {
        MouseInput& mouse = MouseInput::getInstance();
        mouse.scrollX = xOffset;
        mouse.scrollY = yOffset;
    }

    void MouseInput::endFrame()
    {
        MouseInput& mouse = MouseInput::getInstance();
        mouse.scrollX = 0.0;
        mouse.scrollY = 0.0;
        mouse.lastX = mouse.xPos;
        mouse.lastY = mouse.yPos;
    }

    // -- Getters -- //
    float MouseInput::getX()
    {
        return MouseInput::getInstance().xPos;
    }

    float MouseInput::getY()
    {
        return MouseInput::getInstance().yPos;
    }

    float MouseInput::getDx()
    {
        return MouseInput::getInstance().xPos - MouseInput::getInstance().lastX;
    }

    float MouseInput::getDy()
    {
        return MouseInput::getInstance().yPos - MouseInput::getInstance().lastY;
    }

    float MouseInput::getScrollX()
    {
        return MouseInput::getInstance().scrollX;
    }

    float MouseInput::getScrollY()
    {
        return MouseInput::getInstance().scrollY;
    }

    bool MouseInput::isDragging()
    {
        return MouseInput::getInstance().dragging;
    }
    
    bool MouseInput::isMouseButtonPressed(MouseButton button)
    {
        int iButton = static_cast<int>(button);
        if(iButton < 0 || iButton >= 3) {
            return false;
        }
        return MouseInput::getInstance().mouseButtonPressed[iButton];
    }

    bool MouseInput::isMouseButtonJustPressed(MouseButton button)
    {
        int iButton = static_cast<int>(button);
        MouseInput& mouse = MouseInput::getInstance();
        if(iButton < 0 || iButton >= 3) {
            return false;
        }
        if(mouse.mouseButtonPressed[iButton]) {
            mouse.mouseButtonPressed[iButton] = false;
            return true;
        }
        return false;
    }

}