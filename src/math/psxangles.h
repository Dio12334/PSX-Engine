#ifndef PSXANGLES_H
#define PSXANGLES_H

#include <cmath>

namespace psx {
	
	inline constexpr float ToDegrees(float radians){
		return radians*180.f/(2*M_PI);
	}

	inline constexpr float ToRadians(float degrees){
		return degrees*(2*M_PI)/180.f;
	}
}

#endif

