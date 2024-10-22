#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "RenderTexture.h"
#include "Model.h"

#include <exception>
#include <fstream>
#include <iostream>

Shader::Shader(const std::string& _fragpath, const std::string& _vertpath)
	:m_id(0)
	,v_id(0)
	,f_id(0)
	, m_vertsrc("")
	, m_fragsrc("")
	, m_dirty(false)
{
	std::ifstream vertfile(_vertpath);
	

	if (!vertfile.is_open())
	{
		throw std::exception();
	}

	std::string vertline;

	while (!vertfile.eof())
	{
		std::getline(vertfile, vertline);
		vertline += "\n";
		m_vertsrc += vertline;
	}

	std::ifstream fragfile(_fragpath);

	if (!fragfile.is_open())
	{
		throw std::exception();
	}

	std::string fragline;

	while (!fragfile.eof())
	{
		std::getline(fragfile, fragline);
		fragline += "\n";
		m_fragsrc += fragline;
	}

	m_dirty = true;
}

GLuint Shader::id()
{
	if (m_dirty)
	{
		GLuint v_id = glCreateShader(GL_VERTEX_SHADER);
		const GLchar* GLvertsrc = m_vertsrc.c_str();
		glShaderSource(v_id, 1, &GLvertsrc, NULL);
		glCompileShader(v_id);
		GLint success = 0;
		glGetShaderiv(v_id, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			throw std::exception();
		}

		GLuint f_id = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar* GLfragsrc = m_fragsrc.c_str();
		glShaderSource(f_id, 1, &GLfragsrc, NULL);
		glCompileShader(f_id);
		success = 0;
		glGetShaderiv(f_id, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			throw std::exception();
		}

		m_id = glCreateProgram();

		glAttachShader(m_id, v_id);
		glAttachShader(m_id, f_id);

		glLinkProgram(m_id);
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);

		if (!success)
		{
			throw std::exception();
		}

		glDetachShader(m_id, v_id);
		glDeleteShader(v_id);
		glDetachShader(m_id, f_id);
		glDeleteShader(f_id);

		m_dirty = false;

	}

	return m_id;
}

void Shader::uniform(const std::string& _name, const glm::mat4& value)
{

	GLint loc = glGetUniformLocation(id(), _name.c_str());

	glUseProgram(id());
	glUniformMatrix4fv(loc,1, GL_FALSE, glm::value_ptr(value));
	glUseProgram(0);
}

void Shader::draw(Mesh& _mesh, Texture& _tex)
{
	glUseProgram(m_id);
	glBindVertexArray(_mesh.vao_id());
	glBindTexture(GL_TEXTURE_2D, _tex.getId());
	glEnable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, _mesh.vertex_count());
	glUseProgram(0);
}

void Shader::draw(Mesh& _mesh, GLuint _texId)
{
	glUseProgram(m_id);
	glBindVertexArray(_mesh.vao_id());
	glBindTexture(GL_TEXTURE_2D, _texId);
	glEnable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, _mesh.vertex_count());
	glUseProgram(0);
}

void Shader::draw(Model& _model, Texture& _tex)
{
	glUseProgram(m_id);
	glBindVertexArray(_model.vao_id());
	glBindTexture(GL_TEXTURE_2D, _tex.getId());
	glEnable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, _model.vertex_count());
	glUseProgram(0);
}

void Shader::draw(Model& _model, GLuint _texid)
{
	glUseProgram(m_id);
	glBindVertexArray(_model.vao_id());
	glBindTexture(GL_TEXTURE_2D, _texid);
	glEnable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, _model.vertex_count());
	glUseProgram(0);
}
