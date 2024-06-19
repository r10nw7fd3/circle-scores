#include "credentials.hpp"
#include <fstream>
#include <sstream>
#include "log.hpp"
#include "json.hpp"

void Credentials::fnotpresent(const std::string& name) {
	std::cout << LOGE"Field " << name << " does not exist" << std::endl;
}

void Credentials::fnotpresentbutnotnes(const std::string& name) {
	std::cout << LOGW"Field " << name << " does not exist, but is not necessary" << std::endl;
}

Credentials::Credentials(const std::string& filename)
	: filename_(filename) {
}

int Credentials::read() {
	std::ifstream ifs;
	ifs.open(filename_);
	if((ifs.rdstate() & std::ifstream::failbit)) {
		std::cout << LOGE"Failed to open " << filename_ << std::endl;
		return 1;
	}
	std::stringstream sstream;
	sstream << ifs.rdbuf();
	std::string json_str = sstream.str();
	ifs.close();

	rapidjson::Document json;
	json.Parse(&json_str[0]);

	JSON_VALIDATE(json, "osu_id", { fnotpresent("osu_id"); return 1; }, Int)
	osu_id_ = std::to_string(json["osu_id"].GetInt());
	JSON_VALIDATE(json, "osu_key", { fnotpresent("osu_key"); return 1; }, String)
	osu_key_ = json["osu_key"].GetString();

	JSON_VALIDATE_SUCCESS(json, "discord_hook_url", {
		fnotpresentbutnotnes("discord_hook_url");
	}, {
		discord_hook_url_ = json["discord_hook_url"].GetString();
	}, String)

	return 0;
}
