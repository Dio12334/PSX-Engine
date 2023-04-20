#ifndef PSXVECTOR_H
#define PSXVECTOR_H

#include <ostream>
#include <type_traits>
#include <cmath>

namespace psx {
	  	template <typename T = float>
		struct Vec2 final{

        static_assert(std::is_arithmetic<T>::value, "T must be arithmetic");
        static_assert(!std::is_const<T>::value, "T must not be const");
        
			T x, y;
        
			typedef T component_type;

			Vec2(): x(0), y(0){
            
        }

        explicit Vec2(T inX, T inY): x(inX), y(inY){

        }


        template <typename U>
        Vec2(const Vec2<U>& other): x(other.x), y(other.y){
        
        }

        template <typename U>
        Vec2& operator=(const Vec2<U>& other){
            x = other.x;
            y = other.y;

            return *this;
        }

        Vec2(const Vec2&) = default;

        Vec2& operator=(const Vec2& other) = default;

        Vec2(Vec2&& other) = default;

        Vec2& operator=(Vec2&& other) = default;

        ~Vec2() = default;
		
		template <typename U>
        Vec2& operator+=(const Vec2<U>& other){ 
            x += other.x;
            y += other.y;

            return *this;
        }

		template <typename U>
        Vec2& operator-=(const Vec2<U>& other){
            x -= other.x;
            y -= other.y;

            return *this;
        }

        template <typename U>
        Vec2& operator*=(const U& number){
            x *= number;
            y *= number;

            return *this;
        }

		template <typename U>
        Vec2& operator/=(const U& number){
            x /= number;
            y /= number;

            return *this;
        }

        double length() const {
            return std::sqrt(x*x + y*y);
        }

		double squaredLength() const {
			return x*x + y*y;
		}

        Vec2<double> normal() const {
            auto len = (length())? length(): 1;
            return Vec2<double>(x/len, y/len);
        }

        Vec2& normalize() {
            auto len = (length())? length(): 1;
            x /= len;
            y /= len;

            return *this;
        }
		

    };

		
    template <typename T, typename U, typename P = typename std::common_type<T, U>::type>
    Vec2<P> operator+(const Vec2<T>& lv, const Vec2<U>& rv){
        Vec2<P> result(lv);
        result += rv;
        return result;
    }

    template <typename T, typename U, typename P = typename std::common_type<T, U>::type>
    Vec2<P> operator-(const Vec2<T>& lv, const Vec2<U>& rv){
        Vec2<P> result(lv);
        result -= rv;
        return result;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Vec2<T>& vec){
        os << vec.x << " " << vec.y;
        return os;
    }

    template <typename T, typename U>
    auto dotProduct(const Vec2<T>& lv, const Vec2<U>& rv) -> decltype(lv.x*rv.x){
        return lv.x * rv.x + lv.y * rv.y ;
    }

	template <typename T, typename U, typename P = typename std::common_type<T, U>::type>
	Vec2<P> operator*(const Vec2<T>& lv, const Vec2<U>& rv){
		return Vec2<P>(lv.x*rv.x, lv.y*rv.y);
	}

       template <typename T, typename U, typename = typename std::enable_if_t<std::is_arithmetic<U>::value>>
    Vec2<T> operator*(const Vec2<T>& vec, const U& number){
        Vec2<T> result(vec);
        result *= number;
        return result;
    }

    template <typename T, typename U, typename = typename std::enable_if_t<std::is_arithmetic<U>::value>>
    Vec2<T> operator*(const U& number, const Vec2<T>& vec){
        Vec2<T> result(vec);
        result *= number;
        return result;
    }

	template <typename T, typename U, typename = typename std::enable_if_t<std::is_arithmetic<U>::value>>
    Vec2<T> operator/(const Vec2<T>& vec, const U& number){
        Vec2<T> result(vec);
        result /= number;
        return result;
    }

    template <typename T, typename U, typename = typename std::enable_if_t<std::is_arithmetic<U>::value>>
    Vec2<T> operator/(const U& number, const Vec2<T>& vec){
        Vec2<T> result(vec);
        result /= number;
        return result;
    }
    template <typename T>
    Vec2<double> normalize(const Vec2<T>& vec){
        return vec.normal();
    }

	template <typename T>
	double length(const Vec2<T>& vec){
		return vec.length();
	}

	template <typename T>
	double squaredLength(const Vec2<T>& vec){
		return vec.squaredLength();
	}

    template <typename T, typename U>
    bool operator==(const Vec2<T>& lv, const Vec2<U>& rv){
        return lv.x == rv.x && lv.y == rv.y;
    }

    template <typename T, typename U>
    bool operator!=(const Vec2<T>& lv, const Vec2<U>& rv){
        return !(lv == rv);
    }

    template <typename T>
    using Point2 = Vec2<T>;

	using Vec2f = Vec2<>;
	



	inline double toDegrees(double radians){
		return 180.0/M_PI *radians; 
	}

	inline double toRadians(double degrees){
		return M_PI/180.0 * degrees;
	}

	inline bool nearZero(double value, double closeToZero = 0.001){
		return std::abs(value) <= closeToZero;
	}

	template <typename T>
	inline Vec2<T> rotateVector(const Vec2<T>& vec, float angle){
		float s = std::sin(angle);
		float c = std::cos(angle);
		float nx = vec.x*c - vec.y*s;
		float ny = vec.x*s + vec.y*c;

		return Vec2<T>(nx, ny);
	}

}

#endif

