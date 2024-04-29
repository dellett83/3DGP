#include <SDL2/sdl.h>
#include <GL/glew.h>
#include <cmath>
#include <iostream>
#include <GLM/glm.hpp>
#include <GLM/ext.hpp>


#include "Texture.h"
#include "Mesh.h"
#include "Model.h"
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "contrib/include/stb_image.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#undef main

int main()
{

	bool quit = false;
	const GLfloat positions[] = {
		0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,

	};

	const GLfloat colors[] = {
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	 0.0f, 0.0f

	};

	float angle = 0;
	int width = 0;
	int height = 0;


//#----------------------------------------------------------------------------------------------------------------
	SDL_Window* window = SDL_CreateWindow("Triangle",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
//#----------------------------------------------------------------------------------------------------------------	
	if (!SDL_GL_CreateContext(window))
	{
		throw std::exception();
	}

	if (glewInit() != GLEW_OK)
	{
		throw std::exception();
	}
//#----------------------------------------------------------------------------------------------------------------
//	GLuint positionsVboId = 0;
//
//	// Create a new VBO on the GPU and bind it
//	glGenBuffers(1, &positionsVboId);
//
//	if (!positionsVboId)
//	{
//		throw std::exception();
//	}
//
//	glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);
//
//	// Upload a copy of the data from memory into the new VBO
//	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
//
//	// Reset the state
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
////#----------------------------------------------------------------------------------------------------------------
//	GLuint colorsVboId = 0;
//	 
//	//Create a Colours VBO on the gpu and bind it 
//	glGenBuffers(1, &colorsVboId);
//
//	if (!colorsVboId)
//	{
//		throw std::exception();
//	}
//
//	glBindBuffer(GL_ARRAY_BUFFER, colorsVboId);
//
//	//upload a copy of the data from memory intp the new VBO
//	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//
////#----------------------------------------------------------------------------------------------------------------
//	GLuint vaoId = 0;
//
//	// Create a new VAO on the GPU and bind it
//	glGenVertexArrays(1, &vaoId);
//
//	if (!vaoId)
//	{
//		throw std::exception();
//	}
//
//	glBindVertexArray(vaoId);
//
//	// Bind the position VBO, assign it to position 0 on the bound VAO
//	// and flag it to be used
//	glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);
//
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
//		3 * sizeof(GLfloat), (void*)0);
//
//	glEnableVertexAttribArray(0);
//
//	// Bind the color VBO, assign it to position 1 on the bound VAO
//   // and flag it to be used
//	glBindBuffer(GL_ARRAY_BUFFER, colorsVboId);
//
//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
//
//	glEnableVertexAttribArray(1);
//
//	// Reset the state
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//#----------------------------------------------------------------------------
	Model Cat("assets/models/curuthers/curuthers.obj");
	Texture tex("assets/models/curuthers/Whiskers_diffuse.png");

	Shader Shader("assets/files/fragShader.txt", "assets/files/vertShader.txt");

	


	Mesh mesh;
	Face face;
	face.a.positions = glm::vec3(0.0f, 0.5f, 0.0f);
	face.b.positions = glm::vec3(-0.5f, -0.5f, 0.0f);
	face.c.positions = glm::vec3(0.5f, -0.5f, 0.0f);
	face.a.texcoords = glm::vec2(0.0f, 0.0f);
	face.b.texcoords = glm::vec2(0.0f, 1.0f);
	face.c.texcoords = glm::vec2(1.0f, 1.0f);
	mesh.addFace(face);



	glm::vec3 campos(0, 0, 0);
	glm::vec3 camrot(0, 0, 0);

	while (!quit)
	{
		SDL_Event event = {};


		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case(SDLK_DOWN):	
					campos.z += 1;
					break;
				case(SDLK_UP):
					campos.z -= 1;
					break;
				case(SDLK_LEFT):
					camrot.y += 1;
					break;
				case(SDLK_RIGHT):
					camrot.y -= 1;
					break;
			

				}
					
			}
		}
		SDL_GetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);


		//Prepare the perspective projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)width / (float)height, 0.1f, 100.0f);

		//Prepare the model matrix
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0, 0, -10.0f));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));

		//prepare view matrix
		glm::mat4 view(1.0f);
		view = glm::translate(view,campos);
		view = glm::rotate(view, glm::radians(camrot.y), glm::vec3(0, 1, 0));
		view = glm::inverse(view);

		// Increase the float angle so next frame the triangle rotates further
		angle += 1.0f;

		// Make sure the current program is bound


		glClearColor(0.0f,0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		// Instruct OpenGL to use our shader program and our VAO
		

		Shader.uniform("u_Model", model);
		Shader.uniform("u_Projection", projection);
		Shader.uniform("u_View", view);

		glUseProgram(Shader.id());
		glBindVertexArray(Cat.vao_id());
		glBindTexture(GL_TEXTURE_2D, tex.getId());
		glEnable(GL_DEPTH_TEST);
		// Draw 3 vertices (a triangle)
		glDrawArrays(GL_TRIANGLES, 0, Cat.vertex_count());
		glDisable(GL_CULL_FACE);

		// Prepare the orthographic projection matrix (reusing the variable)
		projection = glm::ortho(0.0f, (float)width, 0.0f,
			(float)WINDOW_HEIGHT, 0.0f, 1.0f);

		// Prepare model matrix. The scale is important because now our triangle
		// would be the size of a single pixel when mapped to an orthographic
		// projection.
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(100, height - 100, 0));
		model = glm::scale(model, glm::vec3(100, 100, 1));

		Shader.uniform("u_Model", model);
		Shader.uniform("u_Projection", projection);

		glUseProgram(Shader.id());
		//draw part 2
		glDrawArrays(GL_TRIANGLES, 0, Cat.vertex_count());

		// Reset the state
		glBindVertexArray(0);
		glUseProgram(0);

		SDL_GL_SwapWindow(window);
	}



	return 0;
}