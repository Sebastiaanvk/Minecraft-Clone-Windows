#include <render/textures.hpp>

unsigned int TextureAtlas::textureArrayID = 0;
// float TextureAtlas::textureSizeWidth;
// float TextureAtlas::textureSizeHeight;
// int TextureAtlas::atlasWidth;
// int TextureAtlas::atlasHeight;
// The json contains a dictionary from the entity names to the locations on the texture atlas.
std::ifstream f("assets/blockAtlasJson.json");
nlohmann::json TextureAtlas::jsonAtlasData = nlohmann::json::parse(f);

glm::vec2 jsonGet(const nlohmann::json& data, const std::string& textureName, int atlasWidth, int atlasHeight){
    float xLoc = ((float)data["frames"][textureName]["frame"]["x"])/((float)atlasWidth);
    float yLoc = ((float)data["frames"][textureName]["frame"]["y"])/((float)atlasHeight); 
    return {xLoc, yLoc};
}

// glm::vec2 TextureAtlas::getUVCoord(const BlockID& blockID,const FaceType& faceType){
//     return jsonGet(jsonAtlasData, BlockRegistry::getTextureName(blockID,faceType), atlasWidth, atlasHeight);
// }

// float TextureAtlas::getTextureSizeHeight(){
//     return textureSizeHeight;

// }
// float TextureAtlas::getTextureSizeWidth(){
//     return textureSizeWidth;
// }

// glm::vec2 TextureAtlas::getTextureSize(){
//     return glm::vec2(textureSizeWidth,textureSizeHeight);
// }


// void TextureAtlas::bind(){
//     // glActiveTexture(GL_TEXTURE0); // Optional if I want to have multiple textureAtlases simultaneously.
//     glBindTexture(GL_TEXTURE_2D, textureArrayID);  
// }

bool TextureAtlas::setup(){

    // Setting up the texture atlas
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textureArrayID);  
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayID);  

// Log(1)

    int  nrChannels, atlasWidth, atlasHeight;
    unsigned char *data = stbi_load("assets/blockAtlas.png", &atlasWidth, &atlasHeight, &nrChannels, 0); 
    // Log(1.2)
    if(!data){
        std::cout << "Failed to load texture" << std::endl;
        return false;
    }
    // Log(1.3)
    GLsizei mipLevelCount = 4; //We want 5 mipmap layers because the textures are 16x16, so 16,8,4,2,1.
    // Log(1.4)
    // GLenum err = glGetError();
    // std::cout << "GL error before glTexStorage3D: " << err << std::endl;
    // std::cout<< BlockRegistry::nrTextureIndices() << std::endl;
    // std::cout << "textureArrayID: " << textureArrayID << std::endl;
    // std::cout << glGetString(GL_VERSION) << std::endl;

    // glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevelCount, GL_RGBA8, 16, 16, BlockRegistry::nrTextureIndices());
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, 16, 16, BlockRegistry::nrTextureIndices(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_SRGB8_ALPHA8, 16, 16, BlockRegistry::nrTextureIndices(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    for(int i=0; i<BlockRegistry::nrTextureIndices(); i++){
        std::string textureName = BlockRegistry::indexToTextureName(i);
        unsigned char texture[16*16*4];
        int starting_x = jsonAtlasData["frames"][textureName]["frame"]["x"];
        int starting_y = jsonAtlasData["frames"][textureName]["frame"]["y"];
        int textureIndex = 0;
        for(int dy=0;dy<16;dy++){
            for(int dx=0; dx<16; dx++){
                for(int ch=0; ch<4; ch++){
                    texture[textureIndex] = data[(dy+starting_y)*atlasWidth*4 + (starting_x+dx)*4+ch]; 
                    textureIndex++;
                }
            }
        }
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, 16, 16, 1, GL_RGBA, GL_UNSIGNED_BYTE, texture);
    }
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

// Log(3)
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // ignore the mipmaps.
    // glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // ignore the mipmaps.

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // this one looks the nicest.
    // glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); 
    // glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); 
    // glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR); 

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // THIS IS EXTREMELY  CURSED!!!!!!!

    // // Limit how far down the mip chain it goes
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 4); 
    
    // std::cout << glGetString(GL_VERSION) << std::endl;
    #ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
    #define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
    #endif
    #ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
    #define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
    #endif


    float maxAniso = 0.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
    std::cout << "Max anisotropy: " << maxAniso << std::endl;
    glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

    // float maxAniso = 0.0f;
    // glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAniso);
    // std::cout << "Max anisotropy: " << maxAniso << std::endl;
    // glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY, maxAniso);

// Log(3)
    stbi_image_free(data);

    return true;
}