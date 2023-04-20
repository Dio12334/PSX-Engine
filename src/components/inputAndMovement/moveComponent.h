#ifndef MOVECOMPONENT_H
#define MOVECOMPONENT_H

#include "../component.h"

namespace psx {
	class MoveComponent: public Component{
		public:
			MoveComponent(class Entity* entity, i32 updateOrder = 10);
			void Update(float deltaTime) override;
			float GetAngularSpeed() const { return m_angularSpeed; }
			float GetForwardSpeed() const { return m_forwardSpeed; }
			void SetAngularSpeed(float speed) { m_angularSpeed = speed; }
			void SetForwardSpeed(float speed) { m_forwardSpeed = speed; }

		private:

			float m_angularSpeed;
			float m_forwardSpeed;
	};
}

#endif

