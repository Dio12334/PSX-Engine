#include "userInput.h"
#include <cmath>

namespace psx {

	InputSystem* UserInput::s_inputSystem = nullptr;
	bool UserInput::Init(class InputSystem* system){
		s_inputSystem = system;
		
		return (bool) s_inputSystem;
	}
	
	void UserInput::Shutdown(){
		s_inputSystem = nullptr;
	}

	bool UserInput::GetKeyValue(KeyCode keycode){
		return s_inputSystem->GetState().keyboard.GetKeyValue(keycode);
	}
	
	ButtonState UserInput::GetKeyState(KeyCode keycode){
		return s_inputSystem->GetState().keyboard.GetKeyState(keycode);
	}

	const Vec2f& UserInput::GetPosition(){
		return s_inputSystem->GetState().mouse.GetPosition();
	}

	const Vec2f& UserInput::GetScrollWheel(){
		return s_inputSystem->GetState().mouse.GetScrollWheel();
	}
			
	bool UserInput::IsRelative(){
		return s_inputSystem->GetState().mouse.IsRelative();
	}
	
	bool UserInput::GetButtonValue(MouseButton button){
		return s_inputSystem->GetState().mouse.GetButtonValue(button);
	}
	
	ButtonState UserInput::GetButtonState(MouseButton button){
		return s_inputSystem->GetState().mouse.GetButtonState(button);
	}
}
