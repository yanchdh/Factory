#pragma once
#include "json/json.h"
#include <string>
#include <iostream>

class jsonparse
{
public:
	jsonparse(const jsonparse&) = delete;
	jsonparse& operator = (const jsonparse&) = delete;
	explicit jsonparse(const std::string& strValue)
	{
		if (!m_reader.parse(strValue, m_value))
		{
			fprintf(stderr, "parse json str fialed [%s]\n", strValue);
			m_success = false;
		}
		else
		{
			m_success = true;
		}
	}

	inline bool success()
	{
		return m_success;
	}

	template<typename RetType, typename... string>
	inline bool get_value(RetType* ret, const std::string& key1, const string&... keyN)
	{
		return _get_value(ret, m_value, key1, keyN...);
	}

private:
	template<typename RetType, typename... string>
	inline bool _get_value(RetType* ret, const Json::Value& value, const std::string& key1, const string&... keyN)
	{
		if (!success() || value.isNull())
			return false;
		if (value.isArray()) 
			return _get_value(ret, value[atoi(key1.c_str())], keyN...);
		else if (value.isObject())
			return _get_value(ret, value[key1], keyN...);
		else
			return false;
		return true;
	}

	inline bool _get_value(bool* ret, const Json::Value& value)
	{
		if (!value.isBool())
			return false;
		*ret = value.asBool();
		return true;
	}

	inline bool _get_value(int* ret, const Json::Value& value)
	{
		if (value.isNull() || !value.isInt())
			return false;
		*ret = value.asInt();
		return true;
	}

	inline bool _get_value(int64_t* ret, const Json::Value& value)
	{
		if (value.isNull() || !value.isInt64())
			return false;
		*ret = value.asInt64();
		return true;
	}

	inline bool _get_value(std::string* ret, const Json::Value& value)
	{
		if (value.isNull() || !value.isString())
			return false;
		*ret = value.asString();
		return true;
	}

	inline bool _get_value(Json::Value* ret, const Json::Value& value)
	{
		if (value.isNull())
			return false;
		*ret = value;
		ret = (Json::Value*)&value;
		return true;
	}

private:
	Json::Reader	m_reader;
	Json::Value		m_value;
	bool			m_success;
};