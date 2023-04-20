#ifndef JSONHELPER_H
#define JSONHELPER_H

#include "../math/psxvector.h"
#include <rapidjson/document.h>

namespace psx::json {

	bool GetInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt);
	bool GetFloat(const rapidjson::Value& inObject, const char* inProperty, float& outFloat);
	bool GetString(const rapidjson::Value& inObject, const char* inProperty, std::string& outString);
	bool GetBool(const rapidjson::Value& inObject, const char* inProperty, bool& outBool);
	bool GetVector2D(const rapidjson::Value& inObject, const char* inProperty, Vec2f& outVector2D);

	void AddInt(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, int value);	
	void AddFloat(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, float value);
	void AddString(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, const std::string& value);
	void AddBool(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, bool value);
	void AddVector2D(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, const Vec2f& value);
}

#endif

