in vec2 TexCoords;
out vec4 color;

uniform usampler2D image;
uniform vec3 spriteColor;

void main()
{    

    uint data = texture(image, TexCoords).r;
    float rC = clamp(data,0.0,1.0);

    color = vec4(rC, 0.0, 0.0, 1.0);
} 