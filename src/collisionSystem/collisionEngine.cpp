#include "collisionEngine.h"
#include "../math/psxgeometry.h"
#include "../components/component.h"
#include "../engine/engine.h"
#include "../components/collision/boxComponent.h"

namespace psx {

	CollisionEngine::CollisionEngine(class Engine* engine): 
		m_engine(engine){

		}

	void CollisionEngine::AddBox(class BoxComponent* box){
		m_boxes.push_back(box);
	}

	void CollisionEngine::RemoveBox(class BoxComponent* box){
		auto iter = std::find(m_boxes.begin(), m_boxes.end(), box);
		if(iter != m_boxes.end()){
			std::iter_swap(iter, m_boxes.end() - 1);
			m_boxes.pop_back();
		}
	}
			
	bool CollisionEngine::SegmentCast(const class LineSegment2D& l, CollisionInfo& colInfo){
		bool collided = false;

		auto closest = math::INF;
		Vec2f norm;

		for(auto box: m_boxes){
			float t;
			if(Intersect(l, box->GetWorldBox(), t, norm)){
				if(t < closest){
					colInfo.point = l.PointOnSegment(t);
					colInfo.normal = norm;
					colInfo.box = box;
					colInfo.entity = box->GetEntity();
					collided = true;
				}
			}
		}
		return collided;
	}
	
	void CollisionEngine::testSweepAndPrune(std::function<void(class Entity*, class Entity*)> f){
		std::sort(m_boxes.begin(), m_boxes.end(),
				[](BoxComponent* a, BoxComponent* b){ 
				return a->GetWorldBox().min.x < b->GetWorldBox().min.x; 
				});

		for(std::size_t i = 0; i < m_boxes.size(); ++i){
			auto a = m_boxes[i];
			auto max = a->GetWorldBox().max.x;
			for(std::size_t j = i + 1; j < m_boxes.size(); ++j){
				auto b = m_boxes[j];
				if(a->GetWorldBox().min.x > max){
					break;
				}
				else if(Intersect(a->GetWorldBox(), b->GetWorldBox())){
					f(a->GetEntity(), b->GetEntity());
				}
			}
		}
	}
	

}
