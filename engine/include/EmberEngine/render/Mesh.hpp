#ifndef EE_Mesh_HPP
#define EE_Mesh_HPP

#include <vector>

namespace EmberEngine
{
    class Vertex;
    class Mesh
    {
        private:
            unsigned int vao;
            unsigned int vbo;
            unsigned int size;
        public:
            Mesh();
            Mesh(const Mesh& other);
            Mesh& operator=(const Mesh& other);
        public:
            void addVertices(std::vector<Vertex> vertices, unsigned int size);
            void printBufferContents();
            void render();
    };
};

#endif