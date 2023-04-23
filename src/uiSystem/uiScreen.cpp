#include "uiScreen.h"
#include "button.h"
#include "../engine/engine.h"
#include "../renderer/renderer.h"
#include "../renderer/texture.h"
#include "../inputSystem/inputSystem.h"
#include "../renderer/font.h"
#include "../renderer/shader.h"
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_stdinc.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace psx {
	UIScreen::UIScreen(class Engine* engine): 
		m_engine(engine),
		m_title(nullptr),
		m_background(nullptr),
		m_titlePos(0.f, 300.f),
		m_nextButtonPos(0.f, 200.f),
		m_BGPos(0.f, 250.f),
		m_state(sActive){
			m_engine->PushUI(this);
			m_font = m_engine->GetFont("assets/Carlito-Regular.ttf");
			m_buttonOn = m_engine->GetRenderer()->LoadTexture("assets/ButtonYellow.png");
			m_buttonOff = m_engine->GetRenderer()->LoadTexture("assets/ButtonBlue.png");
		}

	UIScreen::~UIScreen(){
		if(m_title){
			m_title->Unload();
			delete m_title;
		}

		for(auto b: m_buttons){
			delete b;
		}
		m_buttons.clear();
	}

	void UIScreen::Update(float deltaTime){
			
	}

	void UIScreen::Draw(class Shader *shader){
		if(m_background){
			DrawTexture(shader, m_background, m_BGPos,1.f, m_engine->GetRenderer()->GetZoom());
		}
		if(m_title){
			DrawTexture(shader, m_title, m_titlePos,1.f, m_engine->GetRenderer()->GetZoom());
		}

		for(auto b: m_buttons){
			auto tex = b->GetHighlighted()? m_buttonOn : m_buttonOff;
			DrawTexture(shader, tex, b->GetPosition());
			DrawTexture(shader, b->GetNameTex(), b->GetPosition(), 1.f, m_engine->GetRenderer()->GetZoom());
		}
	}

	void UIScreen::ProcessInput(const struct InputState &state){
		if(!m_buttons.empty()){
			auto mousePos = state.mouse.GetPosition();
			auto screenDim = m_engine->GetRenderer()->GetScreenDimensions();
			mousePos.x -= screenDim.x * 0.5f;
			mousePos.y = screenDim.y * 0.5f - mousePos.y;

			for(auto b : m_buttons){
				if(b->CointainsPoint(mousePos)){
					b->SetHighlighted(true);
				}
				else{
					b->SetHighlighted(false);
				}
			}
		}
	}
	
	void UIScreen::HandleKeyPress(i32 key){
		switch(key){
			case static_cast<i32>(MouseButton::LEFT):
				if(!m_buttons.empty()){
					for(auto b: m_buttons){
						if(b->GetHighlighted()){
							b->OnClick();
							break;
						}
					}
				}
				break;
			default:
				break;
		}
	}

	void UIScreen::Close(){
		m_state = sClosing;
	}

	void UIScreen::SetTitle(const std::string &text, const Color& color, i32 pointSize){
		if(m_title){
			m_title->Unload();
			delete m_title;
			m_title = nullptr;
		}
		m_title = m_font->RenderText(text, color, pointSize);
	}

	void UIScreen::AddButton(const std::string &name, std::function<void ()> onClick){
		Vec2f dims(static_cast<float>(m_buttonOn->GetWidth()), static_cast<float>(m_buttonOn->GetHeight()));
		auto b = new Button(name, m_font, onClick, m_nextButtonPos, dims);
		m_buttons.emplace_back(b);
		m_nextButtonPos.y -= m_buttonOff->GetHeight() + 20.f;
	}

	
	void UIScreen::DrawTexture(class Shader* shader, class Texture* texture, const Vec2f& offset, float scale, float zoom){
		auto scaleMat = glm::scale(glm::mat4(1.f), 
				glm::vec3(
						static_cast<float>(texture->GetWidth()) * scale,
						static_cast<float>(texture->GetHeight())* scale,
						1.f
					));
		auto position = offset + m_engine->GetRenderer()->GetPosition();
		auto transMat = glm::translate(glm::mat4(1.f), glm::vec3(position.x, position.y, 0.f));
		auto world = transMat * scaleMat;
		shader->SetMatrixUniform("uWorldTransform", world);
		texture->SetActive();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

	void UIScreen::SetRelativeMouseMode(bool relative){
		if(relative){
			SDL_SetRelativeMouseMode(SDL_TRUE);
			SDL_GetRelativeMouseState(nullptr, nullptr);
		}
		else{
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}
	}

}
