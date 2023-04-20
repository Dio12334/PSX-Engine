#include "entity.h"
#include "../engine/engine.h"
#include "../inputSystem/inputSystem.h"
#include "../components/component.h"
#include "../fileSystem/jsonHelper.h"
#include <algorithm>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <SDL2/SDL.h>

namespace psx {
	const char* Entity::typeNames[NUM_ACTOR_TYPES] = {
		"Entity"
	};	
	Entity::Entity(class Engine* engine):
		m_state(State::sActive),
		m_position(0, 0),
		m_scale(1.f),
		m_rotation(0),
		m_engine(engine),
		m_worldTransform(1.f),
		m_recomputeWorldTransform(true){
			m_engine->AddEntity(this);
		}

	Entity::~Entity(){
		m_engine->RemoveEntity(this);
		while(!m_components.empty()){
			delete m_components.back();
		}
	}

	void Entity::Update(float deltaTime){
		if(m_state == State::sActive){
			ComputeWorldTransform();

			UpdateComponents(deltaTime);
			UpdateEntity(deltaTime);
			
			ComputeWorldTransform();
		}
	}

	void Entity::UpdateComponents(float deltaTime){
		for(auto component: m_components){
			component->Update(deltaTime);
		}
	}

	void Entity::UpdateEntity(float deltaTime){

	} 


	void Entity::AddComponent(class Component* component){
		int order = component->GetUpdateOrder();
		auto iter = m_components.begin();
		for(; iter != m_components.end(); ++iter){
			if(order < (*iter)->GetUpdateOrder()){
				break;
			}
		}
		m_components.insert(iter, component);
	}

	void Entity::RemoveComponent(class Component* component){
		auto iter = std::find(m_components.begin(), m_components.end(), component);
		if(iter != m_components.end()){
			m_components.erase(iter);
		}
	}


	void Entity::ProcessInput(const struct InputState &state){
		if (m_state == State::sActive){
			for (auto comp : m_components)
				comp->ProcessInput(state);
			EntityInput(state);
		}
	}

	void Entity::EntityInput(const InputState& state){
		
	}

	void Entity::ComputeWorldTransform(){
		if(m_recomputeWorldTransform){
			m_recomputeWorldTransform = false;
			m_worldTransform = glm::translate(glm::mat4(1.f), glm::vec3(m_position.x, m_position.y, 0));
			m_worldTransform = glm::rotate(m_worldTransform, m_rotation, glm::vec3(0.f,0.f,1.f));
			m_worldTransform = glm::scale(m_worldTransform, glm::vec3(m_scale, m_scale, m_scale));
			for(auto comp: m_components){
				comp->OnUpdateWorldTransform();
			}
		}
	}

	void Entity::LoadProperties(const rapidjson::Value &inObj){
		std::string state;
		if(json::GetString(inObj, "state", state)){
			if(state == "active"){
				SetState(sActive);
			}
			else if(state == "paused"){
				SetState(sPaused);
			}
			else if(state == "dead"){
				SetState(sDead);
			}
		}
		json::GetVector2D(inObj, "position", m_position);
		json::GetFloat(inObj, "rotation", m_rotation);
		json::GetFloat(inObj, "scale", m_scale);
		ComputeWorldTransform();
	}

	void Entity::SaveProperties(rapidjson::Document::AllocatorType &alloc, rapidjson::Value &inObj) const {
		
		std::string state = "active";
		if(m_state == sPaused){
			state = "paused";
		}
		else if(m_state == sDead){
			state = "dead";
		}
		
		json::AddString(alloc, inObj, "state", state);
		json::AddVector2D(alloc, inObj, "position", m_position);
		json::AddFloat(alloc, inObj, "rotation", m_rotation);
		json::AddFloat(alloc, inObj, "scale", m_scale);
	}
}
