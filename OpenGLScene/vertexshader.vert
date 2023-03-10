#version 430 core

// Inputs
in vec4 position;
in vec4 color;

// Outputs
out vec4 vColor;

void main()
{
    gl_Position = position;
    vColor = color;
}
