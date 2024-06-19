#include "score.hpp"
#include "uleb128.hpp"
#include "log.hpp"
#include <cstdint>
#include <unordered_map>
#include <cmath>

// https://github.com/ppy/osu-web/blob/master/app/Libraries/Mods.php#L21C28
static const std::unordered_map<std::string, uint32_t> modmap = {
	{ "NF", 1 },
	{ "EZ", 2 },
	{ "TD", 4 },
	{ "HD", 8 },
	{ "HR", 16 },
	{ "SD", 32 },
	{ "DT", 64 },
	{ "HT", 256 },
	{ "NC", 512 },
	{ "FL", 1024 },
	{ "SO", 4096 },
	{ "PF", 16384 },
	{ "V2", 536870912 }
};

Score::Score(const rapidjson::Value& json) {
#define ONERR(code) \
	error_ = code; \
	return;

	JSON_VALIDATE(json, "user", ONERR(1), Object)
	const auto& user = json["user"];
	JSON_VALIDATE(user, "country_code", ONERR(2), String)
	country_ = user["country_code"].GetString();
	JSON_VALIDATE(user, "username", ONERR(3), String)
	player_ = user["username"].GetString();

	JSON_VALIDATE(json, "beatmapset", ONERR(4), Object)
	const auto& mapset = json["beatmapset"];
	JSON_VALIDATE(mapset, "artist", ONERR(5), String)
	artist_ = mapset["artist"].GetString();
	JSON_VALIDATE(mapset, "title", ONERR(6), String)
	song_ = mapset["title"].GetString();
	JSON_VALIDATE(mapset, "id", ONERR(7), Uint64)
	mapset_id_ = mapset["id"].GetUint64();

	JSON_VALIDATE(json, "beatmap", ONERR(8), Object)
	const auto& map = json["beatmap"];
	JSON_VALIDATE(map, "version", ONERR(9), String)
	diff_ = map["version"].GetString();
	JSON_VALIDATE(map, "checksum", ONERR(10), String)
	map_md5_ = map["checksum"].GetString();

	JSON_VALIDATE(json, "accuracy", ONERR(11), Number)
	acc_ = json["accuracy"].GetDouble() * 100.0f;
	JSON_VALIDATE(json, "mods", ONERR(12), Array)
	for(const auto& e : json["mods"].GetArray()) {
		if(!e.IsString()) {
			ONERR(13)
		}
		mods_ += e.GetString();
	}

	JSON_VALIDATE(json, "statistics", ONERR(14), Object)
	JSON_VALIDATE(json["statistics"], "count_miss", ONERR(15), Int)
	misses_ = json["statistics"]["count_miss"].GetInt();

	JSON_VALIDATE(json, "pp", ONERR(16), Number)
	pp_ = std::round(json["pp"].GetDouble());

	JSON_VALIDATE(json, "best_id", ONERR(17), Uint64)
	score_id_ = json["best_id"].GetUint64();

	JSON_VALIDATE(json, "created_at", ONERR(18), String)
	std::string iso8601 = json["created_at"].GetString();
	time_t time = parse_iso8601(&iso8601[0]);
	if(!time) {
		ONERR(19)
	}
	time_ = time;

}

std::string Score::format(bool for_discord) const {
	std::string country;
	if(for_discord) {
		country = std::string(":flag_").append(country_).append(":");
		for(char& c : country)
			c = std::tolower(c);
	}
	else
		country = country_;

	std::string acc = std::to_string(std::round(acc_ / 0.01) * 0.01);
	for(int i = 0; i < 4 && !acc.empty(); ++i)
			acc.pop_back();

	std::string ret = country.append(" ").append(player_);
	ret += " | ";
	ret.append(artist_).append(" - ").append(song_).append(" [").append(diff_).append("] ");
	ret += acc.append("% +").append(mods_.empty() ? "NM" : mods_);
	ret += " ";
	ret += (misses_ ? std::to_string(misses_).append("miss ") : acc_ == 100.0 ? "FC " : "FC? ");
	ret += std::to_string(pp_);
	ret += "pp ";
	ret += get_score_url();
	return ret;
}

std::string Score::get_score_url() const {
	return "https://osu.ppy.sh/scores/osu/" + std::to_string(score_id_);
}

std::string Score::get_cover_url() const {
	std::string url = "https://assets.ppy.sh/beatmaps/" + std::to_string(mapset_id_);
	url += "/covers/cover.jpg";
	return url;
}

int Score::encode_mods(int& dest) const {
	dest = 0;
	for(size_t i = 0; i < mods_.size(); i += 2) {
		std::string sub = mods_.substr(i, 2);	
		try {
			dest |= modmap.at(sub);
		}
		catch(std::exception& e) {
			std::cout << LOGE << e.what();
			return 1;
		}
		if(!sub.compare("NC"))
			dest |= modmap.at("DT");
		else if(!sub.compare("PF"))
			dest |= modmap.at("SD");
	}
	return 0;
}
