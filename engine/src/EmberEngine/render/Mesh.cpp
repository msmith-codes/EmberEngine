#include <EmberEngine/render/Mesh.hpp>
#include <EmberEngine/render/Vertex.hpp>

#include <glad/glad.h>

#include <iostream>

// void checkGLError(const std::string& location) {
//     GLenum err;
//     while ((err = glGetError()) != GL_NO_ERROR) {
//         std::cerr << "OpenGL error at " << location << ": " << err << std::endl;
//     }
// }

namespace EmberEngine
{
    Mesh::Mesh()
    {
        glGenVertexArrays(1, &this->vao);
        glGenBuffers(1, &this->vbo);
        this->size = 0;
    }

    Mesh::Mesh(const Mesh& other)
    {
        this->vbo = other.vbo;
        this->size = other.size;
    }

    Mesh& Mesh::operator=(const Mesh& other)
    {
        this->vbo = other.vbo;
        this->size = other.size;
        return *this;
    }

    void Mesh::addVertices(std::vector<Vertex> vertices, unsigned int size)
    {
        this->size = vertices.size();
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        // checkGLError("addVertices - glBindBuffer");

        std::vector<float> data;
        for(Vertex vertex : vertices) {
            Vec3f position = vertex.position;
            data.push_back(position.getX());
            data.push_back(position.getY());
            data.push_back(position.getZ());
        }

        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
        // checkGLError("addVertices - glBufferData");

        std::cout << "Mesh::addVertices() - Added " << size << " vertices to the mesh!" << std::endl;
        std::cout << "Mesh::addVertices() - VBO: " << this->vbo << std::endl;
    }

    void Mesh::printBufferContents()
    {
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        // checkGLError("printBufferContents - glBindBuffer");

        // Get the size of the buffer
        GLint bufferSize = 0;
        glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
        // checkGLError("printBufferContents - glGetBufferParameteriv");

        // Allocate memory to read the buffer contents
        std::vector<float> bufferData(bufferSize / sizeof(float));

        // Read the buffer contents
        glGetBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, bufferData.data());
        // checkGLError("printBufferContents - glGetBufferSubData");

        // Print the buffer contents
        std::cout << "Buffer contents: ";
        for (float value : bufferData) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    void Mesh::render()
    {
        glBindVertexArray(this->vao);

        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

        // Ensure the vertex attribute pointer is set up correctly
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

        glDrawArrays(GL_TRIANGLES, 0, this->size);

        glDisableVertexAttribArray(0);

        glBindVertexArray(0);
    }
}