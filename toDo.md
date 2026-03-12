# Todo list


## Nu aan werken:

Checken of het blokje niet solid is als ik de boom bouw!
Water beide richtingen op renderen!
Als ik onderwater ben, het hele scherm blauw maken.






### Other 
Important: Adding an unloading distance for the chunks and the meshes!

We need to some kind of centralized tint registry, for block with tints.

Sort the translucent blocks.

We should change the block targeting algorithm and also check whether the block is the type that can be placed unto.
After we change the block targeting algorithm, we can also change the hitbox of objects such as flowers and change the shape of the indicator box for that case.

Physics.


- Optional (Optimization): Calculate the uv coordinates and stuff in the mesh creation function, since it can run in parallel. Good idea dude! But for later I guess.
- Optional (Optimization): Change the render mesh to use local coordinates, so we can use uint_8. Then in the shader, we can add the chunkID location in parallel.
- Optional (Optimization): Change the renderQueue function to give, for example, raw pointers, and to maybe preserve information better. Like we can actually check if we move to a new chunk and then remove the exact ones or something. Hmmm not sure if that works though. lets see.
- Optional (Cleaner Code): De hotbar texture naar de texture class sturen voor consistentie.
- Optional (Cleaner Code): De rendercode mooi encapsulaten zodat de main render functie Heel overzichtelijk wordt.
- Optional: Fix mipmap shizzzle
- Optional: Texture array instead of texture atlas.
- Optional: tick delay voor muis ingedrukt houden, maar geen delay voor muis spammen!


And then for the raytracing, we simply need to check whether its not an air block.
But Im gonna change the raytracing anyways, for entity and hitbox detection and stuff.



Bloemen toevoegen
Bomen toevoegen.


### Entitities


### 


<!-- Refactoren voor playerClass in plaats van Camera. (Gefixed!!!) -->


## Misc en langetermijn ideeen

Camera mode en speler mode? Dus de handling bepaalt de player locatie en waar die naar kijkt en dan wordt de camera geupdate?
Raytracing aanpassen voor ofwel klein stapje, of elke keer door de dichtsbijzijnde surface ipv drie verschillende loops.
Water
biomes
Coolere world Generation

struct  van verschillende perlin noises?

See through shizzle

Wereld saven

chunks als chunklets van 16x16x16 en dan als het helemaal leeg is gewoon als een air chunklet opslaan.


## Al gefixt/afgehandeld:
<!-- input handling hybride:
Deels in de game en deels in de wereld
In de input handler twee verschillende states:
Een die checkt tegen opzichte van de vorige loop
en een die wacht tot een press of release geconsumeerd is. -->


<!-- input handling aanpassen: De input state wordt doorgegeven aan de world en die
consumed events zoals release en pressed, maar laat de down staan. -->

<!-- Game ticks van 1/20 per seconde met accumulators -->

<!-- posities interpoleren voor smooth graphics -->


<!-- Hmm ff nadenken over muis indrukken.
We hebben meer frames per game tick, dus het kan zijn dat we een een key hebben ingedrukt, 
maar volgens mij nu opgelost. -->
<!-- Muis ingedrukt kunnen houden voor deleten en plaatsen -->
<!-- Niet kunnen plaatsen als het blokje te dichtbij is -->

<!-- Range van plaatsen eentje verder dan deleten!
Ray tracing for block targets bepalen?
snel met een block id en een positie en richting de afstand tot het snijpunt bepalen. -->

<!-- Refactor code -->
<!-- Ronddraaien in pauze mode aanpassen -->

<!-- Structs met UIData pointers toevoegen. -->

<!-- Imgui instellen -->


<!-- In ImGui belangrijke stats toevoegen -->
<!-- In Camera de muisdraai snelheid toevoegen.(Nevermind, staat in de player class.) -->
<!-- Bij Pause mode alles pauzeren! -->

<!-- Default variabele waardes van de constructor naar de header verplaatsen. -->

<!-- Crashes als ik buiten de chunks ben en een cube in een chunk probeer te verwijderen. Gefixt! -->

<!-- Mijn OpenGl code opnieuw begrijpen. -->

<!-- Alle IMGUI dingen in een ander bestand. -->

<!-- Lijnen rond geselecteerd blokje -->

<!-- Setting up an opengl testing folder. -->

