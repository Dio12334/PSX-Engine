#ifndef BUTTON_H
#define BUTTON_H

#include "../math/psxvector.h"
#include <string>
#include <functional>

namespace psx {

	class Button{
		public:
			Button(const std::string& name, 
					class Font* font, std::function<void()> onClick,
					const Vec2f& pos, const Vec2f& dim);

			~Button();
			void SetName(const std::string& name);
			class Texture* GetNameTex() const { return m_nameTex; }
			const Vec2f& GetPosition() const { return m_position; }
			void SetPosition(const Vec2f& position) { m_position = position; }
			void SetHighlighted(bool sel) { m_highlighted = sel; }
			bool GetHighlighted() const { return m_highlighted; }

			bool CointainsPoint(const Vec2f& pt) const;
			void OnClick();

		private:
			std::function<void()> m_onClick;
			std::string m_name;
			class Texture* m_nameTex;
			class Font* m_font;
			Vec2f m_position;
			Vec2f m_dimensions;
			bool m_highlighted;
	};
}

#endif

