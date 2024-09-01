#pragma once

#include <stdexcept>

class JsonException: public std::runtime_error {
public:
	using std::runtime_error::runtime_error;
};

#define RAPIDJSON_ASSERT(expr) if(!(expr)) throw JsonException(#expr);
#include <rapidjson/document.h>
