#include "token.hpp"
#include "json.hpp"
#include "log.hpp"
#include "prefix.hpp"
#include <fstream>

#define TOKENGENFAILED "Failed to generate osu! token: "

Token::Token(const std::string& id, const std::string& key, const std::string& token_filename)
	: postfields_("client_id=" + id + "&client_secret=" + key + "&grant_type=client_credentials&scope=public"),
	req_("https://osu.ppy.sh/oauth/token"),
	token_filename_(token_filename) {
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
	LOGI << "Generating new osu! token" << std::endl;
	std::string data;
	if(req_.perform(data) == -1) {
		LOGE << TOKENGENFAILED << req_.last_err() << std::endl;
		return 1;
	}

	rapidjson::Document doc;
	doc.Parse(&data[0]);
	try {
		token_ = doc["access_token"].GetString();
		expire_ = time(nullptr) + doc["expires_in"].GetInt();
	}
	catch(std::exception& e) {
		LOGE << TOKENGENFAILED "Invalid json" << std::endl;
		return 1;
	}
	LOGI << "New token: " << token_ << std::endl;

	if(!token_filename_.empty()) {
		std::ofstream ofs(token_filename_);
		ofs << token_;
	}

	return 0;
}

int Token::revoke() const {
	if(token_.empty())
		return 0;

	LOGI << "Revoking token, ret = ";
	std::flush(std::cerr);
	BaseRequest req(OSUAPI_PREFIX"oauth/tokens/current");
	curl_easy_setopt(req.get_curl(), CURLOPT_CUSTOMREQUEST, "DELETE");
	req.add_header("Authorization: Bearer " + token_);
	std::string _;
	long long ret = req.perform(_);
	std::cerr << ret << std::endl;
	return (ret < 0 || ret / 100 != 2);
}
