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
//#include "Player.h"

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
	Shader shad2("assets/files/fragShader.txt", "assets/files/vertShader.txt");
	//RenderTexture
	RenderTexture rentex(256, 256);

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------- MESH AND FACES
	Mesh mesh;
	Face face;
	face.a.position = glm::vec3(0.0f, 0.5f, 0.0f);
	face.b.position = glm::vec3(0.0f, 0.0f, 0.0f);
	face.c.position = glm::vec3(0.5f, 0.5f, 0.0f);
	face.a.texcoord = glm::vec2(0.0f, 1.0f);
	face.b.texcoord = glm::vec2(0.0f, 0.0f);
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
	glm::vec3 movement(0, 0, -20.0f);
	glm::vec3 grassMovement(0, -4.0f, -20.0f);
	angle = 0.0f;


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
				case(SDLK_DOWN):	
					movement.y -= 1;
					break;
				case(SDLK_UP):
					movement.y += 1;
					break;
				case(SDLK_LEFT):
					movement.x -= 1;
					angle = 270.0f;
					break;
				case(SDLK_RIGHT):
					movement.x += 1;
					angle = 90.0f;
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
		//Prepare the perspective projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),(float)width / (float)height, 0.1f, 200.0f);
		//Prepare the model matrix
		glm::mat4 model(1.0f);
		model = glm::translate(model, movement);
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 90, 0));

		// Adjust camera position
		glm::vec3 campos(movement.x, movement.y, 0);

		//prepare view matrix
		glm::mat4 view(1.0f);
		view = glm::translate(view, campos);
		view = glm::rotate(view, glm::radians(camrot.y), glm::vec3(0, 1, 0));
		view = glm::inverse(view);
		// Increase the float angle so next frame the triangle rotates further



		//---------------------------------------------------------------------------------------------------------------------------------------------------------------- GRASS
		glm::mat4 grassModel(1.0f);
		grassModel = glm::translate(grassModel, grassMovement);

		//---------------------------------------------------------------------------------------------------------------------------------------------------------------- DRAWING ONTO SCREEN
		// Make sure the current program is bound
		glClearColor(0.0f,0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		// Instruct OpenGL to use our shader program and our VAO for the cat
		shad.uniform("u_Model", model);
		shad.uniform("u_Projection", projection);
		shad.uniform("u_View", view);
		shad.draw(cat, cattex, rentex);
		shad.draw(cat, cattex);

		// Instruct OpenGL to use our shader program and our VAO for the grass
		shad2.uniform("u_Model", grassModel);
		shad2.uniform("u_Projection", projection);
		shad2.uniform("u_View", view);
		shad2.draw(grass, grasstex, rentex);
		shad2.draw(grass, grasstex);


		//--------------------------------------------------------------------------
		glDisable(GL_CULL_FACE);


		//---------------------------------------------------------------------------------------------------------------------------------------------------------------- DRAWING ONTO SCREEN ORTHOGRAPHIC
		// Prepare the orthographic projection matrix (reusing the variable)
		projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.0f, 1.0f);

		// Prepare model matrix. The scale is important because now our triangle would be the size of a single pixel when mapped to an orthographic projection.
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(100, height - 200, 0));
		model = glm::scale(model, glm::vec3(200, 200, 1));
		shad.uniform("u_Model", model);
		shad.uniform("u_Projection", projection);
		shad.uniform("u_View", glm::mat4(1.0f));
		glUseProgram(shad.id());
		shad.draw(mesh,orthotex);
		shad.draw(mesh2, orthotex2);

		//---------------------------------------------------------------------------------------------------------------------------------------------------------------- BIND VERTEX ARRAY
		// Reset the state
		glBindVertexArray(0);
		glUseProgram(0);

		SDL_GL_SwapWindow(window);
	}



	return 0;
}