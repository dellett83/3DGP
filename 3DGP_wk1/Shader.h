#include <GL/glew.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

struct Shader
{
	Shader(const std::string& _vertexshaderpath);
	Shader(const std::string& _fragmentshaderpath);
	~Shader();

	GLuint id();

private:
	GLuint m_id;
	GLuint f_id;
	GLuint v_id;

	std::string m_vertexsrc;
	std::string m_fragmentsrc;
};

#include <stdexcept>
inline Shader::Shader(const std::string& _vertexshaderpath)
: m_vertexsrc("")
{
	
	std::string vcurrentline;
	std::ifstream file(_vertexshaderpath.c_str());

	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open model [" + _vertexshaderpath + "]");
	}

	while (!file.eof())
	{
		std::getline(file, vcurrentline);
		if (vcurrentline.length() < 1) continue;
		m_vertexsrc += vcurrentline;
	}
}


inline Shader::Shader(const std::string& _fragmentshader)
	: m_fragmentsrc("")
{

	std::string vcurrentline;
	std::ifstream file(_fragmentshader.c_str());

	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open model [" + _fragmentshader + "]");
	}

	while (!file.eof())
	{
		std::getline(file, vcurrentline);
		if (vcurrentline.length() < 1) continue;
		m_vertexsrc += vcurrentline;
	}
}