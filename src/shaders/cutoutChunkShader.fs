#version 330 core
out vec4 FragColor;  

in vec2 TexCoord;
in vec4 fragTint;

uniform sampler2D textureAtlas;
  
void main()
{
    FragColor = fragTint*texture(textureAtlas,TexCoord);
    // FragColor = texture(textureAtlas,TexCoord);
    if(FragColor.a<0.5f){
        discard;
    }
}