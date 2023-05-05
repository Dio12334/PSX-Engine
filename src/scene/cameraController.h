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

			void OnUpdate(float dt) override {
				auto& t = GetComponent<TransformComponent>();
				float speed = 700.f;
				
				if(UserInput::GetKeyValue(KeyCode::A)){
					t.Translation.x -= speed * dt;
				}
				if(UserInput::GetKeyValue(KeyCode::S)){
					t.Translation.y -= speed * dt;
				}
				if(UserInput::GetKeyValue(KeyCode::D)){
					t.Translation.x += speed * dt;
				}
				if(UserInput::GetKeyValue(KeyCode::W) ){
					t.Translation.y += speed * dt;
				}

				auto& c = GetComponent<CameraComponent>();
				
				auto scroll = UserInput::GetScrollWheel().y;
				if(scroll){
					int zoomStep = 20;
					auto cameraZoom = c.camera.GetOrthographicSize();
					if(scroll > 0){
						cameraZoom = !(cameraZoom - zoomStep > 0)? 1: cameraZoom - zoomStep;
					}
					else{
						cameraZoom += zoomStep;
					}
					c.camera.SetOrthographicSize(cameraZoom);
				}
				
			}
	};
}

#endif

