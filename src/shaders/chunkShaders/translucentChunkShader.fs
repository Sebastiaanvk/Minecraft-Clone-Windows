#version 330 core
out vec4 FragColor;  

in vec3 TexCoord;
in vec4 fragTint;

uniform sampler2DArray textureArray;
uniform bool gammaCorrectionFlag;
  
void main()
{
    // FragColor = vec4(0.0f,1.0f,0.0f,1.0f);
    FragColor = fragTint*texture(textureArray,TexCoord);
}