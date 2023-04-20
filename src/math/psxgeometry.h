#ifndef PSXGEOMETRY_H
#define PSXGEOMETRY_H
#include "psxvector.h"
#include <vector>

namespace psx {
	namespace math{
		const auto INF = std::numeric_limits<float>::infinity();
		const auto NEGINF = -std::numeric_limits<float>::infinity();
	}
	struct LineSegment2D{
		Vec2f start;
		Vec2f end;

		LineSegment2D(const Vec2f& i_start, const Vec2f& i_end): 
			start(i_start),
			end(i_end){

			}

		Vec2f PointOnSegment(float t) const ;
		float MinDistSq(const Vec2f& point) const ;
	};

	struct Circle{
		Vec2f center;
		float radius;

		Circle(const Vec2f& i_center, float i_radius): 
			center(i_center), 
			radius(i_radius){

			}

		bool Contains(const Vec2f& point) const ;
	};

	struct AABB{
		Vec2f min;
		Vec2f max;

		AABB(const Vec2f& i_min, const Vec2f& i_max):
			min(i_min),
			max(i_max){

			}
		void Update(const Vec2f& point);
		void Rotate(float angle);
		bool Contains(const Vec2f& point) const ;
		float MinDistSq(const Vec2f& point) const ;
	};

	struct OBB{
		Vec2f center;
		float rotation;
		Vec2f extents;
	};

	struct ConvexPolygon{
		std::vector<Vec2f> vertices;
		bool Contains(const Vec2f& point) const;
	};

	bool Intersect(const Circle& a, const Circle& b);
	bool Intersect(const AABB& a, const AABB& b);
	bool Intersect(const Circle& a, const AABB& b);
	bool Intersect(const AABB& a, const Circle& b);

	bool Intersect(const LineSegment2D& a, const LineSegment2D& b, float& t);
	bool Intersect(const LineSegment2D& l, const Circle& c, float& t);
	bool Intersect(const LineSegment2D& l, const AABB& a, float& t, Vec2f& n);
}
#endif

