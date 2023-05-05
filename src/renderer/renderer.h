#ifndef RENDERER_H
#define RENDERER_H

#include "../math/psxvector.h"
#include "../scene/components.h"
#include <SDL2/SDL.h>
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>

namespace psx {
	class Renderer{
		public:

			static bool Initialize(unsigned int screenWidth, unsigned int screenHeight);
			static void Shutdown();
			static void UnloadData();


            static void DrawSprite(const TransformComponent& transform, const SpriteComponent& sprite);
            static void BeginScene(const Camera& camera, const glm::mat4& cameraTransform);

            static void EndScene();

			static class Texture* LoadTexture(const std::string& fileName);

			static Vec2f GetScreenDimensions() { return Vec2f(m_screenWidth, m_screenHeight); }

			static SDL_Window* GetWindow() { return m_window; }
			static SDL_GLContext GetContext() { return m_context; }

		private:

			static bool LoadShaders();
			static void CreateSpriteVerts();

			static SDL_Window* m_window;
			static class Shader* m_spriteShader;
			static class VertexArray* m_spriteVerts;

			static int m_screenWidth;
			static int m_screenHeight;
			
			static SDL_GLContext m_context;
			
			static std::unordered_map<std::string, class Texture*> m_textures;
			
		
	};

}

#endif

