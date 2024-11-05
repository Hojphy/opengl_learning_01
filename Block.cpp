#include "Block.h"

void Block::Initialize()
{
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, // Bottom-left-back, red
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, // Bottom-right-back, green
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f, // Top-right-back, blue
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, // Top-left-back, yellow
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f, // Bottom-left-front, magenta
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f, // Bottom-right-front, cyan
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, // Top-right-front, white
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // Top-left-front, black
    };

    GLuint indices[] = {
        // Back face
        0, 1, 2,
        2, 3, 0,
        // Front face
        4, 5, 6,
        6, 7, 4,
        // Left face
        4, 0, 3,
        3, 7, 4,
        // Right face
        1, 5, 6,
        6, 2, 1,
        // Bottom face
        4, 5, 1,
        1, 0, 4,
        // Top face
        3, 2, 6,
        6, 7, 3
    };

    vao = new VAO();
    vbo = new VBO(vertices, sizeof(vertices));
    ebo = new EBO(indices, sizeof(indices));

    vao->Bind();
    vbo->Bind();
    ebo->Bind();

    vao->LinkVBO(*vbo, 0);
    vao->LinkVBO(*vbo, 1);

    vao->Unbind();
    vbo->Unbind();
    ebo->Unbind();
}

void Block::Draw(Shader& shader)
{
    shader.Activate();
    vao->Bind();

    glm::mat4 model = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size));

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform4f(glGetUniformLocation(shader.ID, "color"), rgb.x, rgb.y, rgb.z, 1.0f);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    vao->Unbind();
}

Block::Block(glm::vec3 position, float size, glm::vec3 rgb, BlockType blockType)
{
    this->position = position;
    this->size = size;
    this->rgb = rgb;
    this->blockType = blockType;
    damping = 0.99f;
    mass = 1.0f;
    gravity = glm::vec3(0, -2.81f, 0);
    bounciness = 0.8f;
    velocity = glm::vec3(0, 0,0);
    previousPosition = position;
}

Block::~Block()
{
    vao->Delete();
    vbo->Delete();
    ebo->Delete();
    delete vao;
    delete vbo;
    delete ebo;
}