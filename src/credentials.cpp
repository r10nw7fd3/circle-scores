#include "credentials.hpp"
#include <fstream>
#include <sstream>
#include "log.hpp"
#include "json.hpp"

void Credentials::fnotpresent(const std::string& name) {
	LOGE << "Field " << name << " does not exist" << std::endl;
}

void Credentials::fnotpresentbutnotnes(const std::string& name) {
	LOGW << "Field " << name << " does not exist, but is not necessary" << std::endl;
}

Credentials::Credentials(const std::string& filename)
	: filename_(filename) {
}

int Credentials::read() {
	std::ifstream ifs;
	ifs.open(filename_);
	if((ifs.rdstate() & std::ifstream::failbit)) {
		LOGE << "Failed to open " << filename_ << std::endl;
		return 1;
	}
	std::stringstream sstream;
	sstream << ifs.rdbuf();
	std::string json_str = sstream.str();
	ifs.close();

	rapidjson::Document json;
	json.Parse(&json_str[0]);

	try { osu_id_ = std::to_string(json["osu_id"].GetInt()); } catch(std::exception& e) {
		fnotpresent("osu_id");
		return 1;
	}
	try { osu_key_ = json["osu_key"].GetString(); } catch(std::exception& e) {
		fnotpresent("osu_key");
		return 1;
	}

	try { discord_hook_url_ = json["discord_hook_url"].GetString(); } catch(std::exception& e) {
		fnotpresentbutnotnes("discord_hook_url");
	}

	try { telegram_bot_token_ = json["telegram_bot_token"].GetString(); } catch(std::exception& e) {
		fnotpresentbutnotnes("telegram_bot_token");
	}

	return 0;
}
