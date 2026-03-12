# DevLog

## 12/3/2026
Alright, won't have as much time today, but anyways.
Added a leaf to my tree that I forgot to render.

I fixed that trees dont places leaves into non-air blocks.
Water faces now render both sides.
When underwater, it longer displays the block you're targeting.

## 11/3/2026
Ok I spent the morning trying to get everything to run on another computer.
Apparently some setups were not as universal as I thought, so I had to install the mingw gcc compiler and re compile the glfw and everything.
Of course this stuff is always more work than you expected, but at least I can tell I'm getting faster at dealing with problems and understanding how this all works.
Quite dependent on AI to help me brainstorm the possible problems...

Anyways, going to work on adding and rendering water now.

Yay, added the translucent rendering and have see through water blocks now!
First I was also rendering the faces between water blocks, but I fixes that now.
Looks really good. The problem is that I actually need to sort all the translucent blocks in distance from the camera, because if you have two block behind each other with a gap, it wont always show the one in the back.
There is still depth testing for translucent blocks, so we ideally render the ones in the back first.
This means we have to sort all the water blocks every time a player moves and upload those to the gpu.
That's a lot of work. I will leave that to later.
For now I want to focus on world generation, because with water, trees and flowers I can generate super cools landscapes.

Currently thinking about how to do the chunk generation so that we can still run it all asynchronously.
Currently all the chunks have an attribute that contains the generation data.
But since we are only reading, its apparently safe to read from the same data as long as it never changes.
So I was considering making like a static class or a namespace.
I think I will do a namespace actually, seems perfect for this scenario.

<img src="assets\media\terrain water 11-3-2026.gif" width="900" >

I refactored the terrain generation code and I got water working! Looks really cool.

<img src="assets\media\flowers and grass 11-3-2026.png" width="900" >

Alright I also added flowers and grass to the chunk generation.


cool I added the tree generation. So far its just tree trunks though, I will add a tree model as a queue of offsets containing leaves I think.
The game did slow down a lot though. Still aroun 60 fps, but drops are frequent.
I know one way I can speed it up by a lot.
I also realized that I need to consider all 8 neighbors when making the tree and not just the four adjacent chunk neighbors. That will be a bit annoying to add.

In the middle of doing the trees, I added underwater plants.
It took a surprisingly short time, even though I add a bunch of things, for example: the I added a new underwater block flag, so water would also render on the face of a block containing an underwater plant.

<img src="assets\media\trees and underwater plants 11-3-2026.png" width="900">

Ok its looking quite nice now.
The trees are rendering. And the underwater plants are nice as well.
There's one problem, the program crashes when I try to do the tree generation asynchronously.

Wooow found the bug to why the tree generation was crashing.
That was some real cpp type shit. 
I move the pointers to the chunks to a c array, but of course then the pointers dont get copied.
So I changed it to std::array and that solved the problem.
I got pretty far with debugging, because I found that the neighboring chunks had a flag set to false that was previously true and there was no way set it back to false.
So I knew something was up with the pointers.
And then Claude came in clutch and told me about the problem with the c style array( which is basically just a pointer as well).

Tomorrow, I will make the trees check that there is not already a block on spot where they want to place the leaves.


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




