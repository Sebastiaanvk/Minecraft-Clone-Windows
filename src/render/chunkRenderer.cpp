#include <render/chunkRenderer.hpp>
#include <render/renderer.hpp>

ChunkRenderer::ChunkRenderer(Renderer& renderer) :
renderer(renderer),//projectionDistance(renderer.renderSettings.projectionDistance),
textureMargin(renderer.renderSettings.textureMargin),maxNewMeshesPerFrame(renderer.renderSettings.maxNewMeshesPerFrame),
maxTerrainRenders(renderer.renderSettings.maxTerrainRenders),maxWaterRenders(renderer.renderSettings.maxWaterRenders),
maxVegetationRenders(renderer.renderSettings.maxVegetationRenders),
ambientOcclusionFlag(renderer.renderSettings.ambientOcclusion),gammaCorrectionFlag(renderer.renderSettings.gammaCorrection) {
    
}

bool ChunkRenderer::init(){
//  Shaders:
// Msg(solidShader)
    solidChunkShaderProgram = Shader("src/shaders/chunkShaders/solidChunkShader.vs","src/shaders/chunkShaders/solidChunkShader.fs");
    solidChunkShaderProgram.use();
    solidChunkShaderProgram.setInt("textureAtlas",renderer.blockTextureSlotOffset);
    viewLocChunksSolid = glGetUniformLocation(solidChunkShaderProgram.ID, "view");
    projectionLocChunksSolid = glGetUniformLocation(solidChunkShaderProgram.ID, "projection");

// Msg(cutout Shader)
    cutoutChunkShaderProgram = Shader("src/shaders/chunkShaders/cutoutChunkShader.vs","src/shaders/chunkShaders/cutoutChunkShader.fs");
    cutoutChunkShaderProgram.use();
    cutoutChunkShaderProgram.setInt("textureAtlas",renderer.blockTextureSlotOffset);
    viewLocChunksCutout = glGetUniformLocation(cutoutChunkShaderProgram.ID, "view");
    projectionLocChunksCutout = glGetUniformLocation(cutoutChunkShaderProgram.ID, "projection");

// Msg(translucent Shader)
    translucentChunkShaderProgram = Shader("src/shaders/chunkShaders/translucentChunkShader.vs","src/shaders/chunkShaders/translucentChunkShader.fs");
    translucentChunkShaderProgram.use();
    translucentChunkShaderProgram.setInt("textureAtlas",renderer.blockTextureSlotOffset);
    viewLocChunksTranslucent = glGetUniformLocation(translucentChunkShaderProgram.ID, "view");
    projectionLocChunksTranslucent = glGetUniformLocation(translucentChunkShaderProgram.ID, "projection");
// Msg(Done with chunk shaders)

    return true;
}


