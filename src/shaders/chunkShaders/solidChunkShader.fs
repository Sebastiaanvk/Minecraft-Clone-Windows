#version 330 core
out vec4 FragColor;  

in vec3 TexCoord;
in vec4 fragTint;
in float occlusion;

uniform sampler2DArray textureArray;
uniform bool ambientOcclusionFlag;
  
void main()
{
    FragColor = fragTint*texture(textureArray,TexCoord);
    if(ambientOcclusionFlag){
        FragColor = occlusion*FragColor;
    }
    // FragColor = occlusion*fragTint*texture(textureArray,TexCoord); // this also multiplies the a, but we dont do anything with it.
    // This will be maybe useful later if we have transparent textures.
    // if(FragColor.a<0.5f){
    //     discard;
    // }
}