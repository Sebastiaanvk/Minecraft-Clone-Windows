#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float sideIndex;
  
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 textureOffset[6];
uniform vec2 textureSize;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord =    aTexCoord*textureSize + textureOffset[int(sideIndex)]; 
}   