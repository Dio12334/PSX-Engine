#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <glm/ext/matrix_transform.hpp>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../utils/psxint.h"

namespace psx {

	struct IDComponent{

	};

	struct TransformComponent{
		glm::vec3 Translation {0.f, 0.f, 0.f};
		glm::vec3 Scale {1.f, 1.f, 1.f};
		float Rotation {0.f};

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation):
			Translation(translation) {}

		glm::mat4 GetTransform() const {
			return glm::translate(glm::mat4(1.f), Translation)
				* glm::rotate(glm::mat4(1.f), Rotation, glm::vec3(0.f, 0.f, 1.f))
				* glm::scale(glm::mat4(1.f), Scale);
		}
	};

	struct TagComponent{

		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag): Tag(tag){}
	};

	class Texture;
	struct SpriteComponent{
		class Texture* texture = nullptr;
		i32 DrawOrder = 100;
		i32 TexHeight = 0;
		i32 TexWidth = 0;
		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(class Texture* texture, i32 drawOrder = 100):
			DrawOrder(drawOrder){
				SetTexture(texture);
			}
		void SetTexture(class Texture* texture);
	};

	struct CameraComponent{
		class Renderer* renderer;

		CameraComponent(class Renderer* renderer): renderer(renderer){

		} 
	};

	class ScriptableEntity;
	struct NativeScriptComponent{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InstantiateFunc)();
		void(*DestroyInstanceFunc)(NativeScriptComponent*);

		template <typename T>
		void Bind(){
			InstantiateFunc = []() {return static_cast<ScriptableEntity*>(new T()); };
			DestroyInstanceFunc = [](NativeScriptComponent* nsc) { delete (T*)nsc->Instance; nsc->Instance = nullptr;};
			
		}
	};
}
#endif

