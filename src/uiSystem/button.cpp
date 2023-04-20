#include "button.h"
#include "../renderer/texture.h"
#include "../math/psxvector.h"
#include "../font/font.h"

namespace psx {
	Button::Button(const std::string& name, Font* font, std::function<void()> onClick, const Vec2f& pos, const Vec2f& dims): 
		m_onClick(onClick),
		m_nameTex(nullptr),
		m_font(font),
		m_position(pos),
		m_dimensions(dims),
		m_highlighted(false){
		
		SetName(name);
		}

	Button::~Button(){
		if(m_nameTex){
			m_nameTex->Unload();
			delete m_nameTex;
		}
	}

	void Button::SetName(const std::string &name){
		m_name = name;
		if(m_nameTex){
			m_nameTex->Unload();
			delete m_nameTex;
			m_nameTex = nullptr;
		}
		m_nameTex = m_font->RenderText(m_name);
	}

	bool Button::CointainsPoint(const Vec2f &pt) const {
		return pt.x >= (m_position.x - m_dimensions.x/2.f) &&
			pt.x <= (m_position.x + m_dimensions.x/2.f) &&
			pt.y >= (m_position.y - m_dimensions.y/2.f) &&
			pt.y <= (m_position.y + m_dimensions.y/2.f);
	}

	void Button::OnClick(){
		if(m_onClick){
			m_onClick();
		}
	}
}
