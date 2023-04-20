#include "boxComponent.h"
#include "../../entity/entity.h"

namespace psx {

	BoxComponent::BoxComponent(class Entity* entity): 
		Component(entity),
		m_objectBox(Vec2f(), Vec2f()),
		m_worldBox(Vec2f(), Vec2f()),
		m_shouldRotate(true){
		
		}

	BoxComponent::~BoxComponent(){

	}
	
	void BoxComponent::OnUpdateWorldTransform() {
		m_worldBox = m_objectBox;
		m_objectBox.min *= m_entity->GetScale();
		m_worldBox.max *= m_entity->GetScale();
		if(m_shouldRotate){
			m_worldBox.Rotate(m_entity->GetRotation());
		}

		m_worldBox.min += m_entity->GetPosition();
		m_worldBox.max += m_entity->GetPosition();
	}
}
