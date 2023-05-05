#include "texture.h"
#include <SDL2/SDL_surface.h>
#include <SOIL/SOIL.h>
#include <SDL2/SDL_log.h>
#include <GL/glew.h>

namespace psx {

	Texture::Texture(): 
		m_textureID(0),
		m_width(0),
		m_height(0){

		}

	Texture::~Texture(){

	}

	bool Texture::Load(const std::string &fileName){
		i32 channels = 0;
		unsigned char* image = SOIL_load_image(fileName.c_str(), &m_width, &m_height, &channels, SOIL_LOAD_AUTO);
		if(!image){
			SDL_Log("SOIL failed to load image: %s\n", fileName.c_str());
			return false;
		}
		m_fileName = fileName;

		i32 format = GL_RGB;
		if(channels == 4){
			format = GL_RGBA;
		}

		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format,m_width, m_height, 0, format,
				GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		return true;
	}

	void Texture::Unload(){
		glDeleteTextures(1, & m_textureID);
	}

	void Texture::SetActive(){
		glBindTexture(GL_TEXTURE_2D, m_textureID);
	}

	void Texture::CreateFromSurface(SDL_Surface* surface){
		
		if(!surface){
			SDL_Log("error\n");
			return;
		}
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		auto convertedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ABGR8888, 0);
	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, convertedSurface->w, 
				convertedSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, convertedSurface->pixels);
		m_width = convertedSurface->w;
		m_height = convertedSurface->h;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		SDL_FreeSurface(convertedSurface);
	}
}
