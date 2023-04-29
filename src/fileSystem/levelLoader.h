#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <string>
#include <functional>
#include <rapidjson/document.h>
#include <unordered_map>
#include <utility>
#include <iostream>

namespace psx {
	using EntityFunc = std::function<class Entity*(class Engine*, const rapidjson::Value&)>;
	using ComponentFunc = std::function<class Component* (class Entity*, const rapidjson::Value&)>;
	
	class LevelLoader{
		public:
			static bool LoadLevel(class Engine* engine, const std::string& fileName);
			static bool LoadJSON(const std::string& fileName, rapidjson::Document& outDoc);
			static void SaveLevel(class Engine* engine, const std::string& fileName);
			
			static std::string AddEntityFunc(const std::pair<std::string, EntityFunc>& en){
				entityFactory().insert(en);
				return en.first;
			}

			static std::string AddComponentFunc(const std::pair<std::string, ComponentFunc>& co){
				componentFactory().insert(co);
				return co.first;
			}

			static std::unordered_map<std::string, EntityFunc>& entityFactory();

			static std::unordered_map<std::string, ComponentFunc>& componentFactory();

		protected:

			static void LoadGlobalProperties(class Engine* engine, const rapidjson::Value& inObject);
			static void LoadEntities(class Engine* engine, const rapidjson::Value& inArray);
			static void LoadComponents(class Entity* entity, const rapidjson::Value& inArray);


			static void SaveGlobalProperties(rapidjson::Document::AllocatorType& alloc, class Engine* engine, rapidjson::Value& inObject);
			static void SaveEntities(rapidjson::Document::AllocatorType& alloc, class Engine* engine, rapidjson::Value& inArray);
			static void SaveComponents(rapidjson::Document::AllocatorType& alloc, const class Entity* entity, rapidjson::Value& inArray);
	};
}
#endif

