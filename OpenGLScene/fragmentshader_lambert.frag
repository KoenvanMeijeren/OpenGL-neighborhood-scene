#version 430 core

// Material properties
uniform vec3 mat_ambient;
uniform vec3 mat_diffuse;
uniform sampler2D textSampler;
uniform int apply_texture;
uniform float mat_metalness;

// Input from vertex shader
in VS_OUT
{
    vec3 N;
    vec3 L;
    vec3 V;
} fs_in;

in vec2 UV;

// Output color
out vec4 fragColor;

void main()
{
    // Normalize the incoming N, L and V vectors
    vec3 N = normalize(fs_in.N);
    vec3 L = normalize(fs_in.L);
    vec3 V = normalize(fs_in.V);

    // Calculate R locally
    vec3 R = reflect(-L, N);

    // Compute the diffuse and specular components for each fragment
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    if (apply_texture == 1) {
        ambient = vec3(0.0, 0.0, 0.0);
        diffuse = max(dot(N, L), 0.0) * texture2D(textSampler, UV).rgb;
    } else {
        ambient = mat_ambient;
        diffuse = max(dot(N, L), 0.0) * mat_diffuse;
    }
    
    // add metalness effect to specular
    specular = mix(vec3(0.04), mat_diffuse, mat_metalness);

    // Compute final color using the lighting equation
    vec3 result = mat_ambient + diffuse + specular;

    // Write final color to the framebuffer
    fragColor = vec4(result, 1.0);
}