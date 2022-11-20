#version 460

in vec3 outColor;
in vec2 TexturePosition;
out vec4 color;

uniform sampler2D Texture;

void main()
{

	color = texture(Texture, TexturePosition);

}