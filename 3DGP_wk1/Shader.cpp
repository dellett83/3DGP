#include "Shader.h"
#include "Mesh.h"

Shader::Shader()
	: m_id(0)
{}

void Shader::uniform(const std::string& _name, const glm::mat4& _value)
{
	GLint loc = glGetUniformLocation(id(), _name.c_str());

	glUseProgram(id());
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(_value));
	glUseProgram(0);
}

void Shader::draw(const Mesh& _mesh)
{
	glUseProgram(id());
	// glDrawArrays;
	glUseProgram(0);
}

GLuint Shader::id()
{
	if (!m_id)
	{
		const GLchar* vertexShaderSrc =
			"uniform mat4 u_Projection;				" \
			"uniform mat4 u_View;					" \
			"uniform mat4 u_Model;					" \
			"                                       " \
			"attribute vec3 a_Position;				" \
			"attribute vec2 a_TexCoord;				" \
			"                                       " \
			"varying vec2 v_TexCoord;				" \
			"                                       " \
			"void main()                            " \
			"{                                      " \
			" gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0); " \
			" v_TexCoord = a_TexCoord;                  " \
			"}                                      " \
			"                                       ";

		// create new vertex shader, compile, error check
		GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderId, 1, &vertexShaderSrc, NULL);
		glCompileShader(vertexShaderId);
		GLint success = 0;
		glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			throw std::exception();
		}

		// ------------------------------------------------------------------------------------------------------------------- writing fragment shader

		const GLchar* fragmentShaderSrc =
			"uniform sampler2D u_Texture;    " \
			"                          " \
			"varying vec2 v_TexCoord; " \
			"                          " \
			"void main()               " \
			"{                         " \
			" vec4 tex = texture2D(u_Texture, v_TexCoord); " \
			" gl_FragColor = tex; " \
			"}                         " \
			"                          ";

		//create frag shader, compile, error check

		GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderId, 1, &fragmentShaderSrc, NULL);
		glCompileShader(fragmentShaderId);
		glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			throw std::exception();
		}

		// ------------------------------------------------------------------------------------------------------------------- place the shader in position

		//create new shader program
		m_id = glCreateProgram();
		glAttachShader(m_id, vertexShaderId);
		glAttachShader(m_id, fragmentShaderId);

		//ensure vao pos sets first pos
		glBindAttribLocation(m_id, 0, "in_Position");

		glBindAttribLocation(m_id, 1, "a_Color");

		//perform link
		glLinkProgram(m_id);
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);

		//check link fail
		if (!success)
		{
			throw std::exception();
		}

		//find uniform locations
		GLint modelLoc = glGetUniformLocation(m_id, "u_Model");
		GLint viewLoc = glGetUniformLocation(m_id, "u_View");
		GLint projectionLoc = glGetUniformLocation(m_id, "u_Projection");

		//detach and destroy, shader was compiled
		glDetachShader(m_id, vertexShaderId);
		glDeleteShader(vertexShaderId);
		glDetachShader(m_id, fragmentShaderId);
		glDeleteShader(fragmentShaderId);
	}
	return m_id;
}