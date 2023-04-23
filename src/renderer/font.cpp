#include "font.h"
#include "texture.h"
#include "../utils/psxcolor.h"
#include "../engine/engine.h"
#include <SDL2/SDL_ttf.h>

namespace psx {
	Font::Font(class Engine* engine): 
		m_engine(engine){

		}
			
	Font::~Font(){

	}
			
	bool Font::Load(const std::string& fileName){
		std::array<i32, 29> fontSizes = {
			8, 9, 10, 11, 12, 14, 16, 18, 20, 22,
			24, 26, 28, 30, 32, 34, 36, 38, 40, 42,
			44, 46, 48, 52, 56, 60, 64, 68, 72
		};

		for(auto& size: fontSizes){
			TTF_Font* font = TTF_OpenFont(fileName.c_str(), size);
			if(!font){
				SDL_Log("Failed to load font %s in size %d\n", fileName.c_str(), size);
				return false;
			}
			m_fontData.emplace(size, font);
		}
		return true;
	}
			
	void Font::Unload(){
		
	}

	Texture* Font::RenderText(const std::string& text, const class Color& color, i32 pointSize){
		Texture* texture = nullptr;
		SDL_Color sdlColor;
		sdlColor.r = static_cast<u8>(color.r * 255);
		sdlColor.g = static_cast<u8>(color.g * 255);
		sdlColor.b = static_cast<u8>(color.b * 255);
		sdlColor.a = 255;
		
		auto iter = m_fontData.find(pointSize);
		if(iter != m_fontData.end()){
			auto font = iter->second;
			const std::string& actualText = m_engine->GetText(text);
			auto surf = TTF_RenderUTF8_Blended(font, actualText.c_str(), sdlColor);
			if(surf){
				texture = new Texture();
				texture->CreateFromSurface(surf);
				SDL_FreeSurface(surf);
			}
		}
		else{
			SDL_Log("Point size %d is not supported\n", pointSize);
		}
		return texture;
	}
}
