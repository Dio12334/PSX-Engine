#include "inputSystem.h"
#include <SDL2/SDL_mouse.h>
#include <cstring>

namespace psx {

	bool KeyboardState::GetKeyValue(KeyCode keyCode) const {
		return m_currState[static_cast<i32>(keyCode)] == 1;
	}

	ButtonState KeyboardState::GetKeyState(KeyCode keyCode) const {
		if(m_prevState[static_cast<i32>(keyCode)] == 0){
			if(m_currState[static_cast<i32>(keyCode)] == 0){
				return ButtonState::none;
			}
			else{
				return ButtonState::pressed;
			}
		}
		else{
			if(m_currState[static_cast<i32>(keyCode)] == 0){
				return ButtonState::released;
			}
			else{
				return ButtonState::held;
			}
		}
	}

	bool MouseState::GetButtonValue(MouseButton button) const {
		return (SDL_BUTTON(static_cast<i32>(button)) & m_currButtons) == 1;
	}

	ButtonState MouseState::GetButtonState(MouseButton button) const {
		i32 mask = SDL_BUTTON(static_cast<i32>(button));
		if((mask & m_prevButtons) == 0){
				if((mask & m_currButtons) == 0){
					return ButtonState::none;
				}
				else{
					return ButtonState::pressed;
				}
		}
		else{
			if((mask & m_currButtons) == 0){
				return ButtonState::released;
			}
			else{
				return ButtonState::held;
			}
		}
	}
	
	bool InputSystem::Initialize(){
		m_state.keyboard.m_currState = SDL_GetKeyboardState(NULL);
		memset(m_state.keyboard.m_prevState, 0, SDL_NUM_SCANCODES);
		m_state.mouse.m_currButtons = 0;
		m_state.mouse.m_prevButtons = 0;
		return true;
	}

	void InputSystem::Update(){
		i32 x = 0, y = 0;
		if(m_state.mouse.m_isRelative){
			m_state.mouse.m_currButtons = SDL_GetRelativeMouseState(&x, &y);
		}
		else{
			m_state.mouse.m_currButtons = SDL_GetMouseState(&x, &y);
		}
		m_state.mouse.m_mousePos.x = static_cast<float>(x);
		m_state.mouse.m_mousePos.y = static_cast<float>(y);
	}

	void InputSystem::SetRelativeMouseMode(bool value){
		SDL_bool set = value ? SDL_TRUE : SDL_FALSE;
		SDL_SetRelativeMouseMode(set);
		m_state.mouse.m_isRelative = value;
	}

	void InputSystem::ProcessEvent(union ::SDL_Event &event){
		switch(event.type){
			case SDL_MOUSEWHEEL:
				m_state.mouse.m_scrollWheel = Vec2(static_cast<float>(event.wheel.x), static_cast<float>(event.wheel.y));
				break;
			default:
				break;
		}
	}

	void InputSystem::Shutdown(){
	
	}

	void InputSystem::PrepareForUpdate(){
		memcpy(m_state.keyboard.m_prevState, m_state.keyboard.m_currState, SDL_NUM_SCANCODES);
		m_state.mouse.m_prevButtons = m_state.mouse.m_currButtons;
		m_state.mouse.m_scrollWheel = Vec2(0,0);
	}
}