<!-- Textures file met texture class om dat overzichtelijk te krijgen. -->

<!-- Plusje voor richten
9 vierkantjes onderin -->

<!-- Crosshair  -->

<!-- inventory slots. Hotbar selection toevoegen aan player en imgui. -->
<!-- Daarna veldje renderern dat geselecteerd is. -->
<!-- Daarna in de input een hotbar selection toevoegen. -->

<!-- inventory toevoegen aan de player.(misschien met bool voor empty, kan later int voor hoeveelheid worden.) -->

<!-- Daarna: inventory toevoegen voor verschillende blokjes kunnen plaatsen. -->


<!-- UI library maken. -->
<!-- Mijn idee: functie die een rechthoek op het scherm output.
Krijgt een locatie, width and height en een kleur. thats it.IN -->
<!-- verschillende types blokjes kunnen plaatsen -->
<!-- Remove the include folder and move everything to src -->


<!-- Render hotbar blocks -->

<!-- ### Chunks rond speler rednerern
Chunks spawnen rond de speler?
Uitzoeken wie verantwoordelijk is voor het bijhouden van welk chunks we wel en niet renderen.(world of chunkManager?)
De world checkt welke chunks in range van de player zijn. -->
<!-- Frame rate toevoegen aan ImGui.

Chunk rendering and generation rond de speler. -->

<!-- Change the chunk mesh generation with isOpaque instead of isSolid. -->
<!-- Meer type blokjes toevoegen. -->

<!-- Change types, solid for when youre not able to walk through it
opaque for when the block is not transparent. -->

<!-- zwarte lijn blijk blokjes weg als ik vanuit een hoek kijk. Ja wat de hek is dit? Its like a circle around the player where you get these lines and de blocks get darker as well. -->
<!-- Add a flag to the chunks, so we dont start another thread to calculate the mesh. so two flags: dirty and calculating mesh (atomic maken?)
The calculating mesh moet door de chunkmanager gezet worden in de main thread (important).
Add: calculate meshes and calculate meshesAsync.
We have to separate the functions that give the calculated meshes to the renderer and that start the chunk mesh calculation.
So we like I said, I write the calculateMeshes and calculateMeshesAsync functions. Both of these check an area around the player and if the chunk is dirty and is not currently generating we start the mesh creation.
The async doesn this asynchronously and we can call this after calculating the new position of the player, at the start of the world update function.
The non async does it in the main thread after all the game logic. This is important, because these chunks are close to the player, so need to be rendered.
Then the get chunkmeshes function loops through the chunks and checks which ones are not dirty and finished generating and we send it to the renderer.
Question: We need to make sure that the renderer only renders the chunks that are indeed finished. hmmmm.
So the renderer can check whether a mesh has changed. otherwise it just uses the old vbo. 
Ooh I need to double check this.
Oh right, the renderable mesh has an updated flag, which only the renderer can turn off again I think.
So the renderer actually changes the chunkmeshes. 
Thats quite clever.
And the chunk mesh creation changes the flag to updated and the renderer turns the flag off right?
Also, the world only puts the chunkID's in the queue of what needs to be rendered. 
Hmm, the chunk mesh creator puts the updated flag to true at the start of the function. Can this cause problems?
Dont think so, the chunkmanager sets the generatingChunkFlag to true and the chunk should put that to false at the end of generating. 
Just to be safe, I put the chunk updated flag to the end of the function, so the renderer only updates the mesh when the chunk is actually done.



Wat we need to do for asynchronous generating and mesh calculation:
- A check for a chunk whether all surrounding chunks have been generated.
- A chunk generation funciton that can be parallelized.
- We first construct the chunk normally though
- Then we let the chunks be generated asynchronously. We have an std::atomic<bool> flag to indicate that a chunk is done generating.
- A rendermesh function that can be parallelized.
- When we move into a new chunk, we check whether there are chunks in the renderrange whose mesh still needs to be rendered.
 We check whether all adjacent chunks have been generated. If yes, we do the mesh creation function asynchronously
 If not, we add the chunk the an ordered set and we attempt next iteration.
 For the pending data structure, we also check whether the chunk is still in rendering range.
- At the end of the game loop, we check all the dirty chunks and calculate the mesh in the main loop.
This is important, because these chunk are close to the player, so its very important that they are rendered.

