in vec2 TexCoords;
out vec4 color;

uniform usampler2D image;
uniform vec3 spriteColor;

uniform vec2 spriteDimensions;
uniform float edgeThickness;
uniform vec3 edgeColor;

void main()
{

    int data = int(texture(image, TexCoords).r);

    if(edgeColor.r != 0)
    {
        vec2 spritePixel = TexCoords * spriteDimensions;
        vec2 edgeLimits = round(spritePixel);
        if(spritePixel.x <= (edgeLimits.x + edgeThickness) && spritePixel.x >= (edgeLimits.x - edgeThickness))
        {
            color = vec4(edgeColor, 1.0);
            return;
        }

        if(spritePixel.y <= (edgeLimits.y + edgeThickness) && spritePixel.y >= (edgeLimits.y - edgeThickness))
        {
            color = vec4(edgeColor, 1.0);
            return;
        }
    }
    
    if(data == 0)
    {
        color = vec4(0, 0, 0, 0);
    }
    else
    {
        color = vec4(spriteColor, 1.0);
    }
    
}