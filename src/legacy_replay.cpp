#include "legacy_replay.hpp"
#include "json.hpp"
#include <stdexcept>
#include <iostream>
#include "log.hpp"
#define BASE64_IMPL
#include "../3rdparty/base64.h"

namespace osu::requests {
	LegacyReplay::LegacyReplay(std::string key, unsigned long long score_id)
		: GetRequest(std::string("https://osu.ppy.sh/api/get_replay?k=").append(key).append("&s=").append(std::to_string(score_id))) {
	}

	long long LegacyReplay::perform(std::string& data) {
		std::string json;
		long long ret = GetRequest::perform(json);
		if(ret < 0)
			return ret;

		rapidjson::Document doc;
		doc.Parse(&json[0]);
		if(doc.HasParseError())
			return -2;

		if(doc.HasMember("error") && doc["error"].IsString()) {
			std::cout << LOGE << doc["error"].GetString() << std::endl;
		}

		JSON_VALIDATE(doc, "content", return -2;, String)
		std::string base64ed = doc["content"].GetString();
		try {
			data = base64_decode(base64ed);
		}
		catch(const std::runtime_error& e) {
			std::cout << LOGE << e.what();
			std::cout << "Full response:" << std::endl << json << std::endl; 
			return -3;
		}

		if(!data.compare("")) {
			std::cout << LOGE"Empty base64 result, full response:" << std::endl;
			std::cout << json << std::endl;
		}

		return ret;
	}
}
