#include <SDL2/sdl.h>
#include <GL/glew.h>
#include <cmath>
#include <iostream>
#include <GLM/glm.hpp>
#include <GLM/ext.hpp>
#include <stdlib.h> 
#include "Texture.h"
#include "Mesh.h"
#include "Model.h"
#include "Shader.h"
#include "RenderTexture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "contrib/include/stb_image.h"
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#undef main

int main()
{
	bool quit = false;

	float angle = 0;
	int width = 0;
	int height = 0;

	//#----------------------------------------------------------------------------------------------------------------

	SDL_Window* window = SDL_CreateWindow("3DGP ",
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

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------- OBJECTS
	//Model
	Model cat("assets/models/curuthers/cat.obj");
	Model grass("assets/models/grass/grass.obj");
	//Texture
	Texture cattex("assets/models/curuthers/cat.png");
	Texture grasstex("assets/models/grass/grass.png");
	Texture orthotex("assets/sample.png");
	Texture orthotex2("assets/sample2.png");
	//Shader
	Shader shad("assets/files/fragShader.txt", "assets/files/vertShader.txt");
	Shader shad2("assets/files/fragShader2.txt", "assets/files/vertShader2.txt");

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------- MESH AND FACES
	Mesh mesh;
	Face face;
	face.a.position = glm::vec3(0.0f, 0.0f, 0.0f);
	face.b.position = glm::vec3(0.0f, 0.5f, 0.0f);
	face.c.position = glm::vec3(0.5f, 0.5f, 0.0f);
	face.a.texcoord = glm::vec2(0.0f, 0.0f);
	face.b.texcoord = glm::vec2(0.0f, 1.0f);
	face.c.texcoord = glm::vec2(1.0f, 1.0f);
	mesh.addFace(face);
	Mesh mesh2;
	Face face2;
	face2.a.position = glm::vec3(0.5f, 0.0f, 0.0f);
	face2.b.position = glm::vec3(0.0f, 0.0f, 0.0f);
	face2.c.position = glm::vec3(0.5f, 0.5f, 0.0f);
	face2.a.texcoord = glm::vec2(0.0f, 1.0f);
	face2.b.texcoord = glm::vec2(0.0f, 0.0f);
	face2.c.texcoord = glm::vec2(1.0f, 1.0f);
	mesh2.addFace(face2);

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------- VARIABLES
	
	glm::vec3 camrot(0, 0, 0);
	glm::vec3 movement(0.0f, 0.0f, -20.0f);
	glm::vec3 blockPos(6.0f, -4.8f, -20.0f);
	int lives = 1;
	float velocity = 0;
	bool inAir = false;


	while (!quit)
	{
		SDL_Event event = {};
		width = 0;
		height = 0;

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
				case(SDLK_UP):
					if (inAir == false)
					{
						velocity = 0.5;
						inAir = true;
					}
					break;
				case(SDLK_LEFT):
					if (movement.x > -15)
					{
						movement.x -= 1;
						angle = 270.0f;
					}
					break;
				case(SDLK_RIGHT):
					if (movement.x < 15)
					{
						movement.x += 1;
						angle = 90.0f;
					}
					break;
				case(SDLK_i):
					movement.z -= 1;
					break;
				case(SDLK_o):
					movement.z += 1;
					break;
				}
			}
		}
		SDL_GetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);

		//---------------------------------------------------------------------------------------------------------------------------------------------------------------- PLAYER	
		
		if (movement.y > -4.8)
		{
			velocity -= 0.01f;
		}
		else if (movement.y < -4.801)
		{
			inAir = false;
			velocity = 0;
			movement.y = -4.8;
		}
		movement.y += velocity;

		//Prepare the perspective projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 200.0f);
		//Prepare the model matrix
		glm::mat4 model(1.0f);
		model = glm::translate(model, movement);
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 90, 0));

		// Adjust camera position
		glm::vec3 campos(0, 0, 0);

		//prepare view matrix
		glm::mat4 view(1.0f);
		view = glm::translate(view, campos);
		view = glm::rotate(view, glm::radians(camrot.y), glm::vec3(0, 1, 0));
		view = glm::inverse(view);

		//---------------------------------------------------------------------------------------------------------------------------------------------------------------- DRAWING ONTO SCREEN
		//---------------------------------------------------------------------------------------------------------------------------------------------------------------- PLAYER
	
		// Make sure the current program is bound
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Instruct OpenGL to use our shader program and our VAO for the cat
		shad.uniform("u_Model", model);
		shad.uniform("u_Projection", projection);
		shad.uniform("u_View", view);
		shad.draw(cat, cattex);


		//---------------------------------------------------------------------------------------------------------------------------------------------------------------- GRASS
		// Instruct OpenGL to use our shader program and our VAO for the grass
		glm::mat4 grassModel(1.0f);
		grassModel = glm::translate(grassModel, glm::vec3(-14.0f, -9.0f, -20.0f));
		grassModel = glm::scale(grassModel, glm::vec3(30, 1, 1));

		shad.uniform("u_Model", grassModel);
		shad.uniform("u_Projection", projection);
		shad.uniform("u_View", view);

		shad.draw(grass, grasstex);


		//---------------------------------------------------------------------------------------------------------------------------------------------------------------- OBSTACLE
		glm::mat4 grassModel2(1.0f);
		grassModel2 = glm::translate(grassModel2, glm::vec3(blockPos));

		blockPos.x += 0.1;
		if (blockPos.x > 15)
		{
			blockPos.x = -15.0f;
		}
		

		shad.uniform("u_Model", grassModel2);
		shad.uniform("u_Projection", projection);
		shad.uniform("u_View", view);

		shad.draw(grass, grasstex);
		
		//--------------------------------------------------------------------------
		glDisable(GL_CULL_FACE);


		//---------------------------------------------------------------------------------------------------------------------------------------------------------------- DRAWING ONTO SCREEN ORTHOGRAPHIC
		// Prepare the orthographic projection matrix (reusing the variable)
		projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.0f, 1.0f);


		// Prepare model matrix. The scale is important because now our triangle would be the size of a single pixel when mapped to an orthographic projection.
		for (int l(0); l < lives + 1; l++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(100, height - 200, 0));
			model = glm::scale(model, glm::vec3(200, 200, 1));
			shad2.uniform("u_Model", model);
			shad2.uniform("u_Projection", projection);
			shad2.uniform("u_View", glm::mat4(1.0f));
			glUseProgram(shad.id());

			shad2.draw(mesh, orthotex);
			shad2.draw(mesh2, orthotex2);
			if (blockPos.x > movement.x - 0.5 && blockPos.x < movement.x + 0.5)
			{
				if (blockPos.y == movement.y)
				{
					lives -= 1;
				}
			}
		}
		if (lives == 0)
		{
			exit(0);
		}
		//---------------------------------------------------------------------------------------------------------------------------------------------------------------- BIND VERTEX ARRAY
		// Reset the state
		glBindVertexArray(0);
		glUseProgram(0);

		SDL_GL_SwapWindow(window);
	}
	return 0;
}