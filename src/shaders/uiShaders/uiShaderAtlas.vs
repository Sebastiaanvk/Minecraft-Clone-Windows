#version 330 core
layout (location = 0) in vec2 aPos;   
layout (location = 1) in vec2 aTexCoord;
  
out vec2 TexCoord;
// out vec4 fragTint;

uniform vec2 loc;
uniform vec2 size;
uniform vec2 textureLoc;
uniform vec2 textureSize;

void main()
{
    gl_Position = vec4(loc.x+aPos.x*size.x,loc.y+aPos.y*size.y,0.0, 1.0);
    TexCoord =    vec2(textureLoc.x+aTexCoord.x*textureSize.x,textureLoc.y+aTexCoord.y*textureSize.y); 
}   