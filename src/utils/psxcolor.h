#ifndef PSXCOLOR_H
#define PSXCOLOR_H

namespace psx {
	struct Color{
		float r = 0.f, g = 0.f, b = 0.f;
		
		Color(){

		}
		explicit Color(float ir, float ig, float ib):
			r(ir),
			g(ig),
			b(ib){

			}

		static const Color White;
		static const Color Black;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
	};
}

#endif

