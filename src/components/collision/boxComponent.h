#ifndef BOXCOMPONENT_H
#define BOXCOMPONENT_H

#include "../component.h"
#include "../../math/psxgeometry.h"
namespace psx {

	class BoxComponent: public Component{
		public:
			BoxComponent(class Entity* entity);
			~BoxComponent();
			void OnUpdateWorldTransform() override;
			void SetObjectBox(const AABB& model) { m_objectBox = model; }
			const AABB& GetWorldBox() const { return m_worldBox; }
			void SetShouldRotate(bool value) { m_shouldRotate = value; }

		private:
			AABB m_objectBox;
			AABB m_worldBox;
			bool m_shouldRotate;
	};
}

#endif

