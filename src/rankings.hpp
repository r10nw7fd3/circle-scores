#pragma once

#include "get_request.hpp"
#include <vector>
#include <tuple>

class Rankings: public GetRequest {
public:
	Rankings(const std::string&, int);

	long long perform(std::vector<std::tuple<int, std::string>>&);
};