void ChunkRenderer::renderChunks(World& world, glm::mat4& view, glm::mat4& projection){

    std::vector<std::shared_ptr<RenderableChunkMesh>> chunksToUploadGPU;
    std::vector<ChunkID> chunksToRender;

    // Loop through the renderable chunk meshes.
    // START_TIMING(renderableChunkQueue)
    std::queue<std::shared_ptr<RenderableChunkMesh>> chunkQueue = world.toRenderableChunkQueue();
    // END_TIMING(renderableChunkQueue)

    // START_TIMING(checkingChunksToUpdate)
    while(!chunkQueue.empty()){
        std::shared_ptr<RenderableChunkMesh> chunkPtr = chunkQueue.front();
        chunkQueue.pop();
        ChunkID chunkID = chunkPtr->chunkId;
        if(solidMeshes.count(chunkID)!=0 && !chunkPtr->updated){
            chunksToRender.push_back(chunkID);
        } else {
            chunksToUploadGPU.push_back(chunkPtr);
        }
    }

    LocInt playerLoc = world.player.getBlockLoc();
    if(chunksToUploadGPU.size()>maxNewMeshesPerFrame){
        std::nth_element(chunksToUploadGPU.begin(),chunksToUploadGPU.begin()+maxNewMeshesPerFrame-1,chunksToUploadGPU.end(),
        [playerLoc](const std::shared_ptr<RenderableChunkMesh>& m1, const std::shared_ptr<RenderableChunkMesh>& m2){
            int dx1 = (m1->chunkId.x-playerLoc.x);
            int dz1 = (m1->chunkId.z-playerLoc.z);
            int dx2 = (m2->chunkId.x-playerLoc.x);
            int dz2 = (m2->chunkId.z-playerLoc.z);
            return dx1*dx1+dz1*dz1<dx2*dx2+dz2*dz2;
        });
    }
    for(int i=0; i<maxNewMeshesPerFrame && i<chunksToUploadGPU.size(); i++){
        std::shared_ptr<RenderableChunkMesh> chunkPtr = chunksToUploadGPU[i];
        ChunkID chunkID = chunkPtr->chunkId;
        if( solidMeshes.count(chunkID) ){
            updateRenderMesh(chunkID, *chunkPtr);
        } else {
            createRenderMesh(chunkID,*chunkPtr);
        }
        chunksToRender.push_back(chunkID);
    }
    // END_TIMING(checkingChunksToUpdate)

    auto closerToPlayer = [playerLoc](const ChunkID& m1, const ChunkID& m2){
        int dx1 = (m1.x-playerLoc.x);
        int dz1 = (m1.z-playerLoc.z);
        int dx2 = (m2.x-playerLoc.x);
        int dz2 = (m2.z-playerLoc.z);
        return dx1*dx1+dz1*dz1<dx2*dx2+dz2*dz2;
    };
    // Assuming terrain Renders >= water Renders >= vegetation renders    
    if(chunksToRender.size()>maxVegetationRenders){
        std::nth_element(chunksToRender.begin(),chunksToRender.begin()+maxVegetationRenders-1,chunksToRender.end(),closerToPlayer);
    }
    if(chunksToRender.size()>maxWaterRenders){
        std::nth_element(chunksToRender.begin()+maxVegetationRenders,chunksToRender.begin()+maxWaterRenders-1,chunksToRender.end(),closerToPlayer);
    }
    if(chunksToRender.size()>maxTerrainRenders){
        std::nth_element(chunksToRender.begin()+maxWaterRenders,chunksToRender.begin()+maxTerrainRenders-1,chunksToRender.end(),closerToPlayer);
    }

    // START_TIMING(solidChunks)
    // Set the view and projection matrices to the right values in the chunk shader program
    solidChunkShaderProgram.use();
    glUniformMatrix4fv(viewLocChunksSolid, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLocChunksSolid, 1, GL_FALSE, glm::value_ptr(projection));
    solidChunkShaderProgram.setBool("ambientOcclusionFlag",ambientOcclusionFlag);
    solidChunkShaderProgram.setBool("gammaCorrectionFlag",gammaCorrectionFlag);
    for(int i=0; i<maxTerrainRenders&& i<chunksToRender.size(); i++){
        ChunkID chunkID = chunksToRender[i];
        glBindVertexArray(solidMeshes[chunkID].VAO);
        glDrawArrays(GL_TRIANGLES, 0, solidMeshes[chunkID].nrVertices);
    }
    // END_TIMING(solidChunks)

    // START_TIMING(cutoutChunks)
    cutoutChunkShaderProgram.use();
    glUniformMatrix4fv(viewLocChunksCutout, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLocChunksCutout, 1, GL_FALSE, glm::value_ptr(projection));
    cutoutChunkShaderProgram.setBool("gammaCorrectionFlag",gammaCorrectionFlag);
    for(int i=0; i<maxVegetationRenders&& i<chunksToRender.size(); i++){
        ChunkID chunkID = chunksToRender[i];
        glBindVertexArray(cutoutMeshes[chunkID].VAO);
        glDrawArrays(GL_TRIANGLES, 0, cutoutMeshes[chunkID].nrVertices);
    }
    // END_TIMING(cutoutChunks)

    // START_TIMING(translucentChunks)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    translucentChunkShaderProgram.use();
    glUniformMatrix4fv(viewLocChunksTranslucent, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLocChunksTranslucent, 1, GL_FALSE, glm::value_ptr(projection));
    translucentChunkShaderProgram.setBool("gammaCorrectionFlag",gammaCorrectionFlag);
    for(int i = std::min(maxWaterRenders,(int)chunksToRender.size())-1;i>=0; i--){
        ChunkID chunkID = chunksToRender[i];
        glBindVertexArray(translucentMeshes[chunkID].VAO);
        glDrawArrays(GL_TRIANGLES, 0, translucentMeshes[chunkID].nrVertices);
    }
    // END_TIMING(translucentChunks)


    glDisable(GL_BLEND);
}

