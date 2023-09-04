#pragma once

#define RAPIDJSON_ASSERT(expr)
#include <rapidjson/document.h>

#define JSON_VALIDATE(parent, member, onerr, type) \
	if(!parent.HasMember(member) || !parent[member].Is##type()) { \
		onerr \
	}
