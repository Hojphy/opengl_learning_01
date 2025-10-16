#include "shaderClass.h"

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return contents;
	}
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLint success;
	    char infoLog[1024];
	    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	    if (!success) {
		glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
		std::cerr << "Vertex shader compile error:\n" << infoLog << std::endl;
	    }

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
        std::cerr << "Fragment shader compile error:\n" << infoLog << std::endl;
    }
	ID = glCreateProgram();

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	glBindAttribLocation(ID, 0, "aPos");
	glBindAttribLocation(ID, 1, "color");

	glLinkProgram(ID);

	GLint success2;
	glGetProgramiv(ID, GL_LINK_STATUS, &success2);
	if (!success2) {
	    char infoLog[512];
	    glGetProgramInfoLog(ID, 512, NULL, infoLog);
	    std::cerr << "Shader linking failed:\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}
