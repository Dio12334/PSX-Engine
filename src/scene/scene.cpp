#include "scene.h"
#include "entity.h"
#include "components.h"
#include "scriptableEntity.h"
#include "../renderer/renderer.h"
#include "../uiSystem/uiSystem.h"

namespace psx {



	Scene::Scene(class Engine* engine): m_engine(engine){
	
	}

	Scene::~Scene(){

	}


	Entity Scene::CreateEntity(const std::string& name){
		Entity entity = {m_registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty()? "Entity": name;
		return entity;
	}


	void Scene::DestroyEntity(Entity entity){
		m_registry.destroy(entity);
	}
	
	void Scene::OnUpdate(float dt){


        //Update UI
        {


        }

		// Update scripts
		{
			m_registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc){

						for(auto instance: nsc.Instances){

							if(!instance->m_entity.GetScene()){
								instance->m_entity = { entity, this };
								instance->OnCreate();
							}

							instance->OnUpdate(dt);
						}
				});
		}

		//Physics
		{
				
		}

		{
		
			Camera* mainCamera = nullptr;
			glm::mat4 cameraTransform;
			{
				auto view = m_registry.view<TransformComponent, CameraComponent>();
				for(auto entity: view){
					auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
					if(camera.Primary){
						mainCamera = &camera.camera;
						cameraTransform = transform.GetTransform();
						break;
					}
				}
			}

			if(mainCamera){
				Renderer::BeginScene(*mainCamera, cameraTransform);
                UISystem::BeginRender();

                auto group = m_registry.group<TransformComponent>(entt::get<SpriteComponent>);
                group.sort<SpriteComponent>([](const auto& lhs, const auto& rhs){
                    return lhs.DrawOrder < rhs.DrawOrder;
                });
                for(auto entity : group){
                    auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
                    Renderer::DrawSprite(transform, sprite);
                }

                UISystem::EndRender();

                Renderer::EndScene();
			}
		}		
	}

	void Scene::OnViewPortResize(u32 width, u32 height){
		if (m_viewportWidth == width && m_viewportHeight == height)
			return;

		m_viewportWidth = width;
		m_viewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.camera.SetViewportSize(width, height);
		}	
	}
}
