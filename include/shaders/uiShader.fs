#version 330 core
out vec4 FragColor;  

in vec2 TexCoord;

uniform sampler2D textureAtlas;
  
void main()
{
    FragColor = texture(textureAtlas,TexCoord);
    if(FragColor.a<0.5){
        discard;
    }
}