#include <EmberEngine/render/shaders/BasicShader.hpp>

#include <sstream>
#include <string>

namespace EmberEngine
{
    BasicShader::BasicShader()
    {
        std::stringstream ss;

        std::string vertexShader;
        ss << "#version 410\n";
        ss << "layout(location = 0) in vec3 position;\n";
        ss << "out vec4 color;\n";
        ss << "void main() {\n";
        ss << "    color = vec4(clamp(position, 0.0, 1.0), 1.0);\n";
        ss << "    gl_Position = vec4(position, 1.0);\n";
        ss << "}\n";
        vertexShader = ss.str();

        ss.str("");

        std::string fragmentShader;
        ss << "#version 410\n";
        ss << "in vec4 color;\n";
        ss << "out vec4 fragColor;\n";
        ss << "void main() {\n";
        ss << "    fragColor = color;\n";
        ss << "}\n";
        fragmentShader = ss.str();

        ss.str("");

        this->addVertexShader(vertexShader);
        this->addFragmentShader(fragmentShader);
        this->compileShader();
    }
}