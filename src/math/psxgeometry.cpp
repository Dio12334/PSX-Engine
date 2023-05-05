#include "psxgeometry.h"
#include "psxvector.h"
#include <array>
#include <algorithm>

namespace psx {
	
	
	Vec2f LineSegment2D::PointOnSegment(float t) const { 
		return start + (end - start)*t;
	}
		
	float LineSegment2D::MinDistSq(const Vec2f& point) const {
			
		auto ab = end - start;
		auto ba = -1.f * ab;
		auto ac = point - start;
		auto bc = point - end;

		if(dotProduct(ab, ac) < 0.f){
			return ac.squaredLength();
		}
		else if(dotProduct(ba, bc) < 0.f){
			return bc.squaredLength();
		}
		else{
			float scalar = dotProduct(ac, ab);
			auto p = scalar * ab;
			return (ac - p).squaredLength();
		}
	}


	bool Circle::Contains(const Vec2f& point) const {
		return (center - point).squaredLength() <= (radius * radius);
	}

	void AABB::Update(const Vec2f& point){
		min.x = std::min(point.x, min.x);
		min.y = std::min(point.y, min.y);
		max.x = std::max(point.x, max.x);
		max.y = std::max(point.y, max.y);
	}

	void AABB::Rotate(float angle){
		std::array<Vec2f, 4> points;
		
		points[0] = min;
		points[1] = Vec2f(min.x, max.y); 
		points[2] = max;
		points[3] = Vec2f(max.x, min.y);

		auto p = rotateVector(points[0], angle);
		min = p;
		max = p;

		for(std::size_t i = 1; i < points.size(); ++i){
			p = rotateVector(points[i], angle);
			Update(p);
		}
	}

	bool AABB::Contains(const Vec2f& point) const {
		return point.x >= min.x &&
			point.y >= min.y &&
			point.x <= max.x &&
			point.y <= max.y;
	}

	float AABB::MinDistSq(const Vec2f& point) const {
		auto dx = std::max(min.x - point.x, 0.f);
		dx = std::max(dx, point.x - max.x);

		auto dy = std::max(min.y - point.y, 0.f);
		dy = std::max(dy, point.y - max.y);

		return dx*dx + dy*dy;
	}

	bool ConvexPolygon::Contains(const Vec2f &point) const {
		float sum = 0.f;
		Vec2f a, b;
		for(std::size_t i = 0; i < vertices.size() - 1; ++i){
			a = vertices[i] - point;
			a.normalize();
			b = vertices[i + 1] - point;
			b.normalize();

			sum += std::acos(dotProduct(a, b));
		}
		a = vertices.back() - point;
		a.normalize();
		b = vertices.front() - point;
		b.normalize();
		sum += std::acos(dotProduct(a, b));
		return nearZero(sum - 2*M_PI);
	}


	bool Intersect(const Circle& a, const Circle& b){
		return (a.center - b.center).squaredLength() <= (a.radius + b.radius)*(a.radius + b.radius);
	}

	bool Intersect(const AABB& a, const AABB& b){
		return a.max.x >= b.min.x &&
			a.max.y >= b.min.y &&
			b.max.x >= a.min.x &&
			b.max.y >= a.min.y;
	}

	bool Intersect(const Circle& a, const AABB& b){
		return b.MinDistSq(a.center) <= (a.radius * a.radius);
	}

	bool Intersect(const AABB& a, const Circle& b){
		return Intersect(b,a);
	}

	
	bool Intersect(const LineSegment2D& a, const LineSegment2D& b, float& t){
		auto p1 = a.start;
		auto p2 = a.end;
		auto p3 = b.start;
		auto p4 = b.end;

		auto tp = ((p1.x - p3.x)*(p3.y - p4.y) - (p1.y - p3.y)*(p3.x - p4.x))/
			((p1.x - p2.x)*(p3.y - p4.y) - (p1.y - p2.y)*(p3.x - p4.x));

		if(tp >= 0.f && tp <= 1.f){
			t = tp;
			return true;
		}
		return false;
	}

	bool Intersect(const LineSegment2D& l, const Circle& c, float& t){
		auto X = l.start - c.center;
		auto Y = l.end - l.start;
		
		auto a = dotProduct(Y, Y);
		auto b = 2.f * dotProduct(X, Y);
		auto c_c = dotProduct(X, X) - c.radius*c.radius;
		
		auto disc = b*b - 4.f*a*c_c;
		
		if(disc < 0.f){
			return false;
		}
		else{
			disc = std::sqrt(disc);
			auto tmin = (-b - disc)/(2.f*a);
			auto tmax = (-b + disc)/(2.f*a);

			if(tmin >= 0.f && tmin <= 1.f){
				t = tmin;
				return true;
			}
			else if(tmax >= 0.f && tmax <= 1.f){
				t = tmax;
				return true;
			}
			else{
				return false;
			}
		}
	}

	bool Intersect(const LineSegment2D& l, const AABB& a, float& t, Vec2f& n){
		auto p1 = a.min, p2 = Vec2f(a.min.x, a.max.y), 
			 p3 = a.max, p4 = Vec2f(a.max.x, a.min.y);

		auto l1 = LineSegment2D(p1, p2), l2 = LineSegment2D(p2, p3),
			 l3 = LineSegment2D(p3, p4), l4 = LineSegment2D(p4, p1);
		
		std::array<float, 4> tValues;
		bool intersect = false;

		intersect |= Intersect(l, l1, tValues[0]);	
		intersect |= Intersect(l, l2, tValues[1]);
		intersect |= Intersect(l, l3, tValues[2]);
		intersect |= Intersect(l, l4, tValues[3]);
		
		if(!intersect){
			return false;
		}

		std::sort(tValues.begin(), tValues.end());
		for(auto it : tValues){
			p1 = l.PointOnSegment(it);
			if(a.Contains(p1)){
				t = it;
				return true;
			}
		}
		return false;
	}

}
