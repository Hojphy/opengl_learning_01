#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 color;

uniform mat4 model;
out vec4 fragColor;

void main()
{
   gl_Position = model * vec4(aPos, 1.0);
   fragColor = color;
}