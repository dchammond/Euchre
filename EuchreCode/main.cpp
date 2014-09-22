//
//  main.cpp
//  Euchre
//
//  Created by Dillon Hammond on 9/12/14.
//  Copyright (c) 2014 Learning. All rights reserved.
//

#include "Euchre.h"

int main() {
	Euchre::MakeObject bg;
	SDL_Window* window = bg.createWindow("Euchre", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	glewInit();
	
	auto buffers = bg.makeAllbgBuffers();
	
	GLuint vertexArrayObject = std::get<0>(buffers);
	GLuint BGpositionBuffer = std::get<1>(buffers);
	GLuint BGcolorBuffer = std::get<2>(buffers);
	GLuint BGtextureBuffer = std::get<3>(buffers);
	GLuint BGelementBuffer = std::get<4>(buffers);
	
	GLuint shaderProgram = bg.LoadShaders("./Resources/vertexShaders/BGvertexShader.txt", "./Resources/fragmentShaders/BGfragmentShader.txt");
	glUseProgram(shaderProgram);
	
	// Specify the layout of the vertex data
	bg.makeAttribute(shaderProgram, "bgposition", GL_ARRAY_BUFFER, BGpositionBuffer, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Specify the color attributes
	bg.makeAttribute(shaderProgram, "bgcolor", GL_ARRAY_BUFFER, BGcolorBuffer, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Specifiy the texture usage
	bg.makeAttribute(shaderProgram, "bgcoord", GL_ARRAY_BUFFER, BGtextureBuffer, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	std::vector<GLuint> textures(1,0); // Creates vector with one copy of a zero
	bg.LoadTextures(textures, "./Resources/Background.png", "backGround", shaderProgram, 0); // Binds the background texture to the single number in vector textures
	
	SDL_Event windowEvent;
	while (true) {
		if (SDL_PollEvent(&windowEvent)) {
			if (windowEvent.type == SDL_QUIT) {
				break;
			}
		}
		
		// Clear the screen to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		// Draw a rectangle from the 2 triangles using 6 indices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		// Update window
		SDL_GL_SwapWindow(window);
	}
	glDeleteTextures(static_cast<GLsizei>(textures.size()), &textures.front()); // Casted to remove warning about precision loss (this doesn't matter)
	
	glDeleteProgram(shaderProgram);
	
	// Delete all the buffers!
	glDeleteBuffers(1, &BGelementBuffer);
	glDeleteBuffers(1, &BGpositionBuffer);
	glDeleteBuffers(1, &BGcolorBuffer);
	glDeleteBuffers(1, &BGtextureBuffer);
	
	glDeleteVertexArrays(1, &vertexArrayObject);
	
	SDL_GL_DeleteContext(context);
	return 0;
}