#ifndef UISYSTEM_H
#define UISYSTEM_H

#include "imguiHeaders.h"
#include <SDL2/SDL.h>

namespace psx {
	class UISystem{
		public:
			void Initialize(SDL_Window* window, SDL_GLContext context);
			void ProcessEvent(SDL_Event* event);
			void StartFrame();
			void BeginRender();
			void EndRender();
			void Shutdown();

		private:
			ImGuiIO* m_io;
			
	};
}

#endif

