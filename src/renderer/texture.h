#ifndef TEXTURE_H
#define TEXTURE_H

#include "../utils/psxint.h"
#include <string>

struct SDL_Surface;

namespace psx {
	class Texture{
		public:
			Texture();
			~Texture();
			bool Load(const std::string& fileName);
			std::string GetFileName() const { return m_fileName; }
			void Unload();
			void SetActive();
			i32 GetWidth() const { return m_width; }
			i32 GetHeight() const { return m_height;}
			void CreateFromSurface(SDL_Surface* surface);
			i32 GetTextureID() const { return m_textureID; }
		private:
			u32 m_textureID;
			i32 m_width;
			i32 m_height;
			std::string m_fileName;
	};
}

#endif

