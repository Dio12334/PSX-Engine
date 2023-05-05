#include "userInput.h"

namespace psx {

	bool UserInput::GetKeyValue(KeyCode keycode){
		return InputSystem::GetState().keyboard.GetKeyValue(keycode);
	}
	
	ButtonState UserInput::GetKeyState(KeyCode keycode){
		return InputSystem::GetState().keyboard.GetKeyState(keycode);
	}

	const Vec2f& UserInput::GetPosition(){
		return InputSystem::GetState().mouse.GetPosition();
	}

	const Vec2f& UserInput::GetScrollWheel(){
		return InputSystem::GetState().mouse.GetScrollWheel();
	}
			
	bool UserInput::IsRelative(){
		return InputSystem::GetState().mouse.IsRelative();
	}
	
	bool UserInput::GetButtonValue(MouseButton button){
		return InputSystem::GetState().mouse.GetButtonValue(button);
	}
	
	ButtonState UserInput::GetButtonState(MouseButton button){
		return InputSystem::GetState().mouse.GetButtonState(button);
	}
}
