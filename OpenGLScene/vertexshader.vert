#version 430 core

// Uniform inputs
uniform mat4 mvp;

// Per-vertex inputs
in vec3 position;

void main()
{
    gl_Position = mvp * vec4(position, 1.0);
}
