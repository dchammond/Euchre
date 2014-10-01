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
	SDL_Window* window = program.createWindow("Euchre", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	glewInit();
	
	GLuint shaderProgram = glCreateProgram();
	
	// Create the vao
	GLsizei numOfVAO = 2;
	std::vector<GLuint> vertexArrayObject = program.makeVertexArrayObject(numOfVAO);
	glBindVertexArray(vertexArrayObject.at(0));
	
	// BEGIN set up the background
	bool bg = true;
	auto BGbuffers = program.makeAllBuffers(bg);
	
	GLuint BGpositionBuffer = std::get<0>(BGbuffers);
	GLuint BGcolorBuffer = std::get<1>(BGbuffers);
	GLuint BGtextureBuffer = std::get<2>(BGbuffers);
	GLuint BGelementBuffer = std::get<3>(BGbuffers);
	
	program.LoadShaders(shaderProgram, "./Resources/vertexShaders/BGvertexShader.txt", "./Resources/fragmentShaders/BGfragmentShader.txt");
	
	// Specify the layout of the vertex data
	program.makeAttribute(shaderProgram, "bgposition", GL_ARRAY_BUFFER, BGpositionBuffer, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Specify the color attributes
	program.makeAttribute(shaderProgram, "bgcolor", GL_ARRAY_BUFFER, BGcolorBuffer, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Specifiy the texture usage
	program.makeAttribute(shaderProgram, "bgcoord", GL_ARRAY_BUFFER, BGtextureBuffer, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	std::vector<GLuint> textures(1,0); // Creates vector with one copy of a zero
	program.LoadTextures(textures, "./Resources/Background.png", "backGround", shaderProgram, 0); // Binds the background texture to the single number in vector textures
	// END set up background
	// BEGIN set up card
/*	glBindVertexArray(vertexArrayObject.at(1));
	bg = false;
	
	auto CARDbuffers = program.makeAllBuffers(bg);
	GLuint CARDpositionBuffers = std::get<0>(CARDbuffers);
	GLuint CARDcolorBuffer = std::get<1>(CARDbuffers);
	GLuint CARDtextureBuffer = std::get<2>(CARDbuffers);
	GLuint CARDelementBuffer = std::get<3>(CARDbuffers);

	program.LoadShaders(shaderProgram, "./Resources/vertexShaders/CardVertexShader.txt", "./Resources/fragmentShaders/CardFragmentShader.txt");
	
	program.makeAttribute(shaderProgram, "cardposition", GL_ARRAY_BUFFER, CARDpositionBuffers, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	program.makeAttribute(shaderProgram, "cardcolor", GL_ARRAY_BUFFER, CARDcolorBuffer, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	program.makeAttribute(shaderProgram, "cardcoord", GL_ARRAY_BUFFER, CARDtextureBuffer, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	std::vector<GLuint> tex(1,0);
	program.LoadTextures(tex, "./Resources/king_of_hearts.png", "card", shaderProgram, 0);
	*/
	SDL_Event windowEvent;
	while (true) {
		if (SDL_PollEvent(&windowEvent)) {
			if (windowEvent.type == SDL_QUIT) {
				break;
			}
		}
		
		glUseProgram(shaderProgram);
		
		// Clear the screen to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glBindVertexArray(vertexArrayObject.at(0));
		
		// Draw a rectangle from the 2 triangles using 6 indices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glBindVertexArray(vertexArrayObject.at(1));
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		// Update window
		SDL_GL_SwapWindow(window);
		
		glUseProgram(0);
	}
/*	glDeleteTextures(static_cast<GLsizei>(textures.size()), &textures.front()); // Casted to remove warning about precision loss (this doesn't matter)
	
	glDeleteProgram(shaderProgram);
	
	// Delete all the buffers!
	glDeleteBuffers(1, &BGelementBuffer);
	glDeleteBuffers(1, &BGpositionBuffer);
	glDeleteBuffers(1, &BGcolorBuffer);
	glDeleteBuffers(1, &BGtextureBuffer);
*/
	glDeleteVertexArrays(numOfVAO, &vertexArrayObject.front());
	
	SDL_GL_DeleteContext(context);
	return 0;
}
