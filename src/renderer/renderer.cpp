#include "renderer.h"
#include "texture.h"
#include "vertexArray.h"
#include "shader.h"

#include "../scene/scene.h"
#include <GL/glew.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_video.h>
#include <algorithm>

namespace psx {
    SDL_Window* Renderer::m_window = nullptr;
    class Shader* Renderer::m_spriteShader = nullptr;
    class VertexArray* Renderer::m_spriteVerts = nullptr;

    int Renderer::m_screenWidth = 0;
    int Renderer::m_screenHeight = 0;

    SDL_GLContext Renderer::m_context;

    std::unordered_map<std::string, class Texture*> Renderer::m_textures;

	bool Renderer::Initialize(unsigned int screenWidth, unsigned int screenHeight){
		m_screenHeight = (int) screenHeight;
		m_screenWidth = (int) screenWidth;

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
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

    void Renderer::BeginScene(const Camera& camera, const glm::mat4& cameraTransform){
        glClearColor(0.86f, 0.86f, 0.86f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_spriteShader->SetActive();
        m_spriteVerts->SetActive();

        auto viewProj = camera.GetProjection() * glm::inverse(cameraTransform);
        m_spriteShader->SetMatrixUniform("uViewProj", viewProj);

    }
    void Renderer::EndScene(){
        glDisable(GL_TEXTURE_2D);
        SDL_GL_SwapWindow(m_window);
    }

	void Renderer::DrawSprite(const TransformComponent& tc, const SpriteComponent& sc){
		if(sc.texture){
			auto scaleMat = glm::scale(glm::mat4(1.f),glm::vec3(static_cast<float>(sc.TexWidth), static_cast<float>(sc.TexHeight), 1.0f));
			auto world = tc.GetTransform() * scaleMat;
			m_spriteShader->SetMatrixUniform("uWorldTransform", world);
			sc.texture->SetActive();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
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
				SDL_Log("Failed to load texture %s\n", fileName.c_str());
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
		return true;
	}


}
