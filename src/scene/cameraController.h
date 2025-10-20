#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "scriptableEntity.h"
#include "components.h"
#include "scene.h"
#include "../inputSystem/userInput.h"
#include "../renderer/renderer.h"
#include <iostream>

namespace psx {
	class CameraController: public ScriptableEntity{
		public:
            glm::vec3 direction;
            float scroll = 0;

            void OnInput() override {
            }

			void OnUpdate(float dt) override {
				auto& t = GetComponent<TransformComponent>();
				float speed = 700.f;
				
                direction = glm::vec3(0);
                if(UserInput::GetKeyValue(KeyCode::A)){
                    direction.x -= 1.f;
                }

                if(UserInput::GetKeyValue(KeyCode::S)){
                    direction.y -= 1.f;
                }

                if(UserInput::GetKeyValue(KeyCode::D)){
                    direction.x += 1.f;
                }

                if(UserInput::GetKeyValue(KeyCode::W) ){
                    direction.y += 1.f;
                }

                scroll = UserInput::GetScrollWheel().y;
				
				t.Translation += direction * speed * dt;
				
				auto& c = GetComponent<CameraComponent>();
                int zoomStep = 20;
                auto cameraZoom = c.camera.GetOrthographicSize();

                if(scroll > 0){
                    cameraZoom = cameraZoom - zoomStep <= 0 ? 1 : cameraZoom - zoomStep;
                }
                else if(scroll < 0){
                    cameraZoom += zoomStep;
                }
                c.camera.SetOrthographicSize(cameraZoom);
			}
	};
}

#endif

