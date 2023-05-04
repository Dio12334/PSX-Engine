#include "components.h"
#include "../renderer/texture.h"

namespace psx {

	void SpriteComponent::SetTexture(class Texture *texture){
		this->texture = texture;
		TexHeight = texture->GetHeight();
		TexWidth = texture->GetWidth();
	}

}
