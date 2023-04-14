#version 430 core

// Material properties
uniform vec3 mat_ambient;
uniform vec3 mat_diffuse;
uniform float mat_metalness;
uniform float mat_roughness;
uniform sampler2D textSampler;
uniform int apply_texture;

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
    vec3 F0 = vec3(0.04);
    vec3 F90 = vec3(1.0);
    vec3 F = mix(F0, F90, pow(1.0 - dot(-V, N), 5.0));
    vec3 specular;
    if (apply_texture == 1) {
        ambient = vec3(0.0, 0.0, 0.0);
        diffuse = max(dot(N, L), 0.0) * texture2D(textSampler, UV).rgb;
        specular = mix(F0, F, mat_metalness) * pow(max(dot(R, V), 0.0), 1.0 / mat_roughness);
    } else {
        ambient = mat_ambient;
        diffuse = max(dot(N, L), 0.0) * mat_diffuse;
        specular = mix(F0, F, mat_metalness) * pow(max(dot(R, V), 0.0), 1.0 / mat_roughness);
    }

    // Write final color to the framebuffer
    fragColor = vec4(ambient + diffuse + specular, 1.0);
}