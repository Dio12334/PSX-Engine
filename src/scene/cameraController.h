#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "scriptableEntity.h"
#include "components.h"
#include "scene.h"
#include "../engine/engine.h"
#include "../inputSystem/inputSystem.h"
#include "../renderer/renderer.h"
#include <iostream>

namespace psx {
	class CameraController: public ScriptableEntity{
		public:

			void OnUpdate(float dt) override {
				auto& t = GetComponent<TransformComponent>();
				float speed = 700.f;
				auto is = GetScene()->GetEngine()->GetInputSystem();
				auto ks = is->GetState().keyboard;
				if(ks.GetKeyValue(KeyCode::A)){
					t.Translation.x -= speed * dt;
				}

				if(ks.GetKeyValue(KeyCode::S)){
					t.Translation.y -= speed * dt;
				}
				if(ks.GetKeyValue(KeyCode::D)){
					t.Translation.x += speed * dt;
				}
				if(ks.GetKeyValue(KeyCode::W) ){
					t.Translation.y += speed * dt;
				}
				
				auto renderer = GetComponent<CameraComponent>().renderer;
				renderer->SetZoom(1.f);
				auto wh = renderer->GetScreenDimensions();
				renderer->SetViewMatrix(CreateTopDownViewMatrix(Vec2f(t.Translation.x, t.Translation.y), renderer->GetZoom(), wh.x, wh.y));
				renderer->SetPosition(Vec2f(t.Translation.x, t.Translation.y));
			}
	};
}

#endif

