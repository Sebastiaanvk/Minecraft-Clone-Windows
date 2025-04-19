#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <external/glm/vec3.hpp>
#include <string>


struct Renderable{
    int x,y,z;
    std::string topTexture;
    std::string sideTexture;
    std::string botTexture;
};
#endif // RENDERABLE_HPP