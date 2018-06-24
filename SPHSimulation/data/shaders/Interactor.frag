#version 400

in vec2 gTexCoord;  // From the geometry shader

uniform sampler2D SpriteTex;
uniform vec3 Color;

out vec4 FragColor;

void main()
{
    FragColor = texture(SpriteTex, gTexCoord);
}