#include <EmberEngine/utils/AssetLoader.hpp>

#include <fstream>

namespace EmberEngine
{
    std::string AssetLoader::loadShader(const std::string& path)
    {
        std::ifstream file(path);
        std::string shader;
        std::string line;

        if(file.is_open()) {
            while(getline(file, line)) {
                shader += line + "\n";
            }
            file.close();
        }

        return shader;
    }
}