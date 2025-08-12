#version 330 core
out vec4 FragColor;  

// Not sure if we need this ourColor input
// in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D textureAtlas;
  
void main()
{
    FragColor = texture(textureAtlas,TexCoord);
    // This will be maybe useful later if we have transparent textures.
    // if(FragColor.a<0.5f){
    //     discard;
    // }
}