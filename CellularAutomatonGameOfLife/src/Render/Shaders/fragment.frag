in vec2 TexCoords;
out vec4 color;

uniform usampler2D image;
uniform vec3 spriteColor;

void main()
{

    int data = int(texture(image, TexCoords).r);

    if(data == 0)
    {
        color = vec4(0, 0, 0, 0);
    }
    else
    {
        color = vec4(spriteColor, 1.0);
    }
    
} 