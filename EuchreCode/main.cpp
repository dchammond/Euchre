//
//  main.cpp
//  Euchre
//
//  Created by Dillon Hammond on 9/12/14.
//  Copyright (c) 2014 Learning. All rights reserved.
//

#include "Euchre.h"

int main() {
	Euchre::MakeObject program;
	SDL_Window* window = program.createWindow("Euchre", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 800, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	// Create shaders
	GLuint bgShader = program.LoadShaders(
										  "/Users/Dillon/Desktop/Euchre/EuchreCode/Resources/vertexShaders/BGvertexShader.txt",
										  "/Users/Dillon/Desktop/Euchre/EuchreCode/Resources/fragmentShaders/BGfragmentShader.txt"
	);
	GLuint cardShader = program.LoadShaders(
											"/Users/Dillon/Desktop/Euchre/EuchreCode/Resources/vertexShaders/CardVertexShader.txt",
											"/Users/Dillon/Desktop/Euchre/EuchreCode/Resources/fragmentShaders/CardFragmentShader.txt"
	);

	// Create the vao
	GLsizei numOfVAO = 2;
	std::vector<GLuint> vertexArrayObject = program.makeVertexArrayObject(numOfVAO);
	glBindVertexArray(vertexArrayObject.at(0));

	// BEGIN set up the background
	auto BGbuffers = program.makeAllBuffers(true);

	GLuint BGpositionBuffer = std::get<0>(BGbuffers);
	GLuint BGcolorBuffer = std::get<1>(BGbuffers);
	GLuint BGtextureBuffer = std::get<2>(BGbuffers);
	GLuint BGelementBuffer = std::get<3>(BGbuffers);

	// Specify the layout of the vertex data
	program.makeAttribute(bgShader, "bgposition", GL_ARRAY_BUFFER, BGpositionBuffer, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Specify the color attributes
	program.makeAttribute(bgShader, "bgcolor", GL_ARRAY_BUFFER, BGcolorBuffer, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Specifiy the texture usage
	program.makeAttribute(bgShader, "bgcoord", GL_ARRAY_BUFFER, BGtextureBuffer, 2, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint bgTexture = program.LoadTexture("/Users/Dillon/Desktop/Euchre/EuchreCode/Resources/Background.png", 0); // Binds the background texture to the single number in vector textures
	// END set up background

	// BEGIN set up card
	glBindVertexArray(vertexArrayObject.at(1));

	auto CARDbuffers = program.makeAllBuffers(false);
	GLuint CARDpositionBuffer = std::get<0>(CARDbuffers);
	GLuint CARDcolorBuffer = std::get<1>(CARDbuffers);
	GLuint CARDtextureBuffer = std::get<2>(CARDbuffers);
	GLuint CARDelementBuffer = std::get<3>(CARDbuffers);

	program.makeAttribute(cardShader, "cardposition", GL_ARRAY_BUFFER, CARDpositionBuffer, 2, GL_FLOAT, GL_FALSE, 0, 0);

	program.makeAttribute(cardShader, "cardcolor", GL_ARRAY_BUFFER, CARDcolorBuffer, 3, GL_FLOAT, GL_FALSE, 0, 0);

	program.makeAttribute(cardShader, "cardcoord", GL_ARRAY_BUFFER, CARDtextureBuffer, 2, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint cardTexture = program.LoadTexture("/Users/Dillon/Desktop/Euchre/EuchreCode/Resources/king_of_hearts.png", 0);
	// END set up card
	
	SDL_Event windowEvent;
	while (true) {
		if (SDL_PollEvent(&windowEvent)) {
			if (windowEvent.type == SDL_QUIT) {
				break;
			}
		}

		// Background Shader
		glUseProgram(bgShader);
		glBindTexture(GL_TEXTURE_2D, bgTexture);

		// Clear the screen to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw background
		glBindVertexArray(vertexArrayObject.at(0));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Switch to Card Shader Program
		glUseProgram(cardShader);
		glBindTexture(GL_TEXTURE_2D, cardTexture);

		// Draw card
		glBindVertexArray(vertexArrayObject.at(1));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Update window
		SDL_GL_SwapWindow(window);
		
		glUseProgram(0);
		
		SDL_Delay(16); // 60 fps
	}

	glDeleteProgram(bgShader);
	glDeleteProgram(cardShader);

	// Delete all the buffers!
	glDeleteBuffers(1, &BGelementBuffer);
	glDeleteBuffers(1, &BGpositionBuffer);
	glDeleteBuffers(1, &BGcolorBuffer);
	glDeleteBuffers(1, &BGtextureBuffer);

	// Delete all the buffers!
	glDeleteBuffers(1, &CARDelementBuffer);
	glDeleteBuffers(1, &CARDpositionBuffer);
	glDeleteBuffers(1, &CARDcolorBuffer);
	glDeleteBuffers(1, &CARDtextureBuffer);


	glDeleteVertexArrays(numOfVAO, &vertexArrayObject.front());
	glDeleteTextures(1, &bgTexture);
	glDeleteTextures(1, &cardTexture);

	SDL_GL_DeleteContext(context);
	return 0;
}
