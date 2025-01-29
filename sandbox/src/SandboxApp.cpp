#include <SandboxApp.hpp>

#include <EmberEngine/EmberEngine.hpp>

#include <iostream>
#include <vector>

#include <glad/glad.h>

using namespace EmberEngine;

// void checkGLError(const std::string& location) {
//     GLenum err;
//     while ((err = glGetError()) != GL_NO_ERROR) {
//         std::cerr << "OpenGL error at " << location << ": " << err << std::endl;
//     }
// }

SandboxApp& SandboxApp::getInstance()
{
    static SandboxApp instance;
    return instance;
}

SandboxApp::SandboxApp()
{
    // Setup the Window:
    Window::setSize(800, 600);
    Window::setTitle("SandboxApp");
    Window::setResizable(true);
}

void SandboxApp::onCompose()
{
    this->mesh = new Mesh();
    this->shader = new BasicShader();

    std::vector<Vertex> vertices = {
        Vertex{Vec3f(-1.0f, -1.0f, 0.0f)},
        Vertex{Vec3f( 1.0f, -1.0f, 0.0f)},
        Vertex{Vec3f( 0.0f,  1.0f, 0.0f)}
    };

    this->mesh->addVertices(vertices, vertices.size());
}

void SandboxApp::onUpdate(float dt)
{
    if(KeyInput::isKeyJustPressed(Key::KEY_SPACE)) {
        std::cout << "Space key was pressed!" << std::endl;
    }    

    if(MouseInput::isMouseButtonJustPressed(MouseButton::LEFT)) {
        std::cout << "Mouse clicked at " << MouseInput::getX() << ", " << MouseInput::getY() << std::endl;
    }
}

void SandboxApp::onRender()
{
    RenderUtils::clearScreen();

    this->shader->bind();
        this->mesh->render();
    this->shader->unbind();
}

void SandboxApp::onDispose()
{
    Window::dispose();
}