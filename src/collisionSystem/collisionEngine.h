#ifndef COLLISIONENGINE_H
#define COLLISIONENGINE_H

#include "../math/psxvector.h"
#include <vector>
#include <functional>

namespace psx {
	class CollisionEngine{
		public:
			
			struct CollisionInfo{
				Vec2f point;
				Vec2f normal;
				class BoxComponent* box;
				class Entity* entity;
			};

			CollisionEngine(class Engine* engine);
			void AddBox(class BoxComponent* box);
			void RemoveBox(class BoxComponent* box);
			bool SegmentCast(const class LineSegment2D& l, CollisionInfo& colInfo);
			void testSweepAndPrune(std::function<void(class Entity*, class Entity*)> f);
		private:
			class Engine* m_engine;
			std::vector<class BoxComponent*> m_boxes;
	};
}

#endif

