#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "inputMapping.h"
#include "../math/psxvector.h"
#include "../utils/psxint.h"
#include <SDL2/SDL_events.h>


namespace psx {
	
	enum ButtonState{
		none,
		pressed,
		released,
		held
	};

	class KeyboardState{
		public:
			friend class InputSystem;
			bool GetKeyValue(KeyCode keyCode) const;
			ButtonState GetKeyState(KeyCode keyCode) const;
		private:
			const u8* m_currState;
			u8 m_prevState[SDL_NUM_SCANCODES];
	};

	class MouseState{
		public:
			friend class InputSystem;
			const Vec2f& GetPosition() const { return m_mousePos; }
			const Vec2f& GetScrollWheel() const { return m_scrollWheel; }
			bool IsRelative() const { return m_isRelative; }
			bool GetButtonValue(MouseButton button) const;
			ButtonState GetButtonState(MouseButton button) const;
		private:
			Vec2f m_mousePos;
			Vec2f m_scrollWheel;
			u32 m_currButtons;
			u32 m_prevButtons;
			bool m_isRelative;
	};

	struct InputState{
		KeyboardState keyboard;
		MouseState mouse;
	};

	class InputSystem{
		public:
			bool Initialize();
			void Shutdown();
			void PrepareForUpdate();
			void Update();
			const InputState& GetState() const { return m_state; }
			void SetRelativeMouseMode(bool value);
			void ProcessEvent(union ::SDL_Event& event);
		private:
			InputState m_state;

	};

}

#endif

