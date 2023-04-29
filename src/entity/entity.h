#ifndef ENTITY_H
#define ENTITY_H

#include "../math/psxvector.h"
#include "../components/component.h"
#include <array>
#include <bitset>
#include <vector>
#include <glm/glm.hpp>
#include <rapidjson/document.h>

namespace psx {
	
	class Entity{	
		public:

			enum TypeID{
				tActor = 0,
				NUM_ACTOR_TYPES
			};

			static const char* typeNames[NUM_ACTOR_TYPES];

			virtual TypeID GetType() const { return tActor; }

			enum State{
				sActive,
				sPaused,
				sDead
			};

			Entity(class Engine* engine);
			virtual ~Entity();
			
			void Update(float deltaTime);
			void UpdateComponents(float deltaTime);
			virtual void UpdateEntity(float deltaTime);
			
			void SetState(State state) { m_state = state; }
			State GetState() const { return m_state; }
			
			void SetPosition(const Vec2f& pos) { m_position = pos; m_recomputeWorldTransform = true; }
			const Vec2f& GetPosition() const { return m_position; }
			
			void SetRotation(float rotation) { m_rotation = rotation; m_recomputeWorldTransform = true; }
			float GetRotation() const { return m_rotation; }
			
			void SetScale(float scale) { m_scale = scale; m_recomputeWorldTransform = true; }
			float GetScale() const { return m_scale; }
			
			Vec2f GetForward() const { return Vec2f(std::cos(m_rotation), std::sin(m_rotation)); }
			
			class Engine* GetEngine() const { return m_engine; }
			

			template <typename T>
			void AddComponent(T* component){
				int order = component->GetUpdateOrder();
				auto iter = m_components.begin();
				for(; iter != m_components.end(); ++iter){
					if(order < (*iter)->GetUpdateOrder()){
						break;
					}
				}
				m_components.insert(iter, component);
				m_componentArray[GetComponentTypeID<T>()] = component;
				m_componentBitSet[GetComponentTypeID<T>()] = true;
			}

			template <typename T>
			void RemoveComponent(T* component){
				auto iter = std::find(m_components.begin(), m_components.end(), component);
				if(iter != m_components.end()){
					m_components.erase(iter);
				}
				m_componentArray[GetComponentTypeID<T>()] = nullptr;
				m_componentBitSet[GetComponentTypeID<T>()] = false;

			}
			
			void ProcessInput(const struct InputState& state);
			virtual void EntityInput(const struct InputState& state);

			void ComputeWorldTransform();

			const glm::mat4 GetWorldTransform() const { return m_worldTransform; }

			virtual void LoadProperties(const rapidjson::Value& inObj);
			virtual void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;

			template <typename T>
			static Entity* Create(class Engine* engine, const rapidjson::Value& inObj){
				T* t = new T(engine);
				t->LoadProperties(inObj);
				return t;
			}
				
			Component* GetComponentOfType(Component::TypeID type){
				Component* comp = nullptr;
				for(Component* c: m_components){
					if(c->GetType() == type){
						comp = c;
						break;
					}
				}
				return comp;
			}

			const std::vector<class Component*>& GetComponents() const { return m_components;} 
			
			template <typename T>
			bool HasComponent() const {
				return m_componentBitSet[GetComponentTypeID<T>()];
			}

			template <typename T>
			T& GetComponent() const {
				auto ptr = m_componentArray[GetComponentTypeID<T>()];
				return *static_cast<T*>(ptr);
			}
		private:
			State m_state;
			Vec2f m_position;
			float m_scale;
			float m_rotation;
			std::vector<class Component*> m_components;
			std::bitset<g_maxComponents> m_componentBitSet;
			std::array<class Component*, g_maxComponents> m_componentArray;

			class Engine* m_engine;

			glm::mat4 m_worldTransform;
			bool m_recomputeWorldTransform;
	};
}

#endif

