#ifndef EVENT_H
#define EVENT_H

#include <SDL2/SDL_events.h>

namespace psx {

	class Event{

		public:
			Event() = default;

			SDL_Event& GetSDL_Event() { return m_event; }			

		private:

			SDL_Event m_event;
			bool handled = false;

	};

}

#endif

