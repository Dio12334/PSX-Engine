#ifndef AICOMPONENT_H
#define AICOMPONENT_H

#include "../component.h"
#include <unordered_map>
#include <string>

namespace psx {
	class AIComponent: public Component{
		public:
			AIComponent(class Entity* owner);
			void Update(float deltaTime) override;
			void ChangeState(const std::string& name);
			void RegisterState(class AIState* state);
		private:
			std::unordered_map<std::string, class AIState*> m_stateMap;
			class AIState* m_currentState;
	};
}

#endif

