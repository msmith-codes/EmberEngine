#ifndef EE_AssetLoader_HPP
#define EE_AssetLoader_HPP

#include <string>

namespace EmberEngine
{
    class AssetLoader
    {
        public:
            static std::string loadShader(const std::string& path);
    };
}

#endif