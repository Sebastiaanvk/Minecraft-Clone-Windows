#version 330 core
out vec4 FragColor;  

in vec2 TexCoord;
in vec4 fragTint;

uniform sampler2D textureAtlas;
  
void main()
{
    FragColor = fragTint*texture(textureAtlas,TexCoord);
    // This will be maybe useful later if we have transparent textures.
    // if(FragColor.a<0.5f){
    //     discard;
    // }
}