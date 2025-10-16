#include "Block.h"

#define GLCheckError() { GLenum err; while((err = glGetError()) != GL_NO_ERROR) \
std::cerr << "OpenGL error: " << err << " at line " << __LINE__ << std::endl; }

void Block::Initialize()
{
    if (vao) { vao->Delete(); delete vao; vao = nullptr; }
    if (vbo) { vbo->Delete(); delete vbo; vbo = nullptr; }
    if (ebo) { ebo->Delete(); delete ebo; ebo = nullptr; }

    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  m_rgb.x, m_rgb.y, m_rgb.z, 1.0f, // Bottom-left-back, red
         0.5f, -0.5f, -0.5f,  m_rgb.x, m_rgb.y, m_rgb.z, 1.0f, // Bottom-right-back, green
         0.5f,  0.5f, -0.5f,  m_rgb.x, m_rgb.y, m_rgb.z, 1.0f, // Top-right-back, blue
        -0.5f,  0.5f, -0.5f,  m_rgb.x, m_rgb.y, m_rgb.z, 1.0f, // Top-left-back, yellow
        -0.5f, -0.5f,  0.5f,  m_rgb.x, m_rgb.y, m_rgb.z, 1.0f, // Bottom-left-front, magenta
         0.5f, -0.5f,  0.5f,  m_rgb.x, m_rgb.y, m_rgb.z, 1.0f, // Bottom-right-front, cyan
         0.5f,  0.5f,  0.5f,  m_rgb.x, m_rgb.y, m_rgb.z, 1.0f, // Top-right-front, white
        -0.5f,  0.5f,  0.5f,  m_rgb.x, m_rgb.y, m_rgb.z, 1.0f  // Top-left-front, black
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

    if(vao && vao->supported) vao->Bind();
    vbo->Bind();
    ebo->Bind();

    if(vao->supported)
    {
	    vao->LinkVBO(*vbo, 0);
	    vao->LinkVBO(*vbo, 1);

	    vao->Unbind();
    }
    vbo->Unbind();
    ebo->Unbind();
}

void Block::Draw(Shader& shader)
{
    shader.Activate();
    if (vao && vao->supported) {
        vao->Bind();
    } else {
        vbo->Bind();
        ebo->Bind();

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    glm::mat4 model = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size));

    GLint modelLoc = glGetUniformLocation(shader.ID, "model");
    if (modelLoc != -1)
    	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    GLint colorLoc = glGetUniformLocation(shader.ID, "color");
    if (colorLoc != -1)
    	glUniform4f(colorLoc, m_rgb.x, m_rgb.y, m_rgb.z, 1.0f);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    if(vao && vao->supported) vao->Unbind();
}

glm::vec3 Block::GetRGB()
{
    return m_rgb;
}

void Block::SetRGB(glm::vec3 rgb)
{
    if (rgb != m_rgb)
    {
        m_rgb = rgb;
        Initialize();
    }
    
}

Block::~Block()
{
	if (vao) { vao->Delete(); delete vao; vao = nullptr; }
	if (vbo) { vbo->Delete(); delete vbo; vbo = nullptr; }
	if (ebo) { ebo->Delete(); delete ebo; ebo = nullptr; }
}
