#version 330 core
out vec4 FragColor;  

in vec3 TexCoord;
in vec4 fragTint;
in float occlusion;

uniform sampler2DArray textureArray;
uniform bool ambientOcclusionFlag;
uniform bool gammaCorrectionFlag;
  
void main()
{
    vec4 texColour = texture(textureArray,TexCoord);
    if(!gammaCorrectionFlag){
        texColour = pow(texColour,vec4(1.0/2.2,1.0/2.2,1.0/2.2,1.0));
    }
    FragColor = fragTint*texColour;
    if(ambientOcclusionFlag){
        FragColor = occlusion*FragColor;
    }
    if(!gammaCorrectionFlag){
        FragColor = pow(FragColor,vec4(2.2,2.2,2.2,1.0));
    }
    // FragColor = occlusion*fragTint*texture(textureArray,TexCoord); // this also multiplies the a, but we dont do anything with it.
    // This will be maybe useful later if we have transparent textures.
    // if(FragColor.a<0.5f){
    //     discard;
    // }
}