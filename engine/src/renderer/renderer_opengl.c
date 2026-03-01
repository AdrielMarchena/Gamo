#include "glad/glad.h"

/* shader helpers */
GLuint compile(GLenum type, const char* src)
{
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, NULL);
    glCompileShader(s);
    return s;
}

GLuint make_shader(void)
{
    const char* vs = "#version 330 core\n"
                     "layout (location = 0) in vec3 aPos;\n"
                     "void main(){ gl_Position = vec4(aPos, 1.0); }\n";

    const char* fs = "#version 330 core\n"
                     "out vec4 FragColor;\n"
                     "void main(){ FragColor = vec4(0.2, 0.7, 0.3, 1.0); }\n";

    GLuint v = compile(GL_VERTEX_SHADER, vs);
    GLuint f = compile(GL_FRAGMENT_SHADER, fs);

    GLuint prog = glCreateProgram();
    glAttachShader(prog, v);
    glAttachShader(prog, f);
    glLinkProgram(prog);

    glDeleteShader(v);
    glDeleteShader(f);
    return prog;
}

static unsigned int gl_clear_flags = GL_COLOR_BUFFER_BIT;

static void check_gl_error(const char* file, int line)
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        fprintf(stderr, "OpenGL error in %s at line %d: %d\n", file, line, err);
    }
}

float vertices[] = {
    // Positions             // Texture Coords (U, V)
    -0.5f, -0.5f, 0.0f, // Bottom left corner (0)
    0.5f,  -0.5f, 0.0f, // Bottom right corner (1)
    0.5f,  0.5f,  0.0f, // Top right corner (2)
    -0.5f, 0.5f,  0.0f, // Top left corner (3)
};

unsigned int indices[] = {0, 1, 2, 2, 3, 0};

GLuint shader;

GLuint vao, vbo, ebo;
bool engine_renderer_init()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    shader = make_shader();
}

void engine_renderer_draw_quad(float xpos, float ypos, float width, float height)
{
    glBindVertexArray(vao);
    glUseProgram(shader);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void engine_renderer_set_clear_color(float red, float green, float blue, float alpha)
{
    glClearColor(red, green, blue, alpha);
    check_gl_error(__FILE__, __LINE__);
}

void engine_renderer_clear_color(void)
{
    glClear(gl_clear_flags);
    check_gl_error(__FILE__, __LINE__);
}
