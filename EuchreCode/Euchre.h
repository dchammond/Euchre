//
//  Euchre.h
//  Euchre
//
//  Created by Dillon Hammond on 9/22/14.
//  Copyright (c) 2014 Learning. All rights reserved.
//

#ifndef __Euchre__Euchre__
#define __Euchre__Euchre__

#include <stdio.h>
#define GLEW_STATIC
#include "GL/glew.h"
#include <SDL2/SDL.h>
#include "SOIL.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace Euchre {
	class MakeObject {
	public:
		GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

		SDL_Window* createWindow(const char* title, int x, int y, int w, int h, Uint32 flags);

		void LoadTextures(std::vector<GLuint> textures, const char* filename, const char* texName, GLuint shaderProgram, int texNum);

		template<class vecTYPE> // Allows for a vector of any type to be made and returned!
		std::vector<vecTYPE> LoadData(std::string file_path);

		template<class vecTYPE> // Redefine new vecTYPE
		GLuint makeBufferObject(GLsizei numBuffers, GLenum target, GLenum usage, std::vector<vecTYPE> data);

		GLuint makeVertexArrayObject(GLsizei numBuffers);

		void makeAttribute(GLuint shaderProgram, const char* attrib_name, GLenum target, GLuint targetBuffer, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);

		std::tuple<GLuint, GLuint, GLuint, GLuint> makeAllBuffers(bool BG);
	
	};
}

#endif /* defined(__Euchre__Euchre__) */