To do now: Change the chunk class, so we have a constructor and a separate generation function.
We need a flag for when the chunk is done generating. 
and a flag for when the chunk is dirty and for when the chunk is peding mesh rendering

Brainstorm:
Ok so I have a chunk constructor and a separate chunk generator. Thats good. So the  -->

<!-- Idea for parallelizing the chunk generation and chunk rendering:
We start the game loop by deciding the new position of the player.
Then we use the new position to determine whether there are chunks that need to be generated and meshes that need to be rendered.
These are then added to asynchronous threads and run in the background. Afterwards, we do all the game logic.

chunk generation queue

mesh render queue

parallelization for both.

Changing rendering distance. -->

<!-- Look into: the game crashed when I was placing blocks at the top of the chunk. What happens when you try to place above the max Y? -->

<!-- Add a queue to the renderer class, so it doesnt process all new meshes at the same time.
But instead a couple per game frame.
Ok so we add a variable that gives the max amount of chunks to process per frame.
We need to be pretty careful about how we do it though. For example, we always want to do the chunks that are closest to us.
We could use a binary search tree that orders on the distance to the player and then take the chunks closest.
Should be very fast.
There are some caveats though. WE dont want to add things to the queue multiple times and we should remove things that are out of range.
Ok so what we could do, is that each frame, we loop through what we need to render and then we just draw all the things that we 


Wait lets restart. We have a range that indicates which chunks should be rendered ideally.
of these, there are some that dont have a proper mesh yet. We throw these away, we dont even send them to the renderer.
So the world class actually sends a queue of chunks who are in range and have a proper chunk mesh.
Of these, some have the mesh loaded into the gpu already, some have a mesh loaded into the gpu, but its an old version,
and some need to load the mesh to the gpu.
The ones who have the proper gpu loaded already, we should just draw for sure.
then for the closest ones, we update or add the new mesh to the gpu and render them.
This should be a limit per frame, except maybe at the start.(we can add a bool parameter to the render function that says whether we should render all or not)
Then we have a bunch of chunkIDs for chunks with a proper mesh that we should load in.
We order these, by for example putting them in a vector and sorting by distance to the player.
Take the chunks closest, update or make the chunk vbo and set the updated flags to false. This is a bool that only the renderer can set to false. -->

<!-- Nu doen:
Change the renderer render chunks function, so we first accumulate all the chunkID that need to be rendered in vector.
Before that, we makes sure that the VBO's are up to date.
So it consists of two parts: updating all the VBOs and then rendering everything.
In the first part, we also check for each chunk whether we already have an up to date vbo, we need to build it altogether or we need to update the vbo's 
I suggest we make a function that updates the vbos properly. In this function we can also do all the different vbos, such as the solidMesh and the cutoutMesh.
Question: In terms of VAO's and VBO's, do we want a single map that contains all the structs, or do we want a map for each type of mesh? I think having a struct that contains the different vaos and vbos makes more sense actually.
Ok, to recap: In the chunk render function we still have the same for loops for the chunkID and checking which are already loaded and which ones need loading and we keep the nth_element algorithm.

Then in the end we have a vector of chunkID that need to be rendered and we first render all the solidmeshes then load the shader and change the settings for the cutoutmeshes.

Alright sounds like a good plan. Lets start by refactoring the chunkrender class without changing the functionality and then we add the cutoutMeshes once this works. -->

<!-- Bloemen: Gewoon eerst een flat kans om te spawnen?
We make a mesh for the flowers per chunk. 

Question 1: When do we update the cutoutMesh? Same as dirty. It doesnt matter for the optimization I guess, because we dont change chunks that often.
Question 2: Do we store the cutoutMesh in the same data structure? And then we have an extra VAO and VBO for the cutoutMesh.
We can do the diagonal locations. Only difference is, we need a different shader for the cutOutMEsht and we need to set the culling setting off.
Idea: In the renderer, we fill the vector with the ChunkID's we want to render, and then first, we render the normal chunks and afterwards we render the cutouts. Yeah great idea!
I want to add the chunkID offset to the mesh, so that we only need the u_int8 for the locations. And then we set the uniform variable chunkID in the shader. Parallelization, yeah!
So I guess in the renderable, we could maybe even use the same. No we wont. We will make a special cutOut data structure.
Making the new mesh should be easy enough actually. 


