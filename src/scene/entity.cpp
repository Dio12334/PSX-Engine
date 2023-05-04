#include "entity.h"

namespace psx {
	Entity::Entity(entt::entity handle, Scene* scene): 
		m_entityHandle(handle),
		m_scene(scene)
		{

		}
}
