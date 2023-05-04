#ifndef USERINPUT_H
#define USERINPUT_H

#include "inputSystem.h"


namespace psx {

	class UserInput{
		
		public:
			static bool Init(class InputSystem* system);
			static void Shutdown();

			static bool GetKeyValue(KeyCode keycode);
			static ButtonState GetKeyState(KeyCode keycode);
			
			
			static const Vec2f& GetPosition();
			static const Vec2f& GetScrollWheel();
			static bool IsRelative();
			static bool GetButtonValue(MouseButton button);
			static ButtonState GetButtonState(MouseButton button);


		private:
			static class InputSystem* s_inputSystem;
	};
}
#endif

