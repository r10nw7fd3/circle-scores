#include "rankings.hpp"
#include "prefix.hpp"
#include "json.hpp"

namespace osu::requests {
	Rankings::Rankings(const std::string& token, int page)
		: GetRequest(OSUAPI_PREFIX"rankings/osu/performance?cursor[page]=" + page) {
		GetRequest::add_header(OSUAUTH_PREFIX + token);
	}

	long long Rankings::perform(std::vector<std::tuple<int, std::string>>& dest) {
		std::string json;
		long long ret = GetRequest::perform(json);
		if(ret < 0)
			return ret;

		rapidjson::Document doc;
		doc.Parse(&json[0]);

		if(doc.HasParseError())
			return -2;
		JSON_VALIDATE(doc, "ranking", return -2;, Array)

		const auto& ranking = doc["ranking"];
		for(const auto& e : ranking.GetArray()) {
			const auto& id = e["user"]["id"];
			const auto& name = e["user"]["username"];

			if(!id.IsInt() || !name.IsString())
				return -2;

			dest.push_back(std::make_tuple(id.GetInt(), name.GetString()));
		}

		return ret;
	}
}
