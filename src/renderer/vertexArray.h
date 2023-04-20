#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "../utils/psxint.h"

namespace psx {
	class VertexArray{
		public:
			VertexArray(const float* verts, u32 numVerts, const u32* indices, u32 numIndices);
			~VertexArray();

			void SetActive();

			u32 GetNumIndices() const { return m_numIndices; }
			u32 GetNumVerts() const { return m_numVerts; }

		private:
			u32 m_numVerts;
			u32 m_numIndices;
			u32 m_vertexBuffer;
			u32 m_indexBuffer;
			u32 m_vertexArray;
	};
}

#endif

