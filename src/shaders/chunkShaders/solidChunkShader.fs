#version 330 core
out vec4 FragColor;  

in vec3 TexCoord;
in vec4 fragTint;

uniform sampler2DArray textureArray;
  
void main()
{
    FragColor = fragTint*texture(textureArray,TexCoord);
    // This will be maybe useful later if we have transparent textures.
    // if(FragColor.a<0.5f){
    //     discard;
    // }
}