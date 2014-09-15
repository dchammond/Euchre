//
//  main.cpp
//  Euchre
//
//  Created by Dillon Hammond on 9/12/14.
//  Copyright (c) 2014 Learning. All rights reserved.
//

#define GLEW_STATIC
#include "GL/glew.h"
#include <SDL2/SDL.h>
#include "SOIL.h"
#define GLM_FORCE_RADIANS
#include "glm.hpp"
#include <iostream>
#include <fstream>
#include <vector>

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {
 
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	
	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream;
	VertexShaderStream.open(vertex_file_path);
	if(VertexShaderStream.is_open()) {
		std::string Line = "";
		while(getline(VertexShaderStream, Line)) {
			VertexShaderCode += "\n" + Line;
		}
		VertexShaderStream.close();
	}
 
	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream;
	FragmentShaderStream.open(fragment_file_path);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line)) {
			FragmentShaderCode += "\n" + Line;
		}
		FragmentShaderStream.close();
	}
	GLint Result;
	// Compile Vertex Shader
	const char* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		if (Result == GL_FALSE) {
			std::cout << "NOOOV" << std::endl;
			std::cout << VertexSourcePointer << std::endl;
		}
	
 
	// Compile Fragment Shader
	const char* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		if (Result == GL_FALSE) {
			std::cout << "NOOOF" << std::endl;
			std::cout << FragmentSourcePointer << std::endl;
		}
	
 
	// Link the program
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);
 
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
 
	return ProgramID;
}

SDL_Window* createWindow(const char* title, int x, int y, int w, int h, Uint32 flags) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_Window *window = SDL_CreateWindow(title, x, y, w, h, flags);
	return window;
}

void LoadTextures(std::vector<GLuint> textures, const char* filename, const char* texName, GLuint shaderProgram, int texNum) {
	int width, height;
	unsigned char* image;
	
	glActiveTexture(GL_TEXTURE0 + texNum);
	
	glBindTexture(GL_TEXTURE_2D, textures.at(texNum));
	image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glUniform1i(glGetUniformLocation(shaderProgram, "backGround"), texNum);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

template<class vecTYPE> // Allows for a vector of any type to be made and returned!
std::vector<vecTYPE> LoadData(const char* file_path) {
	// Read Vertices in from a file
	std::vector<vecTYPE> vertices;
	std::string fileCode;
	std::ifstream fileStream;
	fileStream.open(file_path);
	if(fileStream.is_open()) {
		std::string Line;
		while(getline(fileStream, Line, ',')) {
			fileCode = Line;
			float numRead = std::stof(fileCode); // std::stof converts a string to a float
			vertices.push_back(numRead); // When the value is pushed to the vertex, it is casted automatically
		}
		fileStream.close();
	}
	return vertices;
}

template<class vecTYPE> // Redefine new vecTYPE
GLuint makeBufferObject(GLsizei numBuffers, GLenum target, GLenum usage, std::vector<vecTYPE> data) {
	GLuint buffer;
	glGenBuffers(numBuffers, &buffer);
	glBindBuffer(target, buffer);
	glBufferData(target, data.size() * sizeof(vecTYPE), &data.at(0), usage);
	
	return buffer;
}

int main() {
	SDL_Window* window = createWindow("Euchre", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	glewInit();
	
	// Create Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	// Create a vector populated by text file data that describes the coordinates of the triangles to be drawn
	std::vector<float> bgPosition = LoadData<float>("./Resources/bgPosition.txt");
	
	// Create a Position Buffer Object and copy the vector data to it
	GLuint positionBuffer = makeBufferObject(1, GL_ARRAY_BUFFER, GL_STATIC_DRAW, bgPosition);
	
	// Create a vector populated by text file data that has the color of the triangles to be drawn
	std::vector<float> bgColor = LoadData<float>("./Resources/bgColor.txt");
	
	// Create a Color Buffer Object and copy the vector data to it
	GLuint colorBuffer = makeBufferObject(1, GL_ARRAY_BUFFER, GL_STATIC_DRAW, bgColor);
	
	// Create a vector populated by text file data that has the location of the texture image
	std::vector<float> bgTexture = LoadData<float>("./Resources/bgTexture.txt");
	
	// Create a Texture Buffer Object and copy the vector data to it
	GLuint textureBuffer = makeBufferObject(1, GL_ARRAY_BUFFER, GL_STATIC_DRAW, bgTexture);
	
	// Create a vector populated by text file data that describes the order that an object is drawn in
	std::vector<GLuint> drawOrder = LoadData<GLuint>("./Resources/drawOrder.txt");
	
	// Create an Element Buffer Object and copy the vector data to it
	GLuint elementBuffer = makeBufferObject(1, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, drawOrder);
	
	GLuint shaderProgram = LoadShaders("./Resources/vertexShader.txt", "./Resources/fragmentShader.txt");
	glUseProgram(shaderProgram);
	
	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Specify the color attributes
	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Specifiy the texture usage
	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

	std::vector<GLuint> textures(1,0);
	glGenTextures(1, &textures.at(0));
	
	LoadTextures(textures, "./Resources/Background.png", "backGround", shaderProgram, 0);
	
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
		
		SDL_GL_SwapWindow(window);
	}
	glDeleteTextures(static_cast<GLsizei>(textures.size()), &textures.at(0)); // Casted to remove warning about precision loss (this doesn't matter)
	
	glDeleteProgram(shaderProgram);
	
	glDeleteBuffers(1, &elementBuffer);
	glDeleteBuffers(1, &positionBuffer);
	glDeleteBuffers(1, &colorBuffer);
	glDeleteBuffers(1, &textureBuffer);
	
	glDeleteVertexArrays(1, &vao);
	
	SDL_GL_DeleteContext(context);
	return 0;
}