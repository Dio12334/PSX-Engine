#ifndef SCENE_H
#define SCENE_H

#include "../../libs/entt-3.11.1/single_include/entt/entt.hpp"
#include "../utils/psxint.h"
#include <string>

namespace psx{
	class Entity;

	class Scene{
		public:
			Scene(class Engine* engine);
			virtual ~Scene();

			class Engine* GetEngine() const { return m_engine; }

            enum State{
                sGameplay,
                sPause,
                sQuit,
                sUI
            };

			virtual void LoadScene() {}

			Entity CreateEntity(const std::string& name = std::string());
			void DestroyEntity(Entity entity);

			void OnViewPortResize(u32 width, u32 height);
			void OnUpdate(float dt);			

		protected:
			u32 m_viewportWidth = 0, m_viewportHeight = 0;
			entt::registry m_registry;
			State inputState, updateState;
			class Engine* m_engine;
			friend class Entity;
			friend class Renderer;
	};
}

#endif

