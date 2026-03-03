#include <render/textures.hpp>

unsigned int TextureAtlas::textureAtlasID = 0;
float TextureAtlas::textureSizeWidth = 0.0f;
float TextureAtlas::textureSizeHeight = 0.0f;
int TextureAtlas::atlasWidth = 0;
int TextureAtlas::atlasHeight = 0;
// The json contains a dictionary from the entity names to the locations on the texture atlas.
std::ifstream f("assets/blockAtlasJson.json");
nlohmann::json TextureAtlas::jsonAtlasData = nlohmann::json::parse(f);

glm::vec2 jsonGet(const nlohmann::json& data, const std::string& textureName, int atlasWidth, int atlasHeight){
    float xLoc = ((float)data["frames"][textureName]["frame"]["x"])/((float)atlasWidth);
    float yLoc = ((float)data["frames"][textureName]["frame"]["y"])/((float)atlasHeight); 
    return {xLoc, yLoc};
}

glm::vec2 TextureAtlas::getUVCoord(const BlockID& blockID,const FaceType& faceType){
    return jsonGet(jsonAtlasData, BlockRegistry::getTextureName(blockID,faceType), atlasWidth, atlasHeight);
}

float TextureAtlas::getTextureSizeHeight(){
    return textureSizeHeight;

}
float TextureAtlas::getTextureSizeWidth(){
    return textureSizeWidth;
}


void TextureAtlas::bind(){
    // glActiveTexture(GL_TEXTURE0); // Optional if I want to have multiple textureAtlases simultaneously.
    glBindTexture(GL_TEXTURE_2D, textureAtlasID);  

}

bool TextureAtlas::setup(){

    // Setting up the texture atlas
    glGenTextures(1, &textureAtlasID);  
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureAtlasID);  

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int  nrChannels;
    unsigned char *data = stbi_load("assets/blockAtlas.png", &atlasWidth, &atlasHeight, &nrChannels, 0); 
    std::cout << "atlasWidth: " << atlasWidth << ", atlasHeight: " << atlasHeight << std::endl;

    // The atlasWidth and atlasHeight give the number of pixels in the atlas.
    // textureSizeWidth gives the portion of the image that belongs to a single texture.
    textureSizeWidth = (float)16/(float)atlasWidth;
    textureSizeHeight = (float)16/(float)atlasHeight;


    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
        return false;
    }
    stbi_image_free(data);



    return true;
}