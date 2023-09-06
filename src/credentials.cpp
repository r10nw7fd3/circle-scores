#include "credentials.hpp"
#include <iostream>
#include <fstream>
#include "log.hpp"

// Our read() method kind of depends on these, would it be a good idea to make them static member functions? I don't know!
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

	if(!getline(ifs, osu_id_)) {
		fnotpresent("osu_id");
		return 1;
	}
	if(!getline(ifs, osu_key_)) {
		fnotpresent("osu_key");
		return 1;
	}
	if(!getline(ifs, osu_legacy_key_))
		fnotpresentbutnotnes("osu_legacy_key");
	if(!getline(ifs, discord_hook_url_))
		fnotpresentbutnotnes("discord_hook_url");

	ifs.close();
	return 0;
}

const std::string& Credentials::get_filename() const {
	return filename_;
}

const std::string& Credentials::get_osu_id() const {
	return osu_id_;
}

const std::string& Credentials::get_osu_key() const {
	return osu_key_;
}

const std::string& Credentials::get_osu_legacy_key() const {
	return osu_legacy_key_;
}

const std::string& Credentials::get_discord_hook_url() const {
	return discord_hook_url_;
}
