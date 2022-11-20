#version 460 core

layout (location = 0) in vec3 vPosition;
layout (location = 2) in vec2 vTexturePosition;

out vec3 outColor;
out vec2 TexturePosition;

uniform mat4 MVP;

void main(){
     gl_Position = MVP * vec4(vPosition, 1.0f);
     outColor = vPosition;
     TexturePosition = vTexturePosition;
}