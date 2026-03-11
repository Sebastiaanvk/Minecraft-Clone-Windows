#version 330 core
out vec4 FragColor;  

in vec2 TexCoord;
in vec4 fragTint;

uniform sampler2D textureAtlas;
  
void main()
{
    // FragColor = vec4(0.0f,1.0f,0.0f,1.0f);
    FragColor = fragTint*texture(textureAtlas,TexCoord);
}