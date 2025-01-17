#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace lei3d 
{
	class Shader {
	private:
		unsigned int shaderProgramID;
	public:


		Shader();
		Shader(const char* vertexShaderPath, const char* fragShaderPath);

		
		// compile and link shader, then activate the shader
		void use();
		
		void setUniformMat4(glm::mat4& matrix, const char* matrixName);
		void setInt(const std::string &name, int value); // set string value in shader to an int
	};
}

#endif