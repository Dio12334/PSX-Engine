#ifndef SCENE_H
#define SCENE_H

#include "../../libs/entt-3.11.1/single_include/entt/entt.hpp"
#include <string>

namespace psx{
	class Entity;

	class Scene{
		public:
			Scene(class Engine* engine);
			virtual ~Scene();

			class Engine* GetEngine() const { return m_engine; }
			virtual void LoadScene() {}
			Entity CreateEntity(const std::string& name = std::string());
			void OnUpdate(float dt);			

		protected:
			entt::registry m_registry;
			class Engine* m_engine;
			friend class Entity;
			friend class Renderer;
	};
}

#endif

