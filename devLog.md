# DevLog

## 10/3/2026
Working on rendering the flowers.
Alright I added the possibility to place flowers and they show up in the hotbar as well.
Problem is that we can place blocks unto flowers, I think I will fix that later, its not a problem for now.

Did need to change the block Targeting algorithms so now it just check whether the block is anything other than air.
I still dont render the flowers though.

Refactored the chunkRender function in the Renderer class and I think I found a major speed up: I didnt set the updated flag to false when creating the VBO, which meant it would update the VBO in the next frame, even if nothing changed.

Yay, I added the flower rendering. There were some hiccups naturally, but it works perfectly in the end. Plus, I refactored the render class, so adding the translucent rendering should be quite natural.
I added the leaves! Not too sure about which block flag I shall use. Ended keeping backside culling on and I just add both sides of the flowers to the meshes.
I had to add a tint parameter to the cutoutMesh, because the leaves are just in gray scale.

The leaves in the hotbar now also have the proper colour.

<img src="assets/media/Screenshot 2026-03-10 164607.png" width="900">

## 9/3/2026
Have been working a lot on the project lately.
Today I have finally fixed the asynchronous chunk generation, asynchronous mesh creation and setting a limit to the number of meshes that can be added to the gpu per frame.

I also did a lot of other optimizations. 
Seems to be running very smoothly now. 

Also removed the rendering of the bottom of the map, which speeds up quite a bit as well.
Guess I will start on other things now, such as water and flowers.

## Before

<img src="assets/media/chunkGeneration_9-3-2026.gif" width="900"> 
<img src="assets/media/blockPlacement_9-3-2026.gif" width="900"> 

Alright, I started on this project a while back, but took a very long break.
I think I did a bunch in September 2025. Then in February/March 2026 I picked it up again.

This devlog started at 9th of March 2026.
Before that I had the following features already:
World:
- World is split into chunks
- I have an enumeration called BlockType with the types of blocks in my world, such as dirt, stone, cobblestone, different types of wool
- The player can fly around the world and the world gets generated infinitely.
- the chunk generation and the mesh creation happens in different threads than the main thread.
- Right now the world only consists of dirt and grass dirt
- I use Perlin noise for the height of the land.
- The player has the ability to destroy and place different kinds of blocks

UI:
- I have Dear Imgui to show game data and we can chance game settings while the window is open.
- Pause the game with p
- There's a crosshair 
- We make have black lines around the selected block
- We have a hotbar with a couple of different block we can place. Scroll through the hotbar.

Rendering:
- Each chunk creates a mesh of the faces that are adjacent to a non-opaque block (such as air)
- The chunk has a dirty flag, to indicate that it needs to be recalculated.
- These meshes are sent to the Renderer if they are in range of the player. The structs for communication between the World class and the Renderer Class are in the Renderable.hpp file.
- The renderer checks if its a new chunk, an old chunk but with a new mesh or already has the proper mesh stored in the GPU
- If necessary, the mesh is translated to a proper VBO and uploaded to the GPU




