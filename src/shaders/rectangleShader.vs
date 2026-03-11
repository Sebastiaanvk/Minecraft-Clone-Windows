#version 330 core
layout (location = 0) in vec2 aPos;   // the position variable has attribute position 0
  
uniform vec2 loc;
uniform vec2 size;

void main()
{
    gl_Position = vec4(loc.x+aPos.x*size.x,loc.y+aPos.y*size.y,0.0, 1.0);
}   
