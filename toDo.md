# Todo list


World.cpp: toRenderableChunkQueue

Meshdata met een shared pointer van de chunk naar de renderer sturen. Goed idee!

Chunk.cpp: Renderable fixen met ChunkID

Checken of chunk vbo moet worden aangepast. In de renderable chunk hebben we een flag die zowel de chunk als de renderer kunnen aan en uitzetten.



De world checkt welke chunks in range van de player zijn.

De renderer checkt welk vbos hij nog moet opslaan en welke niet. 


Be sure that everything regarding the z axis is not inverted!
This could be a problem. Double check the learnopengl tutorial, because maybe he already fixed it.

