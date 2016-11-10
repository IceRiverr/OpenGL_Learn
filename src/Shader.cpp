#include "Shader.h"

void BasicShaderVSParams::BindShader(GLuint& shaderProgram)
{
	modelMLoc = glGetUniformLocation(shaderProgram, "modelM");
	viewMLoc = glGetUniformLocation(shaderProgram, "viewM");
	projMLoc = glGetUniformLocation(shaderProgram, "projM");
}

void BasicShaderVSParams::SetVSMatrix(const glm::mat4& modelM, const glm::mat4& viewM, const glm::mat4 projM)
{
	glUniformMatrix4fv(modelMLoc, 1, GL_FALSE, glm::value_ptr(modelM));
	glUniformMatrix4fv(viewMLoc, 1, GL_FALSE, glm::value_ptr(viewM));
	glUniformMatrix4fv(projMLoc, 1, GL_FALSE, glm::value_ptr(projM));
}

void BasicShaderVSParams::SetModelMatrix(const glm::mat4& modelM)
{
	glUniformMatrix4fv(modelMLoc, 1, GL_FALSE, glm::value_ptr(modelM));
}

Shader::Shader()
{
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	// 1 open path 
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::stringstream vShaderStream;
	std::stringstream fShaderStream;

	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ." << std::endl;
	}

	//RemoveNotes(vertexCode);
	//RemoveNotes(fragmentCode);

	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	// 2 compile & link shader
	GLint success;
	GLchar infoLog[512];
	GLuint vertexShader;
	GLuint fragmentShader;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, &infoLog[0]);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, &infoLog[0]);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertexShader);
	glAttachShader(this->Program, fragmentShader);
	glLinkProgram(this->Program);
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->Program, 512, nullptr, &infoLog[0]);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	vsParams.BindShader(this->Program);
}

Shader::~Shader()
{
}

void Shader::Use()
{
	glUseProgram(this->Program);
}

void Shader::RemoveNotes(std::string& code)
{
	std::string resultText = "";
	std::string subLine;
	
	int lastNodeEnd = 0;
	int noteBegin = code.find_first_of("//", lastNodeEnd);
	while (noteBegin!= -1)
	{
		int tmp = code.find_first_of("//", noteBegin + 1);
		if (tmp != -1)
			break;

		noteBegin = code.find_first_of("//", lastNodeEnd);
	}

	int noteEnd = code.find_first_of("\n", noteBegin);

	while(noteBegin != -1 && noteEnd != -1 && lastNodeEnd != -1)
	{
		subLine = code.substr(lastNodeEnd, noteBegin - lastNodeEnd);
		resultText += subLine;
		lastNodeEnd = noteEnd;

		noteBegin = code.find_first_of("//", noteEnd);
		noteEnd = code.find_first_of("\n", noteBegin);
	}

	noteBegin = code.size();
	subLine = code.substr(lastNodeEnd, noteBegin - lastNodeEnd);
	resultText += subLine;

	code = resultText;

	//std::cout << resultText << std::endl;

	//int a = 10;
}

