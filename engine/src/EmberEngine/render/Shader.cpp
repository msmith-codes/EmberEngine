#include <EmberEngine/render/Shader.hpp>

#include <glad/glad.h>

#include <iostream>

void checkGLError(const std::string& location) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error at " << location << ": " << err << std::endl;
    }
}

namespace EmberEngine
{
    Shader::Shader()
    {
        this->program = glCreateProgram();

        if(this->program == 0) {
            std::cerr << "Shader::Shader() - Failed to create shader program!" << std::endl;
            std::exit(1);
        }
    }

    void Shader::addVertexShader(const std::string& text)
    {
        this->addProgram(text, GL_VERTEX_SHADER);
    }

    void Shader::addGeometryShader(const std::string& text)
    {
        this->addProgram(text, GL_GEOMETRY_SHADER);
    }

    void Shader::addFragmentShader(const std::string& text)
    {
        this->addProgram(text, GL_FRAGMENT_SHADER);
    }

    void Shader::bind()
    {
        glUseProgram(this->program);
    }

    void Shader::unbind()
    {
        glUseProgram(0);
    }

    void Shader::compileShader()
    {
        glLinkProgram(this->program);

        int success;
        glGetProgramiv(this->program, GL_LINK_STATUS, &success);

        if(!success) {
            char infoLog[512];
            glGetProgramInfoLog(this->program, 512, nullptr, infoLog);
            std::cerr << "Shader::compileShader() - Failed to link shader program: " << infoLog << std::endl;
            std::exit(1);
        }

        glValidateProgram(this->program);

        glGetProgramiv(this->program, GL_VALIDATE_STATUS, &success);

        if(!success) {
            char infoLog[512];
            glGetProgramInfoLog(this->program, 512, nullptr, infoLog);
            std::cerr << "Shader::compileShader() - Failed to validate shader program: " << infoLog << std::endl;
            std::exit(1);
        }

        std::cout << "Shader::compileShader() - Successfully compiled and linked shader program!" << std::endl;
    }

    void Shader::addProgram(const std::string& text, int type)
    {
        unsigned int shader = glCreateShader(type);

        if(shader == 0) {
            std::cerr << "Shader::addProgram() - Failed to create shader!" << std::endl;
            std::exit(1);
        }

        const char* shaderText = text.c_str();
        glShaderSource(shader, 1, &shaderText, nullptr);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if(!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "Shader::addProgram() - Failed to compile shader: " << infoLog << std::endl;
            std::exit(1);
        }

        glAttachShader(this->program, shader);
    }
}