#include <SDL2/sdl.h>
#include <GL/glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Model.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#undef main

int main()
{
	Model cat("assets/models/curuthers/curuthers.obj");
	//Texture tex("assets/models/curuthers/Whiskers_diffuse.png");
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
	//image
	int w = 0;
	int h = 0;

	unsigned char* data = stbi_load("image.png", &w, &h, NULL, 4);

	if (!data)
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
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
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

	// ------------------------------------------------------------------------------------------------------------------- Picture loading

	GLuint textureId = 0;
	glGenTextures(1, &textureId);

	if (!textureId)
	{
		throw std::exception();
	}

	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	free(data);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

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
		"uniform mat4 u_Projection;				" \
		"uniform mat4 u_Model;					" \
		"                                       " \
		"attribute vec3 a_Position;" \
		"attribute vec2 a_TexCoord;" \
		"                                       " \
		"varying vec2 v_TexCoord;" \
		"                                       " \
		"void main()                            " \
		"{                                      " \
		" gl_Position = u_Projection * u_Model * vec4(a_Position, 1.0); " \
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

	//find uniform locations
	GLint modelLoc = glGetUniformLocation(programId, "u_Model");
	GLint projectionLoc = glGetUniformLocation(programId, "u_Projection");

	//detach and destroy, shader was compiled
	glDetachShader(programId, vertexShaderId);
	glDeleteShader(vertexShaderId);
	glDetachShader(programId, fragmentShaderId);
	glDeleteShader(fragmentShaderId);

	// ------------------------------------------------------------------------------------------------------------------- 

	float angle = 0.0f;
	int width = 0;
	int height = 0;

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
			switch (event.type)
			{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:  (angle -= 1.0f); break;
				case SDLK_RIGHT: (angle += 1.0f) ; break;
				}
				break;
			}
		}

		GLint uniformId = glGetUniformLocation(programId, "u_Texture");
		if (uniformId == -1)
		{
			throw std::exception();
		}

		

		SDL_GetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//tell gl to use our shader program

		glBindTexture(GL_TEXTURE_2D, textureId);
		glUseProgram(programId);
		glUseProgram(textureId);
		glUniform1i(uniformId, 1);
		glUseProgram(0);
		glBindVertexArray(cat.vao_id());

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);


		//prepare projection and model matrix
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0, 0, -2.5f));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));

	

		//upload model
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//upload projection
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//draw 3 verts
		glDrawArrays(GL_TRIANGLES, 0, cat.vertex_count());

		//Orthographic Projection

		projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.0f, 1.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(100, height - 100, 0));
		model = glm::scale(model, glm::vec3(100, 100, 1));

		//is program bound
		//upload model
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//upload projection
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// ------------------------------------------------------------------------------------------------------------------- draw it


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