// std::vector<ChunkRenderer::SolidVBOElt> ChunkRenderer::updateSolidVBOVector(const RenderableChunkMesh& worldMesh){
//     std::vector<SolidVBOElt> vertices;
//     // The face has numbered corners starting in the bottom left corner and going counter clockwise.
//     // The corners that correspond to the two triangles that make up the face.
//     const int cornerOrder[6] = {0,1,3,1,2,3};
//     // The uv diffs that correspond to the four corners of the face.
//     const glm::vec2 uvDiff[] = {{0,1},{1,1},{1,0},{0,0}};
//     for( auto face: worldMesh.solidMesh){
//         // Triangles counter-clockwise
//         // glm::vec2 uvCoord = TextureAtlas::getUVCoord(face.blockType,face.faceType);
//         for(int i=0;i<6;i++){
//             SolidVBOElt vboElt;
//             int corner = cornerOrder[i];
            
//             vboElt.pos[0] = face.corners[corner].x;
//             vboElt.pos[1] = face.corners[corner].y;
//             vboElt.pos[2] = -face.corners[corner].z; // Mirror the z!
//             vboElt.uvl = glm::vec3(uvDiff[corner],BlockRegistry::getTextureIndex(face.blockType,face.faceType));
//             // vboElt.uv[0] = uvCoord.x+(textureMargin+(1.0f-2.0f*textureMargin)*uvDiff[corner].first)*TextureAtlas::getTextureSizeWidth();
//             // vboElt.uv[1] = uvCoord.y+(textureMargin+(1.0f-2.0f*textureMargin)*uvDiff[corner].second)*TextureAtlas::getTextureSizeHeight();
//             vboElt.tint[0] = face.tint[0];
//             vboElt.tint[1] = face.tint[1];
//             vboElt.tint[2] = face.tint[2];
//             vboElt.tint[3] = 255;

//             vertices.push_back(vboElt);
//         }
//     }
//     return vertices;
// }

// std::vector<ChunkRenderer::CutoutVBOElt> ChunkRenderer::updateCutoutVBOVector(const RenderableChunkMesh& worldMesh){
//     std::vector<CutoutVBOElt> vertices;
//     // The face has numbered corners starting in the bottom left corner and going counter clockwise.
//     // The corners that correspond to the two triangles that make up the face.
//     const int cornerOrder[6] = {0,1,3,1,2,3};
//     // The uv diffs that correspond to the four corners of the face.
//     const glm::vec2 uvDiff[] = {{0,1},{1,1},{1,0},{0,0}};
//     for( auto face: worldMesh.cutoutMesh){
//         // std::cout << "Face in the cutout mesh !" << std::endl;
//         // Triangles counter-clockwise
//         // glm::vec2 uvCoord = TextureAtlas::getUVCoord(face.blockType,FaceType::Side);
//         for(int i=0;i<6;i++){
//             CutoutVBOElt vboElt;
//             int corner = cornerOrder[i];
            
//             vboElt.pos[0] = face.corners[corner].x;
//             vboElt.pos[1] = face.corners[corner].y;
//             vboElt.pos[2] = -face.corners[corner].z; // mirror the z!
//             vboElt.uvl = glm::vec3(uvDiff[corner],BlockRegistry::getTextureIndex(face.blockType,FaceType::Side));
//             // vboElt.uv[0] = uvCoord.x+(textureMargin+(1.0f-2.0f*textureMargin)*uvDiff[corner].first)*TextureAtlas::getTextureSizeWidth();
//             // vboElt.uv[1] = uvCoord.y+(textureMargin+(1.0f-2.0f*textureMargin)*uvDiff[corner].second)*TextureAtlas::getTextureSizeHeight();
//             // vboElt.uv[0] = uvCoord.x+uvDiff[corner].first*TextureAtlas::getTextureSizeWidth();
//             // vboElt.uv[1] = uvCoord.y+uvDiff[corner].second*TextureAtlas::getTextureSizeHeight();
//             vboElt.tint[0] = face.tint[0];
//             vboElt.tint[1] = face.tint[1];
//             vboElt.tint[2] = face.tint[2];
//             vboElt.tint[3] = 255;
//             vertices.push_back(vboElt);
//         }
//     }
//     return vertices;
// }

