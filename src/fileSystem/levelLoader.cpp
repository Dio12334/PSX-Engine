#include "levelLoader.h"
#include "jsonHelper.h"
#include "../entity/entity.h"
#include "../engine/engine.h"
#include <cstddef>
#include <fstream>
#include <SDL2/SDL_log.h>
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <iostream>
#include <unordered_map>
namespace psx {
	
	static const int LevelVersion = 1;

	std::unordered_map<std::string, EntityFunc>& LevelLoader::entityFactory() {
			
		static std::unordered_map<std::string, EntityFunc> s_entityFactoryMap {{"Entity", &Entity::Create<Entity>}};
		return s_entityFactoryMap;
			
	};

	std::unordered_map<std::string, ComponentFunc>& LevelLoader::componentFactory(){
		static std::unordered_map<std::string, ComponentFunc> s_componentFactoryMap;
		return s_componentFactoryMap;
	};


	bool LevelLoader::LoadLevel(class Engine* engine, const std::string& fileName){
		rapidjson::Document doc;

		if(!LoadJSON(fileName, doc)){
			SDL_Log("Failed to load level %s", fileName.c_str());
			return false;
		}
		
		int version = 0;
		if(!json::GetInt(doc, "version", version) || version != LevelVersion){
			SDL_Log("Incorrect level file version for %s", fileName.c_str());
			return false;
		}
		
		const rapidjson::Value& globals = doc["globalProperties"];
		if(globals.IsObject()){
			LoadGlobalProperties(engine, globals);
		}
			
		const rapidjson::Value& entities = doc["entities"];
		if(entities.IsArray()){
			LoadEntities(engine, entities);
		}
		return true;
	}

	bool LevelLoader::LoadJSON(const std::string& fileName, rapidjson::Document& outDoc){
		std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ios::ate);
		if(!file.is_open()){
			SDL_Log("File %s not found", fileName.c_str());
			return false;
		}

		std::ifstream::pos_type fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<char> bytes(static_cast<size_t>(fileSize) + 1);
		file.read(bytes.data(), static_cast<size_t>(fileSize));
		outDoc.Parse(bytes.data());

		if(!outDoc.IsObject()){
			SDL_Log("File %s is not valid JSON", fileName.c_str());
			return false;
		}

		return true;
	}

	void LevelLoader::SaveLevel(class Engine* engine, const std::string& fileName){
		rapidjson::Document doc;
		doc.SetObject();

		json::AddInt(doc.GetAllocator(), doc, "version", LevelVersion);

		rapidjson::Value globals(rapidjson::kObjectType);
		SaveGlobalProperties(doc.GetAllocator(), engine, globals);
		doc.AddMember("globalProperties", globals, doc.GetAllocator());

		rapidjson::Value entities(rapidjson::kArrayType);
		SaveEntities(doc.GetAllocator(), engine, entities);
		doc.AddMember("entities", entities, doc.GetAllocator());

		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		doc.Accept(writer);
		const char* output = buffer.GetString();

		std::ofstream outFile(fileName);
		if(outFile.is_open()){
			outFile << output;
		}
	}

	void LevelLoader::LoadGlobalProperties(class Engine* engine, const rapidjson::Value& inObject){

	}

	void LevelLoader::LoadEntities(class Engine* engine, const rapidjson::Value& inArray){
		for(rapidjson::SizeType i = 0; i < inArray.Size(); ++i){
			const auto& entityObj = inArray[i];
			if(!entityObj.IsObject()){
				continue;
			}
			std::string type;
			if(!json::GetString(entityObj, "type", type)){
				continue;
			}
					
			auto iter = entityFactory().find(type);
			if(iter == entityFactory().end()){
				SDL_Log("Unkown entity type %s", type.c_str());
				continue;
			}

			Entity* entity = iter->second(engine, entityObj["properties"]);
			if(!entityObj.HasMember("components")){
				continue;
			}
			const auto& components = entityObj["components"];
			if(components.IsArray()){
				LoadComponents(entity, components);
			}
			
		}
	}

	void LevelLoader::LoadComponents(class Entity* entity, const rapidjson::Value& inArray){
		for(rapidjson::SizeType i = 0; i < inArray.Size(); ++i){
			const auto& compObj = inArray[i];
			if(!compObj.IsObject()){
				continue;
			}

			std::string type;
			if(!json::GetString(compObj, "type", type)){
				continue;
			}

			auto iter = componentFactory().find(type);
			if(iter == componentFactory().end()){
				SDL_Log("Unkown component type %s", type.c_str());
				continue;
			}

			Component::TypeID tid = static_cast<Component::TypeID>(iter->first);
			Component* comp = entity->GetComponentOfType(tid);
			if(!comp){
				comp = iter->second(entity,compObj["properties"]);
			}
			else{
				comp->LoadProperties(compObj["properties"]);
			}
		}
	}


	void LevelLoader::SaveGlobalProperties(rapidjson::Document::AllocatorType& alloc, class Engine* engine, rapidjson::Value& inObject){

	}

	void LevelLoader::SaveEntities(rapidjson::Document::AllocatorType& alloc, class Engine* engine, rapidjson::Value& inArray){
		const auto& entities = engine->GetEntities();
		for(const Entity* entity: entities){
			rapidjson::Value obj(rapidjson::kObjectType);

			json::AddString(alloc, obj, "type", Entity::typeNames[entity->GetType()]);
			rapidjson::Value props(rapidjson::kObjectType);
			entity->SaveProperties(alloc, props);
			obj.AddMember("properties", props, alloc);

			rapidjson::Value components(rapidjson::kArrayType);
			SaveComponents(alloc, entity, components);
			obj.AddMember("components", components, alloc);

			inArray.PushBack(obj, alloc);
		}
	}

	void LevelLoader::SaveComponents(rapidjson::Document::AllocatorType& alloc, const class Entity* entity, rapidjson::Value& inArray){
		const auto& components = entity->GetComponents();
		for(const Component* comp: components){
			rapidjson::Value obj(rapidjson::kObjectType);
			json::AddString(alloc, obj, "type", comp->GetType().c_str());
			rapidjson::Value props(rapidjson::kObjectType);
			comp->SaveProperties(alloc, props);
			obj.AddMember("properties", props, alloc);
			inArray.PushBack(obj, alloc);
		}
	}
}
