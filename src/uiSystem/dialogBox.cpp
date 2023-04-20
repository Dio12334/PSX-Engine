#include "dialogBox.h"
#include "../engine/engine.h"
#include "../renderer/renderer.h"
#include <SDL2/SDL.h>

namespace psx {	
	DialogBox::DialogBox(class Engine* engine, const std::string& text, std::function<void()> onOK):
		UIScreen(engine){
			m_BGPos = Vec2f();
			m_titlePos = Vec2f(0.f, 100.f);
			m_nextButtonPos = Vec2f();

			m_background = m_engine->GetRenderer()->LoadTexture("assets/DialogBG.png");
			SetTitle(text, Color::Black, 30);
			AddButton("OKButton", [onOK](){
						onOK();
					});
			AddButton("CancelButton", [this](){
						Close();
					});
		}
	DialogBox::~DialogBox(){

	}
}
