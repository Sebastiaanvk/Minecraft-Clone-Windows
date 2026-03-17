#version 330 core
out vec4 FragColor;  

in vec3 TexCoord;
in vec4 fragTint;

uniform sampler2DArray textureArray;
uniform bool gammaCorrectionFlag;
  
void main()
{
    // FragColor = vec4(0.0f,1.0f,0.0f,1.0f);
    vec4 texColour = texture(textureArray,TexCoord);
    if(!gammaCorrectionFlag){
        texColour = pow(texColour,vec4(1.0/2.2,1.0/2.2,1.0/2.2,1.0));
    }
    FragColor = fragTint*texColour;
    if(!gammaCorrectionFlag){
        FragColor = pow(FragColor,vec4(2.2,2.2,2.2,1.0));
    }
}