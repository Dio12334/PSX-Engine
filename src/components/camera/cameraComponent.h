#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "../component.h"

namespace psx {
	class CameraComponent: public Component{
		public:
			DEFINE_COMPONENT_TYPE(CameraComponent);


			CameraComponent(class Entity* owner);
			~CameraComponent();
			void ProcessInput(const InputState& state) override;
			void Update(float deltaTime) override;
			
			void LoadProperties(const rapidjson::Value& inObj) override;
			void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const override;

		private:
			class Renderer* m_renderer;
			Vec2f m_movement;
			float m_zoom = 1;
	};
}

#endif

