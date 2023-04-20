#include "cameraComponent.h"
#include "../component.h"
#include "../../entity/entity.h"
#include "../../engine/engine.h"
#include "../../renderer/renderer.h"
#include "../../inputSystem/inputSystem.h"
#include "../../fileSystem/jsonHelper.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <iostream>

namespace psx {
	INITIALIZE_COMPONENT_TYPE(CameraComponent);
	CameraComponent::CameraComponent(class Entity* owner): Component(owner){
		m_renderer = owner->GetEngine()->GetRenderer();			
	}
	
	CameraComponent::~CameraComponent(){

	}


	
	void CameraComponent::ProcessInput(const InputState& state){
		m_movement = Vec2f(0,0);
		
		if(state.keyboard.GetKeyValue(KeyCode::S)){
			m_movement.y -= 1;
		}

		if(state.keyboard.GetKeyValue(KeyCode::W)){
			m_movement.y += 1;
		}

		if(state.keyboard.GetKeyValue(KeyCode::A)){
			m_movement.x -= 1;
		}

		if(state.keyboard.GetKeyValue(KeyCode::D)){
			m_movement.x += 1;
		}
		auto swy = state.mouse.GetScrollWheel().y;
		if(swy != 0){		
			if(swy > 0 ){
				m_zoom = !(m_zoom - 1)? 1: m_zoom - 1;
			}
			else{
				++m_zoom;
			}

			m_renderer->SetZoom(m_zoom);
		}
	}

	void CameraComponent::Update(float deltaTime){
		
		m_entity->SetPosition( m_entity->GetPosition() + m_movement * 7.0f );

		auto pos = m_entity->GetPosition();
		auto wh = m_renderer->GetScreenDimensions();
		m_renderer->SetViewMatrix(CreateTopDownViewMatrix(pos, m_zoom, wh.x , wh.y));


		m_renderer->SetPosition(pos);

	}
	
	void CameraComponent::LoadProperties(const rapidjson::Value& inObj){
		Component::LoadProperties(inObj);
		json::GetFloat(inObj, "zoom", m_zoom);	
	}
	void CameraComponent::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const{
		Component::SaveProperties(alloc, inObj);
		json::AddFloat(alloc, inObj, "zoom", m_zoom);
	}

}
