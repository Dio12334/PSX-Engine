#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>

namespace psx {
	class Shader{
		public:
			Shader();
			~Shader();
			bool Load(const std::string& vertName, const std::string& fragName);
			void Unload();
			void SetActive();
			void SetMatrixUniform(const char* name, const glm::mat4& matrix);
		private:
			bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);
			bool IsCompiled(GLuint shader);
			bool IsValidProgram();
			GLuint m_vertexShader;
			GLuint m_fragShader;
			GLuint m_shaderProgram;
	};
}

#endif

