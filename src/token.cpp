#include "token.hpp"
#include <iostream>
#include "json.hpp"
#include "log.hpp"

#define TOKENGENFAILED "Failed to generate osu! token: "

namespace osu::requests {
	Token::Token(const std::string& id, const std::string& key)
		: postfields_("client_id=" + id + "&client_secret=" + key + "&grant_type=client_credentials&scope=public"), req_("https://osu.ppy.sh/oauth/token") {
		req_.set_postdata(postfields_);
	}
 
	const std::string& Token::get_token() {
		if(time(nullptr) >= expire_ - 60) {
			if(generate())
					token_ = "";
		}
		return token_;
	}

	int Token::generate() {
		std::cout << LOGI"Generating new osu! token" << std::endl;
		std::string data;
		if(req_.perform(data) == -1) {
			std::cout << LOGE TOKENGENFAILED << req_.last_err() << std::endl;
			return 1;
		}

		rapidjson::Document doc;
		doc.Parse(&data[0]);
		if(!doc.HasMember("access_token")) {
			std::cout << LOGE TOKENGENFAILED"Malformed JSON" << std::endl;
			std::cout << LOGI"Response: " << data << std::endl;
			return 1;
		}
		token_ = doc["access_token"].GetString();
		expire_ = time(nullptr) + doc["expires_in"].GetInt();
		std::cout << LOGI"New token: " << token_ << std::endl;
		return 0;
	}
}
