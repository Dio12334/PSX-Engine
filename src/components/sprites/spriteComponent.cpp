#include "spriteComponent.h"
#include "../../entity/entity.h"
#include "../../engine/engine.h"
#include "../../renderer/renderer.h"
#include "../../renderer/shader.h"
#include "../../math/psxangles.h"
#include "../../renderer/texture.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "../../fileSystem/jsonHelper.h"
#include <iostream>

namespace psx {

	INITIALIZE_COMPONENT_TYPE(SpriteComponent);
	SpriteComponent::SpriteComponent(class Entity* owner, int drawOrder): 
		Component(owner),
		m_texture(nullptr),
		m_drawOrder(drawOrder),
		m_texHeight(0),
		m_texWidth(0)
	{
		owner->GetEngine()->GetRenderer()->AddSprite(this);	
	}

	SpriteComponent::~SpriteComponent(){
		m_entity->GetEngine()->GetRenderer()->RemoveSprite(this);
	}

	void SpriteComponent::Draw(class Shader* shader){
		if(m_texture){
			auto scaleMat = glm::scale(glm::mat4(1.f),glm::vec3(static_cast<float>(m_texWidth), static_cast<float>(m_texHeight), 1.0f));
			auto world = m_entity->GetWorldTransform() * scaleMat;
			shader->SetMatrixUniform("uWorldTransform", world);
			m_texture->SetActive();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	
		}
	}

	void SpriteComponent::SetTexture(class Texture* input){
		m_texture = input;
		m_texWidth = m_texture->GetWidth();
		m_texHeight = m_texture->GetHeight();
	}

	void SpriteComponent::LoadProperties(const rapidjson::Value& inObj){
		Component::LoadProperties(inObj);
		std::string fileName;
		if(json::GetString(inObj, "fileName", fileName)){
			SetTexture(m_entity->GetEngine()->GetRenderer()->LoadTexture(fileName));
		}
		json::GetInt(inObj, "drawOrder", m_drawOrder);

	}
			
	void SpriteComponent::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const {
		Component::SaveProperties(alloc, inObj);
		if(m_texture){
			json::AddString(alloc, inObj, "fileName", m_texture->GetFileName());
		}
		json::AddInt(alloc, inObj, "drawOrder", m_drawOrder);

	}
}
