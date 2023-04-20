#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "moveComponent.h"

namespace psx {
	class InputComponent: public MoveComponent{
		public:
			InputComponent(class Entity* entity);
			void ProcessInput(const InputState& state) override;
			float GetMaxForward() const { return m_maxForwardSpeed; }
			float GetMaxAngular() const { return m_maxAngularSpeed; }

		private:
			float m_maxForwardSpeed;
			float m_maxAngularSpeed;
	};
}

#endif

