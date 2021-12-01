#version 450 core
in vec4 ourColor;
in vec2 TexCoord;

out vec4 color;

// pixels da textura
uniform sampler2D texsn2;

void main()
{
    color = texture(texsn2, TexCoord);
}
