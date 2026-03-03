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

struct TextureLoc{
    int x;
    int y;
    int width;
    int height;
};

class TextureAtlas{
public:
    static bool setup();
    static void bind();
    static glm::vec2 getUVCoord(const BlockID& blockID, const FaceType& faceType);
    static float getTextureSizeHeight();
    static float getTextureSizeWidth();

private:
    TextureAtlas() = delete;
    static nlohmann::json jsonAtlasData;
    static int atlasWidth;
    static int atlasHeight;
    static float textureSizeHeight;
    static float textureSizeWidth;
    static unsigned int textureAtlasID;
};


// class HotbarTexture{
// };



#endif //TEXTURES_HPP