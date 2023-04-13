#include "shader.h"
#include "glsl.h"

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
    shader::enable();

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
}

void shader::enable()
{
	glUseProgram(program_id_);
}

void shader::send_vao(const std::vector<glm::vec3>& vertices)
{
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}
