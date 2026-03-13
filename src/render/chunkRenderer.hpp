#ifndef CHUNK_RENDERER_HPP
#define CHUNK_RENDERER_HPP

#include <util/loc.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <render/renderable.hpp>
#include <shaders/shader.h>
#include <render/textures.hpp>
#include <map>
#include <world/world.hpp>
#include <util/macros.hpp>
#include <util/loc.h>

class Renderer;

class RenderMesh{
    public:
    unsigned int VAO;
    unsigned int VBO;
    GLsizei nrVertices;
};

class ChunkRenderer{
public:
    ChunkRenderer(Renderer& renderer);
    bool init();
    void renderChunks(World& world, glm::mat4& view, glm::mat4& projection);

private:
    Renderer& renderer;
// Chatgpt suggested using a struct for the data of the vbo.

    const float& projectionDistance;
    const float& textureMargin;
    const int& maxNewMeshesPerFrame;

    struct SolidVBOElt{
        glm::vec3 pos;
        glm::vec3 uvl;
        // int textureLayer;
        uint8_t tint[4];
    };

    struct CutoutVBOElt{
        glm::vec3 pos;
        glm::vec3 uvl;
        // int textureLayer;
        uint8_t tint[4];
    };

    struct TranslucentVBOElt{
        glm::vec3 pos;
        glm::vec3 uvl;
        // int textureLayer;
        uint8_t tint[4];
    };

    Shader solidChunkShaderProgram;
    unsigned int viewLocChunksSolid;
    unsigned int projectionLocChunksSolid;

    Shader cutoutChunkShaderProgram;
    unsigned int viewLocChunksCutout;
    unsigned int projectionLocChunksCutout;

    Shader translucentChunkShaderProgram;
    unsigned int viewLocChunksTranslucent;
    unsigned int projectionLocChunksTranslucent;

    std::map<ChunkID,RenderMesh> solidMeshes; // Considering changing this to a map that contains all meshes.
    std::map<ChunkID,RenderMesh> cutoutMeshes;
    std::map<ChunkID,RenderMesh> translucentMeshes;
    std::vector<SolidVBOElt> updateSolidVBOVector(const RenderableChunkMesh& worldMesh);
    std::vector<CutoutVBOElt> updateCutoutVBOVector(const RenderableChunkMesh& worldMesh);
    std::vector<TranslucentVBOElt> updateTranslucentVBOVector(const RenderableChunkMesh& worldMesh);
    void updateRenderMesh(const ChunkID& chunkID, RenderableChunkMesh& worldMesh);
    void createRenderMesh(const ChunkID& chunkID, RenderableChunkMesh& worldMesh);
};



#endif //CHUNK_RENDERER_HPP