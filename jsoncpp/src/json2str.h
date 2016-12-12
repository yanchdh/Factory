#pragma once
#include "json/json.h"
#include <string>

class json2str
{
public:
	json2str(const json2str&) = delete;
	json2str& operator = (const json2str&) = delete;
	explicit json2str(Json::ValueType value_type) : 
		m_value(value_type)
	{
	}
	inline std::string to_string()
	{
		Json::FastWriter write;
		return std::move(write.write(m_value));
	}
	inline const Json::Value& get_value()
	{
		return m_value;
	}
	template<typename ValType>
	inline void add(const std::string& key, const ValType& val)
	{
		m_value[key] = val;
	}
	template<typename ValType>
	inline void append(const ValType& val)
	{
		m_value.append(val);
	}
private:
	Json::Value			m_value;
};