#include "jsonHelper.h"
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>

namespace psx::json {
	
	bool GetInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt){
		auto itr = inObject.FindMember(inProperty);
		if(itr == inObject.MemberEnd()){
			return false;
		}

		auto& property = itr->value;
		if(!property.IsInt()){
			return false;
		}

		outInt = property.GetInt();
		return true;
	}

	bool GetFloat(const rapidjson::Value& inObject, const char* inProperty, float& outFloat){

		auto itr = inObject.FindMember(inProperty);
		if(itr == inObject.MemberEnd()){
			return false;
		}

		auto& property = itr->value;
		if(!property.IsFloat()){
			return false;
		}

		outFloat = property.GetFloat();
		return true;
	}

	bool GetString(const rapidjson::Value& inObject, const char* inProperty, std::string& outString){
		auto itr = inObject.FindMember(inProperty);
		if(itr == inObject.MemberEnd()){
			return false;
		}

		auto& property = itr->value;
		if(!property.IsString()){
			return false;
		}

		outString = property.GetString();
		return true;
	}

	bool GetBool(const rapidjson::Value& inObject, const char* inProperty, bool& outBool){
		auto itr = inObject.FindMember(inProperty);
		if(itr == inObject.MemberEnd()){
			return false;
		}

		auto& property = itr->value;
		if(!property.IsBool()){
			return false;
		}

		outBool = property.GetBool();
		return true;
	}

	bool GetVector2D(const rapidjson::Value& inObject, const char* inProperty, Vec2f& outVector2D){
		auto itr = inObject.FindMember(inProperty);
		if(itr == inObject.MemberEnd()){
			return false;
		}

		auto& property = itr->value;
		if(!property.IsArray() || property.Size() != 2){
			return false;
		}

		for(rapidjson::SizeType i = 0; i < 2; i++){
			if(!property[i].IsFloat()){
				return false;
			}
		}

		outVector2D.x = property[0].GetFloat();
		outVector2D.y = property[1].GetFloat();
		return true;
	}

	void AddInt(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, int value){
		rapidjson::Value v(value);
		inObject.AddMember(rapidjson::StringRef(name), v, alloc);
	}	

	void AddFloat(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, float value){
		rapidjson::Value v(value);
		inObject.AddMember(rapidjson::StringRef(name), v, alloc);
	}

	void AddString(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, const std::string& value){
		rapidjson::Value v;
		v.SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.length()), alloc);
		inObject.AddMember(rapidjson::StringRef(name), v, alloc);
	}

	void AddBool(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, bool value){
		rapidjson::Value v(value);
		inObject.AddMember(rapidjson::StringRef(name), v, alloc);
	}

	void AddVector2D(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, const Vec2f& value){
		rapidjson::Value v(rapidjson::kArrayType);
		v.PushBack(rapidjson::Value(value.x).Move(), alloc);
		v.PushBack(rapidjson::Value(value.y).Move(), alloc);

		inObject.AddMember(rapidjson::StringRef(name), v, alloc);
	}
}
