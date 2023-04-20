#ifndef UISCREEN_H
#define UISCREEN_H

#include "../math/psxvector.h"
#include "../utils/psxcolor.h"
#include "../utils/psxint.h"
#include <string>
#include <functional>
#include <vector>

namespace psx {
	class UIScreen{
		public:
			UIScreen(class Engine* engine);
			virtual ~UIScreen();

			virtual void Update(float deltaTime);
			virtual void Draw(class Shader* shader);
			virtual void ProcessInput(const struct InputState& state);
			virtual void HandleKeyPress(i32 key);

			enum UIState{
				sActive,
				sClosing
			};

			void Close();

			UIState GetState() const { return m_state; }

			void SetTitle(const std::string& text, const Color& color = Color::White, i32 pointSize = 40);
			void AddButton(const std::string& name, std::function<void()> onClick);
		protected:
			void DrawTexture(class Shader* shader, class Texture* texture, const Vec2f& offset = Vec2f(), float scale = 1.f, float zoom = 1.f);
			void SetRelativeMouseMode(bool relative);
			class Engine* m_engine;
			class Font* m_font;
			class Texture* m_title;
			class Texture* m_background;
			class Texture* m_buttonOn;
			class Texture* m_buttonOff;

			Vec2f m_titlePos;
			Vec2f m_nextButtonPos;
			Vec2f m_BGPos;

			UIState m_state;
			std::vector<class Button*> m_buttons;
	};
}
#endif

