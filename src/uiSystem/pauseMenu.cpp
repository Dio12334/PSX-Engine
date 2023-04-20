#include "pauseMenu.h"
#include "dialogBox.h"
#include "../engine/engine.h"
#include <SDL2/SDL.h>

namespace psx {
	PauseMenu::PauseMenu(Engine* engine):
		UIScreen(engine){

			m_engine->SetState(Engine::sPause);
			SetRelativeMouseMode(false);
			SetTitle("PauseTitle", Color::Black, 72);
			AddButton("ResumeButton", [this](){
						Close();
					});
			AddButton("QuitButton", [this](){
					new DialogBox(m_engine, "QuitText", [this](){
								m_engine->SetState(Engine::sQuit);
							});
					});
		}

	PauseMenu::~PauseMenu(){
		//SetRelativeMouseMode(true);
		m_engine->SetState(Engine::sGameplay);
	}

	void PauseMenu::HandleKeyPress(i32 key){
		UIScreen::HandleKeyPress(key);
		if(key == SDLK_ESCAPE){
			Close();
		}
	}
}

