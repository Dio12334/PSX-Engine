#include "component.h"
#include "../entity/entity.h"
#include "../fileSystem/jsonHelper.h"

#include <SDL2/SDL.h>
namespace psx {

	
	Component::Component(class Entity* owner, i32 updateOrder):
		m_entity(owner),
		m_updateOrder(updateOrder){
			m_entity->AddComponent(this);
		}

	Component::~Component(){
		m_entity->RemoveComponent(this);
	}

	void Component::Update(float deltaTime){

	}

	void Component::LoadProperties(const rapidjson::Value& inObj){
		json::GetInt(inObj, "updateOrder", m_updateOrder);
	}

	void Component::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const {
		json::AddInt(alloc, inObj, "updateOrder", m_updateOrder);
	}

}
