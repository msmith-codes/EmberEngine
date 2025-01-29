#ifndef EE_MouseInput_HPP
#define EE_MouseInput_HPP

class GLFWwindow;

namespace EmberEngine
{
    enum class MouseButton
    {
        LEFT = 0,
        RIGHT = 1,
        MIDDLE = 2
    };

    class MouseInput
    {
        private: 
            double scrollX;
            double scrollY;
            double xPos;
            double yPos;
            double lastX;
            double lastY;
            bool mouseButtonPressed[3];
            bool dragging;
        public:
            static MouseInput& getInstance();
        private:
            MouseInput();
            MouseInput(const MouseInput&) = delete;
            MouseInput& operator=(const MouseInput&) = delete;
        public: 
            static void mousePosCallback(GLFWwindow* window, double xPos, double yPos);
            static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
            static void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
        public:
            static void endFrame();
        public: // -- Getters -- //
            static float getX();
            static float getY();
            static float getDx();
            static float getDy();
            static float getScrollX();
            static float getScrollY();
            static bool isDragging();
        public:
            static bool isMouseButtonPressed(MouseButton button);
            static bool isMouseButtonJustPressed(MouseButton button);

    };
}

#endif