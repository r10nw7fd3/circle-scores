#include "recent_scores.hpp"

#include "prefix.hpp"
#include "json.hpp"
#include "log.hpp"

namespace osu::requests {
	RecentScores::RecentScores(const std::string& token, int user_id)
		: GetRequest(std::string(OSUAPI_PREFIX"users/").append(std::to_string(user_id)).append("/scores/recent?mode=osu")) {
		GetRequest::add_header(OSUAUTH_PREFIX + token);
	}

	long long RecentScores::perform(std::vector<Score>& dest) {
		std::string json;
		long long ret = GetRequest::perform(json);
		if(ret < 0)
			return ret;

		rapidjson::Document doc;
		doc.Parse(&json[0]);
		if(doc.HasParseError() || !doc.IsArray())
			return -2;

		for(auto& e : doc.GetArray()) {
			if(!e.IsObject())
				return -2;

			// Pre-filter score.
			if(e["pp"].IsNull() || e["best_id"].IsNull())
				continue;

			dest.emplace_back(e);
			if(dest.back().get_error()) {
				std::cout << LOGE"Failed to construct score. Full response dump: " << std::endl;
				std::cout << json << std::endl;
				std::cout << LOGE"Score error code: " << dest.back().get_error() << std::endl;
			}
		}

		return ret;
	}
}
