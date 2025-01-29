#ifndef EE_Shader_HPP
#define EE_Shader_HPP

#include <string>

namespace EmberEngine
{
    class Shader
    {
        private:
            unsigned int program;
        public:
            Shader();
        public:
            void addVertexShader(const std::string& text);
            void addGeometryShader(const std::string& text);
            void addFragmentShader(const std::string& text);
        public:
            void bind();
            void unbind();
            void compileShader();
        private:
            void addProgram(const std::string& text, int type);
    };
}

#endif