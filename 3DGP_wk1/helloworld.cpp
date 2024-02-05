#include <SDL2/sdl.h>
#include <GL/glew.h>
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#undef main

int main()
{
	std::cout << "hello world" << std::endl;

	SDL_Window* window = SDL_CreateWindow("Triangle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (!SDL_GL_CreateContext(window))
	{
		throw std::exception();
	}
	if (glewInit() != GLEW_OK)
	{
		throw std::exception();
	}
	// ------------------------------------------------------------------------------------------------------------------- Triangle positions
	const GLfloat positions[] =
	{
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
	};

	GLuint positionsVboId = 0;

	//creating new vbo on the gpu
	glGenBuffers(1, &positionsVboId);

	if (!positionsVboId)
	{
		throw std::exception();
	}

	glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);

	//upload copy of data to vbo from memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	//reset state
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// ------------------------------------------------------------------------------------------------------------------- Triangle colours
	const GLfloat colors[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
	};

	GLuint colorsVboId = 0;

	//creating new vbo on the gpu
	glGenBuffers(1, &colorsVboId);

	if (!colorsVboId)
	{
		throw std::exception();
	}

	glBindBuffer(GL_ARRAY_BUFFER, colorsVboId);

	//upload copy of data to vbo from memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	//reset state
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// ------------------------------------------------------------------------------------------------------------------- creating vao

	GLuint vaoId = 0;

	// create a new vao
	glGenVertexArrays(1, &vaoId);

	if (!vaoId)
	{
		throw std::exception();
	}

	glBindVertexArray(vaoId);

	//bind vbo pos, assign pos 0 on bound vao, flag it to be used
	glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	//bind vbo pos, assign pos 1 on bound vao, flag it to be used
	glBindBuffer(GL_ARRAY_BUFFER, colorsVboId);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), (void*)0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//reset state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// ------------------------------------------------------------------------------------------------------------------- writing shader program
	
	const GLchar* vertexShaderSrc =
		"attribute vec3 a_Position;            " \
		"attribute vec4 a_Color;               " \
		"                                       " \
		"varying vec4 v_Color;                 " \
		"                                       " \
		"void main()                            " \
		"{                                      " \
		" gl_Position = vec4(a_Position, 1.0); " \
		" v_Color = a_Color;                  " \
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
		"varying vec4 v_Color;    " \
		"                          " \
		"void main()               " \
		"{                         " \
		" gl_FragColor = v_Color; " \
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
	GLuint programId = glCreateProgram();
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

	//detach and destroy, shader was compiled
	glDetachShader(programId, vertexShaderId);
	glDeleteShader(vertexShaderId);
	glDetachShader(programId, fragmentShaderId);
	glDeleteShader(fragmentShaderId);

	// ------------------------------------------------------------------------------------------------------------------- 

	bool quit = false;
	while (!quit)
	{
		SDL_Event event = {};

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// ------------------------------------------------------------------------------------------------------------------- draw it

		//tell gl to use our shader program
		glUseProgram(programId);
		glBindVertexArray(vaoId);

		//draw 3 verts
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//reset state
		glBindVertexArray(0);
		glUseProgram(0);

		// -------------------------------------------------------------------------------------------------------------------

		SDL_GL_SwapWindow(window);
	}
	return 0;
}