// std::vector<ChunkRenderer::TranslucentVBOElt> ChunkRenderer::updateTranslucentVBOVector(const RenderableChunkMesh& worldMesh){
//     std::vector<TranslucentVBOElt> vertices;
//     // The face has numbered corners starting in the bottom left corner and going counter clockwise.
//     // The corners that correspond to the two triangles that make up the face.
//     const int cornerOrder[6] = {0,1,3,1,2,3};
//     // The uv diffs that correspond to the four corners of the face.
//     const glm::vec2 uvDiff[] = {{0,1},{1,1},{1,0},{0,0}};
//     for( auto face: worldMesh.translucentMesh){
//         // Triangles counter-clockwise
//         // glm::vec2 uvCoord = TextureAtlas::getUVCoord(face.blockType,face.faceType);
//         for(int i=0;i<6;i++){
//             TranslucentVBOElt vboElt;
//             int corner = cornerOrder[i];
            
//             vboElt.pos[0] = face.corners[corner].x;
//             vboElt.pos[1] = face.corners[corner].y;
//             vboElt.pos[2] = -face.corners[corner].z; // Mirror the z!
//             // vboElt.uv[0] = uvCoord.x+(textureMargin+(1.0f-2.0f*textureMargin)*uvDiff[corner].first)*TextureAtlas::getTextureSizeWidth();
//             // vboElt.uv[1] = uvCoord.y+(textureMargin+(1.0f-2.0f*textureMargin)*uvDiff[corner].second)*TextureAtlas::getTextureSizeHeight();
//             vboElt.uvl = glm::vec3(uvDiff[corner],BlockRegistry::getTextureIndex(face.blockType,face.faceType));
//             vboElt.tint[0] = face.tint[0];
//             vboElt.tint[1] = face.tint[1];
//             vboElt.tint[2] = face.tint[2];
//             vboElt.tint[3] = 255;

//             vertices.push_back(vboElt);
//         }
//     }
//     return vertices;
// }


void ChunkRenderer::updateRenderMesh(const ChunkID& chunkID, RenderableChunkMesh& worldMesh){
    // std::vector<SolidVBOElt> solidVertices = updateSolidVBOVector(worldMesh);
    // solidMeshes[chunkID].nrVertices = 6*worldMesh.solidMesh.size(); 
    solidMeshes[chunkID].nrVertices = worldMesh.solidMesh.size(); 
    glBindVertexArray(solidMeshes[chunkID].VAO);
    glBindBuffer(GL_ARRAY_BUFFER,solidMeshes[chunkID].VBO);
    glBufferData(GL_ARRAY_BUFFER, worldMesh.solidMesh.size() * sizeof(SolidVBOElt), worldMesh.solidMesh.data(), GL_STATIC_DRAW);

    // std::vector<CutoutVBOElt> cutoutVertices = updateCutoutVBOVector(worldMesh);
    // cutoutMeshes[chunkID].nrVertices = 6*worldMesh.cutoutMesh.size(); 
    cutoutMeshes[chunkID].nrVertices = worldMesh.cutoutMesh.size(); 
    glBindVertexArray(cutoutMeshes[chunkID].VAO);
    glBindBuffer(GL_ARRAY_BUFFER,cutoutMeshes[chunkID].VBO);
    glBufferData(GL_ARRAY_BUFFER, worldMesh.cutoutMesh.size() * sizeof(CutoutVBOElt), worldMesh.cutoutMesh.data(), GL_STATIC_DRAW);

    // std::vector<TranslucentVBOElt> translucentVertices = updateTranslucentVBOVector(worldMesh);
    // translucentMeshes[chunkID].nrVertices = 6*worldMesh.translucentMesh.size(); 
    translucentMeshes[chunkID].nrVertices = worldMesh.translucentMesh.size(); 
    glBindVertexArray(translucentMeshes[chunkID].VAO);
    glBindBuffer(GL_ARRAY_BUFFER,translucentMeshes[chunkID].VBO);
    glBufferData(GL_ARRAY_BUFFER, worldMesh.translucentMesh.size() * sizeof(TranslucentVBOElt), worldMesh.translucentMesh.data(), GL_STATIC_DRAW);

    worldMesh.updated = false;
}



