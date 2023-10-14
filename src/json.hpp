#pragma once

#define RAPIDJSON_ASSERT(expr)
#include <rapidjson/document.h>

#define JSON_VALIDATE(parent, member, onerr, type) \
	if(!parent.HasMember(member) || !parent[member].Is##type()) { \
		onerr \
	}

#define JSON_VALIDATE_SUCCESS(parent, member, onerr, onsuccess, type) \
	JSON_VALIDATE(parent, member, onerr, type) \
	else { \
		onsuccess \
	}