Alrigh! What Im going to do tomorrow: IN the renderer, I will first update all the chunk vaos. So they contain the proper mesh.
Then afterwards, I first render all the solid meshes and afterwards render all the cutoutMeshes. Sounds like the plan for me!
Need to continue with the renderChunks function then.
So plan: fix the renderChunks function. 
Make two functions for initializing the cutout VAO and one for updating the cutout VAO.
Write the shaders for the cutout meshes. 
Test if the flowers will render from the inventory.
Add the flowers to the chunk generation. -->

<!-- Add leaves, then add water.
First only from the inventory, afterwards add water to chunk generation (just add a water level), then add vegetation to generation
Keep face culling disabled, but add both sides of the flower texture to the mesh

Which categories should I have for blocks?
I mean there's solid, which means we cant walk through it.
There's opaque so it's not transparent.
Then there blocks that are not opaque, but they are solid and not translucent.
There are also block that are solid, not opaque and not translucent.
I guess we can add a translucent flag.
water is translucent, not opaque and not solid
glass is solid and translucent
leaves are solid and neither translucent nor opaque.


add a tint to the cutoutMesh, because leaves have tints apparently. -->

<!-- Water blocks kunnen renderen.
Wat moet hiervoor gebeuren?
We moeten een translucent shader toevoegen
Een translucent VBO constructor en VAO constructor
Met texture en tint!
Water block. translucentMesh creator, probably same way as leaves, should be easy I think, for now we just check whether the neighboring block is water. Lets start with that.
So add water to blockregistry->add water to player inventory -> add water to mesh creation -> water at correct tint for inventory.
TranslucentVBOElt, translucentMesh, translucentMeshes, 
Kijken hoe we die translucent mesh creeeren.



sort the translucent boxes? Yes!
Camera underwater should make the whole thing more blue.
Maybe tint the entire screen?


Water is rendering!
Todo: dont add water face if its next to another water! Yes did this! -->

<!-- Terrain generation!
First fix the elevation and we can add a water level easily.
Im thinking of adding two perlin noises. One with a higher amplitude and greater distance and another with smaller amplitude and more local.

So first we need to make a struct that contains everything required for chunk generation. Im thinking of writing a header and cpp file as well so its not all contained in the chunk class.
The thing, though, is to make sure that the chunk generation can run in parallel.
So we want to copy this whole chunk generation struct for each chunk, thats fine.
We only need some perlin noise thingies I guess.
And then in the chunk generation class we could add some static functions, or put functions in a namespace.
Its very important that it doesnt contain any state, because that complicates asynchronous generation.
I guess a namespace with the generation is a good idea. -->




<!-- Ok adding trees might be tricky, because of asynchronous.
But I think I have a solution: We can only add trees when all neighbors are done generating chunks
And then we can only renderMeshes when all neighbors are done generating trees.
But what if two threads are simultaneously generating trees?
We need an atomic flag indicating that they are not currently adding vegetation.

Ok so the point is, we can only create meshes when trees are created in all the surrounding chunks: easy enough.
Creating trees: We can only create trees when all neighbors have generated the terrain.
Ok so we need two extra flags for the chunks: generatedTrees and currentlyGeneratingTrees.

So we really need to focus on the flags now and not forget anything, because it can destroy stuff!!!!


Ok so we need to write a generateFlags function that can run asynchronously ( so with pointers to its neighbors).
For the generateFlags function, we need to check that all neighbors are done generating chunks and are not currently generating the tree or generating the mesh.
(Calculating the mesh is not possible, because we only allow starting the mesh when all neighbors are done generating trees)

We then need to change the mesh creation kick off to check that the chunks are done creating trees.

Alright the tree generation is working! The game did slow down a lot though.
right now Im just placing logs, but Im adding leaves as we speak.
The tree shape Im going for: I think I'll make a queue that contains the offsets of the leaves and then loop through those. yeah the shape is like: 6 logs on top of each other. 3 empty layers, 5x5, 5x5,3x3 and a plus on top

Shiiiiiiieeeet, the tree generation needs 8 neighbors! Ok I think thats easy to handle actually.
First, I will make a new struct with the 8 neighborOffsets.
Then, I will change the allow tree generation function.
Then I will make sure that the tree generation function takes in an array of size 8 of chunk Pointers.
then I will add the setBlockID chunk functions that can place in the spots of the neighbors.
Yeah sounds doable. -->