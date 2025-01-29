#ifndef EE_Vertex_HPP
#define EE_Vertex_HPP

#include <EmberEngine/math/Vec3f.hpp>

namespace EmberEngine
{
    struct Vertex
    {
        Vec3f position;
        static const int SIZE = 3;
    };
}

#endif