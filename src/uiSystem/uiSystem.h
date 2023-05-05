#ifndef UISYSTEM_H
#define UISYSTEM_H

#include "imguiHeaders.h"
#include <SDL2/SDL.h>

namespace psx {

	class UISystem{
		public:
			static void Initialize(SDL_Window* window, SDL_GLContext context);
			static void ProcessEvent(SDL_Event* event);
			static void StartFrame();
			static void BeginRender();
			static void EndRender();
			static void Shutdown();
		private:
			static ImGuiIO* s_io;
	};

}

#endif

