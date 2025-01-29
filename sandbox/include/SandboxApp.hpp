#ifndef SB_SandboxApp_HPP
#define SB_SandboxApp_HPP

#include <EmberEngine/EmberEngine.hpp>

class SandboxApp : public EmberEngine::EmberApp
{
    public:
        static SandboxApp& getInstance();
    private:
        EmberEngine::Mesh* mesh;
        EmberEngine::BasicShader* shader;
    private:
        SandboxApp();
    protected:
        void onCompose() override;
        void onUpdate(float dt) override;
        void onRender() override;
        void onDispose() override;      
};

#endif