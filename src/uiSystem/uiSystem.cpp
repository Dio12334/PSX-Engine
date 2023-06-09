#include "uiSystem.h"
#include <GL/glew.h>

namespace psx {

    ImGuiIO* UISystem::s_io = nullptr;
	std::vector<UIWrapper> UISystem::m_uiStack;

	void UISystem::Initialize(SDL_Window* window, SDL_GLContext context){
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		s_io = &ImGui::GetIO();

		ImGui::StyleColorsDark();
		ImGui_ImplSDL2_InitForOpenGL(window, context);
		ImGui_ImplOpenGL3_Init("#version 130");
	}
	
	void UISystem::ProcessEvent(SDL_Event* event){
		ImGui_ImplSDL2_ProcessEvent(event);
	}
	
	void UISystem::StartFrame(){
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
	}

	void UISystem::BeginRender(){
		ImGui::Render();
		glViewport(0, 0, (int)(*s_io).DisplaySize.x, (int)(*s_io).DisplaySize.y);
	}
	
	void UISystem::EndRender(){
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	
	void UISystem::Shutdown(){
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

    void UISystem::ShowScreens() {

        for(auto screen: m_uiStack){
            screen.ShowScreen();
        }
    }
	
	UIWrapper::UIWrapper(UIScreen* screen): m_screen(screen){
		ShowScreen = [screen](){
			screen->ShowScreen(); 
			};
	}

	UIWrapper::~UIWrapper(){			
	}

}
