#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <stdexcept>

struct Mesh;

struct Shader
{
	Shader();

	GLuint id();
	void bind();
	void unbind();

	Shader(const std::string& _vertexshaderpath);
	Shader(const std::string& _fragmentshaderpath);
	~Shader();

	void uniform(const std::string& _name, const glm::mat4& _value);
	void draw(const Mesh& _mesh);

private:
	GLuint m_id;
	GLuint f_id;
	GLuint v_id;

	std::string m_vertexsrc;
	std::string m_fragmentsrc;
	bool m_dirty;
};

inline Shader::Shader(const std::string& _vertexshaderpath)
	: m_id(0)
	, f_id(0)
	, v_id(0)
	, m_vertexsrc("")
	, m_dirty(false)
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
	: m_id(0)
	, f_id(0)
	, v_id(0)
	, m_fragmentsrc("")
	, m_dirty(false)
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
		m_fragmentsrc += vcurrentline;
	}
}