#ifndef COMPONENT_H
#define COMPONENT_H

#include "../utils/psxint.h"
#include "../inputSystem/inputSystem.h"
#include "../fileSystem/levelLoader.h"
#include <rapidjson/document.h>
#include <vector>

namespace psx {

	class Component{
		public:

			/* enum TypeID{ */
			/* 	tComponent = 0, */
			/* 	tCameraComponent, */
			/* 	tMoveComponent, */
			/* 	tSpriteComponent, */
			/* 	NUM_COMPONENT_TYPES */
			/* }; */

			/* static const char* typeNames[NUM_COMPONENT_TYPES]; */
			typedef std::string TypeID;
			virtual TypeID GetType() const = 0;			

			Component(class Entity* owner, i32 updateOrder = 100);
			virtual ~Component();
			virtual void Update(float deltaTime);
			i32 GetUpdateOrder() const { return m_updateOrder; }
			class Entity* GetEntity() const { return m_entity; }
			virtual void ProcessInput(const InputState& state) {}
			virtual void OnUpdateWorldTransform() {}

			virtual void LoadProperties(const rapidjson::Value& inObj);
			virtual void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;

			template<typename T>
			static Component* Create(class Entity* entity, const rapidjson::Value& inObj){
				T* t = new T(entity);
				t->LoadProperties(inObj);
				return t;
			}
			
		protected:
			class Entity* m_entity;
			i32 m_updateOrder;
	};
}

#define DEFINE_COMPONENT_TYPE(ComponentType)\
	const static Component::TypeID s_type;\
	Component::TypeID GetType() const override { return s_type; }\
	static Component::TypeID GetComponentType() { return s_type; }

#define INITIALIZE_COMPONENT_TYPE(ComponentType)\
	const Component::TypeID ComponentType::s_type = #ComponentType;\
	bool t##ComponentType = LevelLoader::AddComponentFunc({ComponentType::GetComponentType(), &Component::Create<ComponentType>});

#endif

