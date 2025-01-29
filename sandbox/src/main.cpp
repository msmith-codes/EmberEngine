#include <SandboxApp.hpp>
#include <EmberEngine/EmberEngine.hpp>

int main(int argc, char* argv[])
{
    EmberEngine::EmberApp& app = SandboxApp::getInstance();
    app.run();

    return 0;
}