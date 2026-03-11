#version 330 core
out vec4 FragColor;  

in vec2 TexCoord;

uniform sampler2D textureAtlas;
uniform vec4 tint; // The tints are floats in [0.0,1.0]
  
void main()
{
    FragColor = tint*texture(textureAtlas,TexCoord);
    if(FragColor.a<0.5){
        discard;
    }
}