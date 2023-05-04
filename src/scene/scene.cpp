#include "scene.h"
#include "entity.h"
#include "components.h"
#include "scriptableEntity.h"

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
	
	void Scene::OnUpdate(float dt){
		
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

		// lets draw some shit
		{
			
		}
	}
}
