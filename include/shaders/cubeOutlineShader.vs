#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
  
uniform mat4 view;
uniform mat4 projection;
uniform vec3 offset;
uniform vec3 localOffset;

void main()
{
    gl_Position = projection * view * vec4(aPos+offset+localOffset, 1.0);
}   