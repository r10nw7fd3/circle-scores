#pragma once

#include "get_request.hpp"

// There is actually an undocumented route in apiv2 which probably does the same,
// but for now I think it's easier to do things this way
class LegacyReplay: public GetRequest {
public:
	LegacyReplay(std::string, unsigned long long);

	long long perform(std::string&);
};
