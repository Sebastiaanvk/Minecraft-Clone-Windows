#version 330 core
out vec4 FragColor;  

in vec3 TexCoord;
in vec4 fragTint;

uniform sampler2DArray textureArray;
uniform bool gammaCorrectionFlag;

  
void main()
{
    FragColor = fragTint*texture(textureArray,TexCoord);
    // FragColor = texture(textureAtlas,TexCoord);
    if(FragColor.a<0.5f){
        discard;
    }
}