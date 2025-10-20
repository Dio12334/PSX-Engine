#include "userInput.h"
#include "../uiSystem/uiSystem.h"

namespace psx {

	bool UserInput::GetKeyValue(KeyCode keycode){
		if(UISystem::IsGettingKeyBoardInputs()){
			return false;
		}
		return InputSystem::GetState().keyboard.GetKeyValue(keycode);
	}
	
	ButtonState UserInput::GetKeyState(KeyCode keycode){
		if(UISystem::IsGettingKeyBoardInputs()){
			return ButtonState::none;
		}
		return InputSystem::GetState().keyboard.GetKeyState(keycode);
	}

	const Vec2f& UserInput::GetPosition(){
		return InputSystem::GetState().mouse.GetPosition();
	}

	const Vec2f& UserInput::GetScrollWheel(){
		if(UISystem::IsGettingMouseInputs()){
			static Vec2f returnableError(0,0);
			return returnableError;
		}
		return InputSystem::GetState().mouse.GetScrollWheel();
	}
			
	bool UserInput::IsRelative(){
		return InputSystem::GetState().mouse.IsRelative();
	}
	
	bool UserInput::GetButtonValue(MouseButton button){
		if(UISystem::IsGettingMouseInputs()){
			return false;
		}
		return InputSystem::GetState().mouse.GetButtonValue(button);
	}
	
	ButtonState UserInput::GetButtonState(MouseButton button){
		if(UISystem::IsGettingMouseInputs()){
			return ButtonState::none;
		}
		return InputSystem::GetState().mouse.GetButtonState(button);
	}
}
