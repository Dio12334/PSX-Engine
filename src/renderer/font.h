#ifndef FONT_H
#define FONT_H

#include "../utils/psxint.h"
#include "../utils/psxcolor.h"
#include <string>
#include <unordered_map>
#include <SDL2/SDL_ttf.h>

namespace psx {
	class Font{
		public:
			Font(class Engine* engine);
			~Font();
			bool Load(const std::string& fileName);
			void Unload();
			class Texture* RenderText(const std::string& text,
					const Color& color = Color::White, i32 pointSize = 30);
		private:
			class Engine* m_engine;
			std::unordered_map<i32, TTF_Font*> m_fontData;
	};
}

#endif

