#include <SDL2/sdl.h>
#include <GL/glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Model.h"
#include "Texture.h"
#include "Shader.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#undef main

int main()
{
	Model cat("assets/models/curuthers/curuthers.obj");
	Texture tex("assets/models/curuthers/Whiskers_diffuse.png");
	Shader shader("vertexshader.txt", "fragmentshader.txt");
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

	// ------------------------------------------------------------------------------------------------------------------- 

	Shader vertexShader()

	float angle = 0.0f;
	int width = 0;
	int height = 0;
	glm::vec3 campos(0, 0, 0);
	glm::vec3 camrot(0, 0, 0);

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
				case SDLK_UP:  (campos.z -= 0.5); break;
				case SDLK_DOWN: (campos.z += 0.5); break;
				case SDLK_LEFT: (camrot.y += 0.5); break;
				case SDLK_RIGHT: (camrot.y -= 0.5); break;
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

		glBindTexture(GL_TEXTURE_2D, tex.TextureId());
		glUseProgram(programId);
		glBindVertexArray(cat.vao_id());

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);


		//prepare projection and model matrix
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0, 0, -12.5f));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
		angle += 1;

		//prepare view matrix
		glm::mat4 view(1.0f);
		view = glm::translate(view, campos);
		view = glm::rotate(view, glm::radians(camrot.y), glm::vec3(0, 1, 0));
		view = glm::inverse(view);

		//upload model
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//upload view
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

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