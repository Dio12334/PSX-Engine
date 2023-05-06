#ifndef UISYSTEM_H
#define UISYSTEM_H

#include "imguiHeaders.h"
#include <functional>
#include <iterator>
#include <vector>
#include <SDL2/SDL.h>
#include <memory>
#include "../utils/psxint.h"

namespace psx {


    class UIScreen;

	class UIWrapper{
		public:
			std::function<void()> ShowScreen;
			UIWrapper(UIScreen* screen);
			
			~UIWrapper();
		private:
			std::shared_ptr<UIScreen> m_screen;
			friend class UIScreen;

	};

	class UISystem{
		public:
			static void Initialize(SDL_Window* window, SDL_GLContext context);
			static void ProcessEvent(SDL_Event* event);
			static void StartFrame();
			static void BeginRender();
			static void EndRender();
			static void Shutdown();

            static void ShowScreens();
		private:
            friend class UIScreen;
			static ImGuiIO* s_io;
            static std::vector<UIWrapper> m_uiStack;

	};

    class UIScreen{
    public:
        UIScreen(const std::string& name = ""): m_windowName(name){

        }
		template <typename T, typename... Args>
        static void AddToStack(Args&&... args){
            UIWrapper wr(new T(std::forward<Args>(args)...));
            auto it = std::find_if(UISystem::m_uiStack.begin(), UISystem::m_uiStack.end(),
                                   [&wr](const UIWrapper& wrapper) {
                                       return wr.m_screen->m_windowName == wrapper.m_screen.get()->m_windowName;
                                   });
            if(it != UISystem::m_uiStack.end()){
                return ;
            }
            UISystem::m_uiStack.push_back(wr);
        }

		static void RemoveFromStack(const UIScreen& screen) {
			auto it = std::find_if(UISystem::m_uiStack.begin(), UISystem::m_uiStack.end(),
                           [&screen](const UIWrapper& wrapper) {
                               return &screen == wrapper.m_screen.get();
                           });
			if (it != UISystem::m_uiStack.end()) {
				UISystem::m_uiStack.erase(it);
			}
		}

        virtual void ShowScreen() {}
		virtual ~UIScreen() {}

	protected:
        std::string m_windowName;
    };
}

#endif

