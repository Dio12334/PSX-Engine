#include "aiComponent.h"
#include "aiState.h"
#include <SDL2/SDL_log.h>

namespace psx {
	
	AIComponent::AIComponent(class Entity* owner):
		Component(owner),
		m_currentState(nullptr){

		}

	void AIComponent::Update(float deltaTime){
		if(m_currentState){
			m_currentState->Update(deltaTime);
		}
	}

	void AIComponent::ChangeState(const std::string &name){
		if(m_currentState){
			m_currentState->OnExit();
		}

		auto iter = m_stateMap.find(name);
		if(iter != m_stateMap.end()){
			m_currentState = iter->second;
			m_currentState->OnEnter();
		}
		else{
			SDL_Log("Could not find AIState %s in state map", name.c_str());
			m_currentState = nullptr;
		}
	}

	void AIComponent::RegisterState(class AIState *state){
		m_stateMap.emplace(state->GetName(), state);
	}
}
