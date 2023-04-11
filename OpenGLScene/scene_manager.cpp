#include "scene_manager.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "objloader.h"
#include "texture.h"
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "glsl.h"

scene_manager::scene_manager(camera *camera)
{
    this->camera_ = camera;
}

//------------------------------------------------------------
// Initializes the fragmentshader and vertexshader
//------------------------------------------------------------
void scene_manager::init_shaders()
{
	const char* vertexshader = glsl::readFile("vertexshader.vert");
	const GLuint vsh_id = glsl::makeVertexShader(vertexshader);

	const char* fragshader = glsl::readFile("fragmentshader.frag");
	const GLuint fsh_id = glsl::makeFragmentShader(fragshader);

    program_id = glsl::makeShaderProgram(vsh_id, fsh_id);
}

//------------------------------------------------------------
// Initializes the matrices.
//------------------------------------------------------------
void scene_manager::init_matrices()
{
    model[0] = glm::mat4();
    model[1] = glm::translate(glm::mat4(), glm::vec3(3.0, 2.5, 0.0));
    model[2] = glm::translate(glm::mat4(), glm::vec3(-3.0, -2.5, 0.0));

    for (int index = 0; index < objects_amount; ++index)
    {
        mv[index] = camera_->get_view() * model[index];
    }
}

//------------------------------------------------------------
// Initializes the objects and textures which will be used.
//------------------------------------------------------------
void scene_manager::init_objects()
{
	// Objects
	loadOBJ("Objects/teapot.obj", vertices[0], uvs[0], normals[0]);
    loadOBJ("Objects/torus.obj", vertices[1], uvs[1], normals[1]);
    loadOBJ("Objects/torus.obj", vertices[2], uvs[2], normals[2]);

    // Textures
    texture_id[0] = loadBMP("Textures/Yellobrk.bmp");
    texture_id[1] = loadBMP("Textures/uvtemplate.bmp");
}

//------------------------------------------------------------
// Initializes the materials and lightning.
//------------------------------------------------------------
void scene_manager::init_materials_light()
{
    light.position = glm::vec3(4.0, 4.0, 4.0);

    // Teapot
    materials[0].ambient_color = glm::vec3(0.0, 0.0, 0.0);
    materials[0].diffuse_color = glm::vec3(0.0, 0.0, 0.0);
    materials[0].specular_color = glm::vec3(1);
    materials[0].power = 1024;
    apply_texture[0] = true;

    // Torus, A.K.A. donut
    materials[1].ambient_color = glm::vec3(0.0, 0.0, 0.0);
    materials[1].diffuse_color = glm::vec3(0.0, 0.0, 0.0);
    materials[1].specular_color = glm::vec3(1);
    materials[1].power = 4;
    apply_texture[1] = true;

    // Box
    materials[2].ambient_color = glm::vec3(0.3, 0.3, 0.0);
    materials[2].diffuse_color = glm::vec3(0.5, 0.0, 0.5);
    materials[2].specular_color = glm::vec3(1);
    materials[2].power = 1024;
    apply_texture[2] = false;
}