void ChunkRenderer::createRenderMesh(const ChunkID& chunkID, RenderableChunkMesh& worldMesh){
    RenderMesh solidRenderMesh;
    glGenVertexArrays(1, &solidRenderMesh.VAO);
    glBindVertexArray(solidRenderMesh.VAO);

    glGenBuffers(1, &solidRenderMesh.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, solidRenderMesh.VBO);
    // std::vector<SolidVBOElt> solidVertices = updateSolidVBOVector(worldMesh);
    // glBufferData(GL_ARRAY_BUFFER, solidVertices.size()*sizeof(SolidVBOElt), solidVertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, worldMesh.solidMesh.size()*sizeof(SolidVBOElt), worldMesh.solidMesh.data(), GL_STATIC_DRAW);
    // solidRenderMesh.nrVertices = worldMesh.solidMesh.size()*6;
    solidRenderMesh.nrVertices = worldMesh.solidMesh.size();
    // Position attribute:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SolidVBOElt), (void*)offsetof(SolidVBOElt, pos));
    glEnableVertexAttribArray(0);
    // uvl attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SolidVBOElt), (void*)offsetof(SolidVBOElt, uvl));
    glEnableVertexAttribArray(1);
    // tint attribute
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(SolidVBOElt), (void*)offsetof(SolidVBOElt, tint));
    glEnableVertexAttribArray(2);
    // occlusion attribute
    glVertexAttribPointer(3, 1, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(SolidVBOElt), (void*)offsetof(SolidVBOElt, occlusion));
    glEnableVertexAttribArray(3);
    solidMeshes[chunkID] = solidRenderMesh;


    RenderMesh cutoutRenderMesh;
    glGenVertexArrays(1, &cutoutRenderMesh.VAO);
    glBindVertexArray(cutoutRenderMesh.VAO);

    glGenBuffers(1, &cutoutRenderMesh.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, cutoutRenderMesh.VBO);
    // std::vector<CutoutVBOElt> cutoutVertices = updateCutoutVBOVector(worldMesh);
    // glBufferData(GL_ARRAY_BUFFER, cutoutVertices.size()*sizeof(CutoutVBOElt), cutoutVertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, worldMesh.cutoutMesh.size()*sizeof(CutoutVBOElt), worldMesh.cutoutMesh.data(), GL_STATIC_DRAW);
    // cutoutRenderMesh.nrVertices = worldMesh.cutoutMesh.size()*6;
    cutoutRenderMesh.nrVertices = worldMesh.cutoutMesh.size();
    // Position attribute:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CutoutVBOElt), (void*)offsetof(CutoutVBOElt, pos));
    glEnableVertexAttribArray(0);
    // uvl attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CutoutVBOElt), (void*)offsetof(CutoutVBOElt, uvl));
    glEnableVertexAttribArray(1);
    // tint attribute
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(CutoutVBOElt), (void*)offsetof(CutoutVBOElt, tint));
    glEnableVertexAttribArray(2);
    cutoutMeshes[chunkID] = cutoutRenderMesh;


    RenderMesh translucentRenderMesh;
    glGenVertexArrays(1, &translucentRenderMesh.VAO);
    glBindVertexArray(translucentRenderMesh.VAO);

    glGenBuffers(1, &translucentRenderMesh.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, translucentRenderMesh.VBO);
    // std::vector<TranslucentVBOElt> translucentVertices = updateTranslucentVBOVector(worldMesh);
    // glBufferData(GL_ARRAY_BUFFER, translucentVertices.size()*sizeof(TranslucentVBOElt), translucentVertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, worldMesh.translucentMesh.size()*sizeof(TranslucentVBOElt), worldMesh.translucentMesh.data(), GL_STATIC_DRAW);
    // translucentRenderMesh.nrVertices = worldMesh.translucentMesh.size()*6;
    translucentRenderMesh.nrVertices = worldMesh.translucentMesh.size();
    // Position attribute:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TranslucentVBOElt), (void*)offsetof(TranslucentVBOElt, pos));
    glEnableVertexAttribArray(0);
    // uvl attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TranslucentVBOElt), (void*)offsetof(TranslucentVBOElt, uvl));
    glEnableVertexAttribArray(1);
    // tint attribute
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(TranslucentVBOElt), (void*)offsetof(TranslucentVBOElt, tint));
    glEnableVertexAttribArray(2);
    translucentMeshes[chunkID] = translucentRenderMesh;

    worldMesh.updated = false;
}

