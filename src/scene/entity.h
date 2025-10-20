#ifndef ENTITY_H
#define ENTITY_H

#include "scene.h"
#include "components.h"

namespace psx {

	class Entity{
		public:
			Entity() = default;
			Entity(entt::entity handle, Scene* scene);

			template <typename T, typename... Args>
			T& AddComponent(Args&&... args){
				return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
			}

			template <typename T>
			T& GetComponent(){
				return m_scene->m_registry.get<T>(m_entityHandle);
			}

			template <typename T>
			bool HasComponent(){
				return m_scene->m_registry.any_of<T>(m_entityHandle);
			}

			template <typename T>
			void RemoveComponent(){
				m_scene->m_registry.remove<T>(m_entityHandle);
			}

			Scene* GetScene() const { return m_scene; }

			UUID GetUUID() { return GetComponent<IDComponent>().ID; }
			
			operator bool() const { return m_entityHandle != static_cast<entt::entity>(0); }
			operator entt::entity() const { return m_entityHandle; }
		private:
			entt::entity m_entityHandle{entt::null};
			Scene* m_scene = nullptr;
	};

}
#endif

