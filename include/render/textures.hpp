#ifndef TEXTURES_HPP
#define TEXTURES_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <json.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <blockRegistry.hpp>
#include <iostream>
#include <fstream>


class TextureAtlas{
public:
    TextureAtlas();
    void bind();
    bool setup();
    glm::vec2 getUVCoord(const BlockID& blockID, const FaceType& faceType) const;
    float getTextureSizeHeight() const;
    float getTextureSizeWidth() const;

private:
    nlohmann::json jsonAtlasData;
    int atlasWidth;
    int atlasHeight;
    float textureSizeHeight;
    float textureSizeWidth;
    unsigned int textureAtlasID;
};







#endif //TEXTURES_HPP