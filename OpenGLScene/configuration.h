#pragma once

//--------------------------------------------------------------------------------
// General settings.
//--------------------------------------------------------------------------------
constexpr int width = 800, height = 600;

unsigned constexpr int delta_time = 10;

//--------------------------------------------------------------------------------
// Typedefs
//--------------------------------------------------------------------------------
struct light_source
{
    glm::vec3 position;
};

struct material_struct
{
    glm::vec3 ambient_color;
    glm::vec3 diffuse_color;
    glm::vec3 specular_color;
    float power{};
};