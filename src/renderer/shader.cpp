#include "shader.h"
#include <cstring>
#include <fstream>
#include <SDL2/SDL_log.h>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

namespace psx {
	Shader::Shader(){

	}

	Shader::~Shader(){

	}

	bool Shader::CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader){
		std::ifstream shaderFile(fileName);
		if(shaderFile.is_open()){
			std::stringstream sstream;
			sstream << shaderFile.rdbuf();
			std::string contents = sstream.str();
			const char* contentsChar = contents.c_str();

			outShader = glCreateShader(shaderType);
			glShaderSource(outShader, 1, &contentsChar, nullptr);
			glCompileShader(outShader);

			if(!IsCompiled(outShader)){
				SDL_Log("Failed to compile shader %s\n", fileName.c_str());
				return false;
			}
		}
		else{
			SDL_Log("Shader file not found: %s\n", fileName.c_str());
			return false;
		}
		return true;
	}

	bool Shader::IsCompiled(GLuint shader){
		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

		if(status != GL_TRUE){
			char buffer[512];
			memset(buffer, 0, 512);
			glGetShaderInfoLog(shader, 511, nullptr, buffer);
			SDL_Log("GLSL Compile Failed:\n%s\n", buffer);
			return false;
		}
		return true;
	}

	bool Shader::Load(const std::string &vertName, const std::string &fragName){
		if(!CompileShader(vertName, GL_VERTEX_SHADER, m_vertexShader) || !CompileShader(fragName, GL_FRAGMENT_SHADER, m_fragShader)){
			return false;
		}

		m_shaderProgram = glCreateProgram();
		glAttachShader(m_shaderProgram, m_vertexShader);
		glAttachShader(m_shaderProgram, m_fragShader);
		glLinkProgram(m_shaderProgram);
		if(!IsValidProgram()){
			return false;
		}
		return true;
	}

	bool Shader::IsValidProgram(){
		GLint status;
		glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &status);

		if(status != GL_TRUE){
			char buffer[512];
			memset(buffer, 0, 512);
			glGetProgramInfoLog(m_shaderProgram, 511, nullptr, buffer);
			SDL_Log("GLSL Link Failed:\n%s\n", buffer);
			return false;
		}
		return true;
	}

	void Shader::SetActive(){
		glUseProgram(m_shaderProgram);
	}

	void Shader::Unload(){
		glDeleteProgram(m_shaderProgram);
		glDeleteShader(m_fragShader);
		glDeleteShader(m_vertexShader);
	}

	void Shader::SetMatrixUniform(const char *name, const glm::mat4 &matrix){
		GLuint loc = glGetUniformLocation(m_shaderProgram, name);
		glUniformMatrix4fv(loc, 1, GL_TRUE, glm::value_ptr(matrix));
	}
}