//------------------------------------------------------------
// Allocates and fills buffers.
//------------------------------------------------------------
void scene_manager::init_buffers()
{
	GLuint vbo_vertices;
    GLuint vbo_normals;
    GLuint vbo_uvs;

    // Get vertex attributes
	const GLuint position_id = glGetAttribLocation(program_id, "position");
	const GLuint normal_id = glGetAttribLocation(program_id, "normal");
	const GLuint uv_id = glGetAttribLocation(program_id, "uv");

    for (int index = 0; index < objects_amount; ++index)
    {
        // vbo for vertices
        glGenBuffers(1, &vbo_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
        glBufferData(GL_ARRAY_BUFFER, vertices[index].size() * sizeof(glm::vec3), vertices[index].data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // vbo for normals
        glGenBuffers(1, &vbo_normals);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
        glBufferData(GL_ARRAY_BUFFER, normals[index].size() * sizeof(glm::vec3), normals[index].data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // vbo for uv
        glGenBuffers(1, &vbo_uvs);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_uvs);
        glBufferData(GL_ARRAY_BUFFER, uvs[index].size() * sizeof(glm::vec2), uvs[index].data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Allocate memory for vao
        glGenVertexArrays(1, &vao[index]);

        // Bind to vao
        glBindVertexArray(vao[index]);

        // Bind vertices to vao
        glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
        glVertexAttribPointer(position_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(position_id);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Bind normals to vao
        glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
        glVertexAttribPointer(normal_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(normal_id);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Bind uv to vao
        glBindBuffer(GL_ARRAY_BUFFER, vbo_uvs);
        glVertexAttribPointer(uv_id, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(uv_id);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Stop bind to vao
        glBindVertexArray(0);
    }

    // Make uniform vars
    uniform_mv = glGetUniformLocation(program_id, "mv");
	uniform_projection = glGetUniformLocation(program_id, "projection");
	uniform_light_pos = glGetUniformLocation(program_id, "light_pos");
    uniform_material_ambient = glGetUniformLocation(program_id, "mat_ambient");
    uniform_material_diffuse = glGetUniformLocation(program_id, "mat_diffuse");
    uniform_material_specular = glGetUniformLocation(program_id, "mat_specular");
    uniform_material_power = glGetUniformLocation(program_id, "mat_power");
    uniform_apply_texture = glGetUniformLocation(program_id, "apply_texture");

    // Attach to program (needed to fill uniform vars)
    glUseProgram(program_id);

    // Fill uniform vars
    glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(camera_->get_projection()));
    glUniform3fv(uniform_light_pos, 1, glm::value_ptr(light.position));
}

void scene_manager::init()
{
    init_shaders();
    init_matrices();
    init_objects();
    init_materials_light();
    init_buffers();
}

void scene_manager::render()
{
    // Define background
    glClearColor(0.0, 0.0, 0.0, 1.0);
    if (camera_->is_drone_mode_enabled)
    {
	    glClearColor(0.0, 0.0, 1.0, 1.0);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Attach to program_id
    glUseProgram(program_id);

    // Before doing anything, update the camera.
    camera_->update();
	glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(camera_->get_projection()));

    // Do transformation (x, y, z)
    model[0] = glm::rotate(model[0], 0.01f, glm::vec3(0.5f, 1.0f, 0.2f));
    model[1] = glm::rotate(model[1], 0.05f, glm::vec3(1.0f, 0.5f, 0.5f));
    model[2] = glm::rotate(model[2], 0.05f, glm::vec3(1.0f, 0.3f, 0.1f));

    for (int index = 0; index < objects_amount; index++)
    {
        // Do transformation
        mv[index] = camera_->get_view() * model[index];

        // Send mvp
        glUniformMatrix4fv(uniform_mv, 1, GL_FALSE, glm::value_ptr(mv[index]));

        // Bind textures
        glUniform1i(uniform_apply_texture, 0);
        if (apply_texture[index])
        {
            glUniform1i(uniform_apply_texture, 1);
            glBindTexture(GL_TEXTURE_2D, texture_id[index]);
        }

        // Fill uniform vars
        glUniform3fv(uniform_material_ambient, 1, glm::value_ptr(materials[index].ambient_color));
        glUniform3fv(uniform_material_diffuse, 1, glm::value_ptr(materials[index].diffuse_color));
        glUniform3fv(uniform_material_specular, 1, glm::value_ptr(materials[index].specular_color));
        glUniform1f(uniform_material_power, materials[index].power);

        // Send vao
        glBindVertexArray(vao[index]);
        glDrawArrays(GL_TRIANGLES, 0, vertices[index].size());
        glBindVertexArray(0);
    }

    // Swap buffers
    glutSwapBuffers();
}
