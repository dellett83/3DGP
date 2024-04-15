#include "Shader.h"

#include <exception>


// ------------------------------------------------------------------------------------------------------------------- writing shader program

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

// ------------------------------------------------------------------------------------------------------------------- shader program object



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
glAttachShader(programId, vertexShaderId);
glAttachShader(programId, fragmentShaderId);




//ensure vao pos sets first pos
glBindAttribLocation(programId, 0, "in_Position");

glBindAttribLocation(programId, 1, "a_Color");

//perform link
glLinkProgram(programId);
glGetProgramiv(programId, GL_LINK_STATUS, &success);

//check link fail
if (!success)
{
	throw std::exception();
}

//find uniform locations
GLint modelLoc = glGetUniformLocation(programId, "u_Model");
GLint viewLoc = glGetUniformLocation(programId, "u_View");
GLint projectionLoc = glGetUniformLocation(programId, "u_Projection");

//detach and destroy, shader was compiled
glDetachShader(programId, vertexShaderId);
glDeleteShader(vertexShaderId);
glDetachShader(programId, fragmentShaderId);
glDeleteShader(fragmentShaderId);

// ------------------------------------------------------------------------------------------------------------------- 