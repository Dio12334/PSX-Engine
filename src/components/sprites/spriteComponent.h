#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "../component.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace psx {
	class SpriteComponent: public Component{
		public:
			DEFINE_COMPONENT_TYPE(SpriteComponent);


			SpriteComponent(class Entity* owner, i32 drawOrder = 100);
			~SpriteComponent();
			virtual void Draw(class Shader* shader);
			virtual void SetTexture(class Texture* texture);

			void LoadProperties(const rapidjson::Value& inObj) override;
			void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const override;

			i32 GetDrawOrder() const { return m_drawOrder; }
			i32 GetTexHeight() const { return m_texHeight; }
			i32 GetTexWidth() const { return m_texWidth; }
		protected:
			class Texture* m_texture;
			i32 m_drawOrder;
			i32 m_texHeight;
			i32 m_texWidth;
	};
}

#endif

