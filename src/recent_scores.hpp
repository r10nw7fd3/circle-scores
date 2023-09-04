#pragma once

#include <vector>
#include "get_request.hpp"
#include "score.hpp"

namespace osu::requests {
	class RecentScores: public GetRequest {
	public:
		RecentScores(const std::string&, int);

		long long perform(std::vector<Score>&);
	};
}
