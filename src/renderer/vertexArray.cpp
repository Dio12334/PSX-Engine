#include "vertexArray.h"
#include <GL/glew.h>

namespace psx {
	VertexArray::VertexArray(const float* verts, u32 numVerts, const u32* indices, u32 numIndices):
		m_numVerts(numVerts),
		m_numIndices(numIndices){
			glGenVertexArrays(1, &m_vertexArray);
			glBindVertexArray(m_vertexArray);

			glGenBuffers(1, &m_vertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, numVerts * 5 * sizeof(float), verts, GL_STATIC_DRAW);

			glGenBuffers(1, &m_indexBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(i32), indices, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(sizeof(float) * 3));
		}

	VertexArray::~VertexArray(){
		glDeleteBuffers(1, &m_indexBuffer);
		glDeleteBuffers(1, &m_vertexBuffer);
		glDeleteVertexArrays(1, &m_vertexArray);
	}

	void VertexArray::SetActive(){
		glBindVertexArray(m_vertexArray);
	}
}

