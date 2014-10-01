//
//  Euchre.cpp
//  Euchre
//
//  Created by Dillon Hammond on 9/22/14.
//  Copyright (c) 2014 Learning. All rights reserved.
//

#include "Euchre.h"
using namespace Euchre;

void MakeObject::LoadShaders(GLuint shaderProgram, const char* vertex_file_path, const char* fragment_file_path) { // Loads a vertex and fragment shader, returns a Shader Program called ProgramID
 
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
	glAttachShader(shaderProgram, VertexShaderID);
	glAttachShader(shaderProgram, FragmentShaderID);
	glLinkProgram(shaderProgram);
 
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
 
}

SDL_Window* MakeObject::createWindow(const char* title, int x, int y, int w, int h, Uint32 flags) { // Creates an SDL window context, returns the window
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_Window *window = SDL_CreateWindow(title, x, y, w, h, flags);
	return window;
}

void MakeObject::LoadTextures(std::vector<GLuint> textures, const char* filename, const char* texName, GLuint shaderProgram, int texNum) { // Loads textures and binds them to a position in textures
	int width, height;
	unsigned char* image;
	
	glActiveTexture(GL_TEXTURE0 + texNum);
	
	glBindTexture(GL_TEXTURE_2D, textures.at(texNum));
	image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glUniform1i(glGetUniformLocation(shaderProgram, texName), texNum);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

template<class vecTYPE> // Allows for a vector of any type to be made and returned!
std::vector<vecTYPE> MakeObject::LoadData(std::string file_path) { // Loads a txt file of numbers (comma delimited) and turns them into the requested type and puts them into a vector, returns that vector
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
GLuint MakeObject::makeBufferObject(GLsizei numBuffers, GLenum target, GLenum usage, std::vector<vecTYPE> data) { // Creates a buffer object, returns the buffer object
	GLuint buffer;
	glGenBuffers(numBuffers, &buffer);
	glBindBuffer(target, buffer);
	glBufferData(target, data.size() * sizeof(vecTYPE), data.data(), usage);
	
	return buffer;
}

std::vector<GLuint> MakeObject::makeVertexArrayObject(GLsizei numBuffers) { // Creates a vertex array object, returns that vao
	std::vector<GLuint> vao(numBuffers,0);
	glGenVertexArrays(numBuffers, &vao.front());
	return vao;
}

void MakeObject::makeAttribute(GLuint shaderProgram, const char* attrib_name, GLenum target, GLuint targetBuffer, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer) {
	GLint attribute = glGetAttribLocation(shaderProgram, attrib_name);
	glEnableVertexAttribArray(attribute);
	glBindBuffer(target, targetBuffer);
	glVertexAttribPointer(attribute, size, type, normalized, stride, pointer);
}

std::tuple<GLuint, GLuint, GLuint, GLuint> Euchre::MakeObject::makeAllBuffers(bool BG) { // Returns vao, posBuffer, colBuffer, texBuffer, elementBuffer
	std::string files[4];
	if (BG) {
		files[0] = "./Resources/BGattribs/bgPosition.txt";
		files[1] = "./Resources/BGattribs/bgColor.txt";
		files[2] = "./Resources/BGattribs/bgTexture.txt";
		files[3] = "./Resources/BGattribs/bgdrawOrder.txt";
	} else if (!BG) {
		files[0] = "./Resources/cardAttribs/cardPosition.txt";
		files[1] = "./Resources/cardAttribs/cardColor.txt";
		files[2] = "./Resources/cardAttribs/cardTexture.txt";
		files[3] = "./Resources/cardAttribs/carddrawOrder.txt";
	}
	
	// Create a vector populated by text file data that describes the coordinates of the triangles to be drawn
	std::vector<float> position = MakeObject::LoadData<float>(files[0]);
	
	// Create a Position Buffer Object and copy the vector data to it
	GLuint positionBuffer = MakeObject::makeBufferObject(1, GL_ARRAY_BUFFER, GL_STATIC_DRAW, position);
	
	// Create a vector populated by text file data that has the color of the triangles to be drawn
	std::vector<float> color = MakeObject::LoadData<float>(files[1]);
	
	// Create a Color Buffer Object and copy the vector data to it
	GLuint colorBuffer = MakeObject::makeBufferObject(1, GL_ARRAY_BUFFER, GL_STATIC_DRAW, color);
	
	// Create a vector populated by text file data that has the location of the texture image
	std::vector<float> texture = MakeObject::LoadData<float>(files[2]);
	
	// Create a Texture Buffer Object and copy the vector data to it
	GLuint textureBuffer = MakeObject::makeBufferObject(1, GL_ARRAY_BUFFER, GL_STATIC_DRAW, texture);
	
	// Create a vector populated by text file data that describes the order that an object is drawn in
	std::vector<GLuint> drawOrder = MakeObject::LoadData<GLuint>(files[3]);
	
	// Create an Element Buffer Object and copy the vector data to it
	GLuint elementBuffer = MakeObject::makeBufferObject(1, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, drawOrder);
	
	return std::tuple<GLuint, GLuint, GLuint, GLuint>(positionBuffer, colorBuffer, textureBuffer, elementBuffer);
}