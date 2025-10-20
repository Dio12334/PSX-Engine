#ifndef SCRIPTABLEENTITY_H
#define SCRIPTABLEENTITY_H

#include "entity.h"

namespace psx {


	class ScriptableEntity{
		public:
			virtual ~ScriptableEntity(){}

			template <typename T>
			T& GetComponent(){
				return m_entity.GetComponent<T>();
			}

			class Scene* GetScene()const {
				return m_entity.GetScene();
			}

		protected:
			virtual void OnCreate() {}
			virtual void OnDestroy() {}
            virtual void OnInput() {}
			virtual void OnUpdate(float dt) {}


		private:
			Entity m_entity;
			friend class Scene;
	};
}
#endif

