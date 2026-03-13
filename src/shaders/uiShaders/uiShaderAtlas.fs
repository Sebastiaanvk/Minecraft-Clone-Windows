#version 330 core
out vec4 FragColor;  

in vec2 TexCoord;

uniform sampler2D textureAtlas;
uniform vec4 tint; // The tints are floats in [0.0,1.0]
  
void main()
{
    // FragColor = texture(textureAtlas,TexCoord);
    FragColor = tint*texture(textureAtlas,TexCoord);
    // FragColor = vec4(1.0,0.0,0.0,1.0);
    // FragColor = vec4(TexCoord.x,TexCoord.y,0.0,1.0);
    // vec4 texColor = texture(textureAtlas, TexCoord);
    // FragColor = vec4(texColor.r, 0.0, 0.0, 1.0); // show only red channel
    if(FragColor.a<0.5){
        discard;
    }
}