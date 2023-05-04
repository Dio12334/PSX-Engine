#ifndef TESTSCENE_H
#define TESTSCENE_H

#include "scene.h"
#include "entity.h"
#include "components.h"
#include "../engine/engine.h"
#include "../renderer/renderer.h"
#include "cameraController.h"

namespace psx{
	class TestScene: public Scene{

		public:

			TestScene(class Engine* engine): Scene(engine) {}

			void LoadScene() override {
				auto bungo = CreateEntity("bungo");
				bungo.GetComponent<TransformComponent>().Translation = glm::vec3(200, 40, 0);
				bungo.AddComponent<SpriteComponent>(m_engine->GetRenderer()->LoadTexture("assets/bungo.png"));
				bungo.AddComponent<CameraComponent>(m_engine->GetRenderer());
				bungo.AddComponent<NativeScriptComponent>().Bind<CameraController>();


				bungo = CreateEntity("bungow??");
				bungo.GetComponent<TransformComponent>().Translation = glm::vec3(200, 30, 0);
				bungo.GetComponent<TransformComponent>().Rotation = M_PI/2;
				bungo.GetComponent<TransformComponent>().Scale = glm::vec3 {10.f};
				bungo.AddComponent<SpriteComponent>(m_engine->GetRenderer()->LoadTexture("assets/bungo.png"));

				bungo = CreateEntity("skeleton");
				bungo.GetComponent<TransformComponent>().Translation = glm::vec3(200, 30, 0);
				bungo.GetComponent<TransformComponent>().Scale = glm::vec3 {10.f};
				bungo.AddComponent<SpriteComponent>(m_engine->GetRenderer()->LoadTexture("assets/skeleton-sprite.png"));
			}
	};	
}

#endif

