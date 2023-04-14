#include "shader_primitive.h"

#include "glsl.h"

shader_primitive::shader_primitive()
: shader_primitive("vertexshader_primitive.vert", "fragmentshader_primitive.frag")
{
}

shader_primitive::shader_primitive(const char* vertex_shader_filename, const char* fragment_shader_filename)
{
	const char* vertexshader = glsl::readFile(vertex_shader_filename);
	const GLuint vsh_id = glsl::makeVertexShader(vertexshader);

	const char* fragshader = glsl::readFile(fragment_shader_filename);
	const GLuint fsh_id = glsl::makeFragmentShader(fragshader);

    program_id = glsl::makeShaderProgram(vsh_id, fsh_id);
}

void shader_primitive::init_buffers(const GLfloat vertices[], const GLsizeiptr vertices_size, const GLfloat colors[], const GLsizeiptr colors_size, const GLushort cube_elements[], const GLsizeiptr cube_elements_size)
{
	GLuint vbo_vertices;
    GLuint vbo_colors;
    GLuint ibo_elements;

    // vbo for vertices
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // vbo for colors
    glGenBuffers(1, &vbo_colors);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
    glBufferData(GL_ARRAY_BUFFER, colors_size, colors, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // vbo for elements
    glGenBuffers(1, &ibo_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube_elements_size,
        cube_elements, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Get vertex attributes
	const GLuint position_id = glGetAttribLocation(program_id, "position");
	const GLuint color_id = glGetAttribLocation(program_id, "color");

    // Allocate memory for vao
    glGenVertexArrays(1, &vao);

    // Bind to vao
    glBindVertexArray(vao);

    // Bind vertices to vao
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glVertexAttribPointer(position_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(position_id);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Bind colors to vao
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
    glVertexAttribPointer(color_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(color_id);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Bind elements to vao
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);

    // Stop bind to vao
    glBindVertexArray(0);

    // Make uniform vars
    uniform_mvp = glGetUniformLocation(program_id, "mvp");
}

void shader_primitive::enable() const
{
	glUseProgram(program_id);
}

void shader_primitive::fill_uniform_vars(const glm::mat4& mvp) const
{
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
}

void shader_primitive::send_vao(const GLushort cube_elements[], const GLsizei cube_elements_count) const
{
    glBindVertexArray(vao);
    glDrawElements(GL_LINES, cube_elements_count, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}

void shader_primitive::update(const glm::mat4& mvp, const GLushort cube_elements[], const GLsizei cube_elements_count) const
{
	enable();
    fill_uniform_vars(mvp);
    send_vao(cube_elements, cube_elements_count);
}
