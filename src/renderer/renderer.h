#ifndef RENDERER_H
#define RENDERER_H

#include "../math/psxvector.h"
#include <SDL2/SDL.h>
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>

namespace psx {
	class Renderer{
		public:
			Renderer(class Engine* engine);
			~Renderer();
			bool Initialize(unsigned int screenWidth, unsigned int screenHeight);
			void Shutdown();
			void UnloadData();
			void Draw(class Scene* scene);

			class Texture* LoadTexture(const std::string& fileName);

			
			void SetViewMatrix(const glm::mat4& matrix) { m_view = matrix; }
			glm::mat4 GetViewMatrix() const { return m_view; }

			void SetPosition(const Vec2f& position) { m_position = position; }
			Vec2f GetPosition() const { return m_position; }

			Vec2f GetScreenDimensions() const { return Vec2f(m_screenWidth, m_screenHeight); }

			glm::mat4 CreateUIProjMatrix();

			void SetZoom(float zoom) { m_zoom = zoom; }
			float GetZoom() const { return m_zoom; }

			SDL_Window* GetWindow() const { return m_window; }
			SDL_GLContext GetContext() const { return m_context; }
		private:
			bool LoadShaders();
			void CreateSpriteVerts();
		
			class Engine* m_engine;
			SDL_Window* m_window;
			class Shader* m_spriteShader;
			class VertexArray* m_spriteVerts;

			int m_screenWidth;
			int m_screenHeight;
			
			SDL_GLContext m_context;
			glm::mat4 m_view;
			
			Vec2f m_position;
			float m_zoom;
			std::unordered_map<std::string, class Texture*> m_textures;
			
		
	};

	glm::mat4 CreateTopDownViewMatrix(Vec2f cameraPosition, float zoom, float width, float height);
}

#endif

