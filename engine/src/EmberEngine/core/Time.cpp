#include <EmberEngine/core/Time.hpp>

#include <glfw/glfw3.h>

namespace EmberEngine
{
    double Time::getTime()
    {
        return glfwGetTime();
    }
}