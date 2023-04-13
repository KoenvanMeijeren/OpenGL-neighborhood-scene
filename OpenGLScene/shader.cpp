#include "shader.h"
#include "glsl.h"

shader::shader() = default;

shader::shader(const char* vertex_shader_filename, const char* fragment_shader_filename)
{
	const char* vertexshader = glsl::readFile(vertex_shader_filename);
	const GLuint vsh_id = glsl::makeVertexShader(vertexshader);

	const char* fragshader = glsl::readFile(fragment_shader_filename);
	const GLuint fsh_id = glsl::makeFragmentShader(fragshader);

    program_id_ = glsl::makeShaderProgram(vsh_id, fsh_id);
}

shader::~shader() = default;

void shader::init_buffers(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs)
{
    GLuint vbo_vertices;
    GLuint vbo_normals;
    GLuint vbo_uvs;

    // Get vertex attributes
	const GLuint position_id = glGetAttribLocation(program_id_, "position");
	const GLuint normal_id = glGetAttribLocation(program_id_, "normal");
	const GLuint uv_id = glGetAttribLocation(program_id_, "uv");

    // vbo for vertices
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // vbo for normals
    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // vbo for uv
    glGenBuffers(1, &vbo_uvs);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_uvs);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Allocate memory for vao
    glGenVertexArrays(1, &vao_);

    // Bind to vao
    glBindVertexArray(vao_);

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

    // Make uniform vars
    uniform_mv_ = glGetUniformLocation(program_id_, "mv");
	uniform_projection_ = glGetUniformLocation(program_id_, "projection");
	uniform_light_pos_ = glGetUniformLocation(program_id_, "light_pos");
    uniform_material_ambient_ = glGetUniformLocation(program_id_, "mat_ambient");
    uniform_material_diffuse_ = glGetUniformLocation(program_id_, "mat_diffuse");
    uniform_material_specular_ = glGetUniformLocation(program_id_, "mat_specular");
    uniform_material_power_ = glGetUniformLocation(program_id_, "mat_power");
}

void shader::enable()
{
	glUseProgram(program_id_);
}

void shader::fill_uniform_vars(const glm::mat4& model_view, const glm::mat4& projection, const light_source& light, const material& material)
{
	glUniformMatrix4fv(uniform_mv_, 1, GL_FALSE, glm::value_ptr(model_view));
    glUniformMatrix4fv(uniform_projection_, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(uniform_light_pos_, 1, glm::value_ptr(light.position));
	glBindTexture(GL_TEXTURE_2D, texture_id_);
    glUniform3fv(uniform_material_ambient_, 1, glm::value_ptr(material.ambient_color));
    glUniform3fv(uniform_material_diffuse_, 1, glm::value_ptr(material.diffuse_color));
    glUniform3fv(uniform_material_specular_, 1, glm::value_ptr(material.specular_color));
    glUniform1f(uniform_material_power_, material.power);
}

void shader::send_vao(const std::vector<glm::vec3>& vertices)
{
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

void shader::update(const glm::mat4& model_view, const glm::mat4& projection, const light_source& light,
	const material& material, const std::vector<glm::vec3>& vertices)
{
    enable();
    fill_uniform_vars(model_view, projection, light, material);
    send_vao(vertices);
}
