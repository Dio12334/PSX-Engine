#ifndef USERINPUT_H
#define USERINPUT_H

#include "inputSystem.h"

namespace psx {

	class UserInput{
		
		public:

			static bool GetKeyValue(KeyCode keycode);
			static ButtonState GetKeyState(KeyCode keycode);
			
			
			static const Vec2f& GetPosition();
			static const Vec2f& GetScrollWheel();
			static bool IsRelative();
			static bool GetButtonValue(MouseButton button);
			static ButtonState GetButtonState(MouseButton button);
	};
}
#endif

