#include "renderer.h"
#include "texture.h"
#include "vertexArray.h"
#include "shader.h"
#include "../components/sprites/spriteComponent.h"
#include "../engine/engine.h"
#include <GL/glew.h>
#include <SDL2/SDL_video.h>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace psx {

	Renderer::Renderer(class Engine* engine) :
		m_engine(engine), 
		m_window(nullptr), 
		m_spriteShader(nullptr),
		m_spriteVerts(nullptr),
		m_screenWidth(0), 
		m_screenHeight(0){ 

	}

	Renderer::~Renderer(){

	}
			
	bool Renderer::Initialize(unsigned int screenWidth, unsigned int screenHeight){
		m_screenHeight = screenHeight;
		m_screenWidth = screenWidth;

		// Use the core OpenGL profile
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		// Specify version 3.3
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		// Request a color buffer with 8-bits per RGBA channel
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		// Enable double buffering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		// Force OpenGL to use hardware acceleration
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		m_window = SDL_CreateWindow(
				"P.N. ENGINE",
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				m_screenWidth,
				m_screenHeight, 
				SDL_WINDOW_OPENGL
				);

		if(!m_window){
			SDL_Log("Failed to create window: %s\n", SDL_GetError());
			return false;
		}

		m_context = SDL_GL_CreateContext(m_window);

		glewExperimental = GL_TRUE;
		if(glewInit() != GLEW_OK){
			SDL_Log("Failed to initialize GLEW.\n");
			return false;
		}
		glGetError();
		
		if(!LoadShaders()){
			return false;
		}

		CreateSpriteVerts();

		return true;
	}
			
	void Renderer::Shutdown(){
		IMG_Quit();
		SDL_GL_DeleteContext(m_context);
		SDL_DestroyWindow(m_window);
	}
	
	void Renderer::UnloadData(){
		for(auto i: m_textures){
			i.second->Unload();
			delete i.second;
		}
		m_textures.clear();
	}
			
	void Renderer::Draw(){
		glClearColor(0.86f, 0.86f, 0.86f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		m_spriteShader->SetActive();
		m_spriteVerts->SetActive();

		m_spriteShader->SetMatrixUniform("uViewProj", m_view);

		for(auto sprite: m_sprites){
			sprite->Draw(m_spriteShader);
		}

		glDisable(GL_TEXTURE_2D);

		SDL_GL_SwapWindow(m_window);
	}


	void Renderer::AddSprite(class SpriteComponent *sprite){
		int drawOrder = sprite->GetDrawOrder();
		auto iter = m_sprites.begin();
		for(; iter != m_sprites.end(); ++iter){
			if(drawOrder < (*iter)->GetDrawOrder()){
				break;
			}
		}

		m_sprites.insert(iter, sprite);
	}

	void Renderer::RemoveSprite(class SpriteComponent *sprite){
		auto iter = std::find(m_sprites.begin(), m_sprites.end(), sprite);
		if(iter != m_sprites.end()){
			m_sprites.erase(iter);
		}
	}

	
	class Texture* Renderer::LoadTexture(const std::string& fileName){
		const char* name = fileName.c_str();
		Texture* text = nullptr;
	
		auto iter = m_textures.find(fileName);
	
		if(iter != m_textures.end()){
			text = iter->second;
		}
		else{
			text = new Texture();
			if(text->Load(name)){
				m_textures.emplace(fileName, text);
			}
			else{
				delete text;
				return nullptr;
			}
		}
		return text;
	}

	void Renderer::CreateSpriteVerts(){
		float vertices[] = {
			-0.5f,  0.5f, 0.f, 0.f, 0.f,// top left
			0.5f,  0.5f, 0.f, 1.f, 0.f,// top right
			0.5f, -0.5f, 0.f, 1.f, 1.f, // bottom right
			-0.5f, -0.5f, 0.f, 0.f, 1.f  // bottom left
		};

		u32 indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		m_spriteVerts = new VertexArray(vertices, 4, indices, 6);
	}

	bool Renderer::LoadShaders(){
		m_spriteShader = new Shader();
		if(!m_spriteShader->Load("shaders/sprite.vert", "shaders/sprite.frag")){
			return false;
		}
		m_spriteShader->SetActive();
		m_view = CreateTopDownViewMatrix(Vec2f(0.f, 0.f), 400.f, m_screenWidth, m_screenHeight);
		m_spriteShader->SetMatrixUniform("uViewProj", m_view);
		return true;
	}


	glm::mat4 CreateTopDownViewMatrix(Vec2f cameraPosition, float zoom, float width, float height){
		auto halfWidth = width * 0.5f / zoom;
		auto halfHeight = height * 0.5f / zoom;
		auto left = cameraPosition.x - halfWidth;
		auto right = cameraPosition.x + halfWidth;
		auto bottom = cameraPosition.y - halfHeight;
		auto top = cameraPosition.y + halfHeight;
		auto view = glm::ortho(left, right, bottom, top, -1.f, 1.f);
		return view;
	}

	glm::mat4 Renderer::CreateUIProjMatrix(){
		return glm::ortho(0.0f, static_cast<float>(m_screenWidth), static_cast<float>(m_screenHeight), 0.0f, -1.0f, 1.0f);
	}
}
