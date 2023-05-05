#include "components.h"
#include "scriptableEntity.h"
#include "../renderer/texture.h"


namespace psx {

	void SpriteComponent::SetTexture(class Texture *i_texture){
		texture = i_texture;
		TexHeight = texture->GetHeight();
		TexWidth = texture->GetWidth();
	}

	NativeScriptComponent::~NativeScriptComponent(){
		for(auto instance: Instances){
			delete instance;
		}

		Instances.clear();
	}
}
