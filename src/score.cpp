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
	std::string iso8601;
	try {
		const auto& user = json["user"];
		country_ = user["country_code"].GetString();
		player_ = user["username"].GetString();

		const auto& mapset = json["beatmapset"];
		artist_ = mapset["artist"].GetString();
		song_ = mapset["title"].GetString();
		mapset_id_ = mapset["id"].GetUint64();

		const auto& map = json["beatmap"];
		diff_ = map["version"].GetString();
		map_id_ = map["id"].GetUint64();
		map_md5_ = map["checksum"].GetString();

		acc_ = json["accuracy"].GetDouble() * 100.0f;
		for(const auto& e : json["mods"].GetArray()) {
			mods_ += e.GetString();
		}

		const auto& statistics = json["statistics"];
		count_300_ = statistics["count_300"].GetInt();
		count_100_ = statistics["count_100"].GetInt();
		count_50_ = statistics["count_50"].GetInt();
		misses_ = statistics["count_miss"].GetInt();

		max_combo_ = json["max_combo"].GetInt();

		grade_ = json["rank"].GetString();

		pp_ = std::round(json["pp"].GetDouble());

		score_ = json["score"].GetUint64();

		score_id_ = json["best_id"].GetUint64();

		iso8601 = json["created_at"].GetString();
	}
	catch(std::exception& e) {
		error_ = 1;
		return;
	}
	time_t time = parse_iso8601(&iso8601[0]);
	if(!time) {
		error_ = 1;
		return;
	}
	time_ = time;

}

static inline void escape(std::string& str) {
	for(size_t start = 0; (start = str.find("_", start))
			!= std::string::npos; start += 2)
		str.replace(start, 1, "\\_");
	for(size_t start = 0; (start = str.find("*", start))
			!= std::string::npos; start += 2)
		str.replace(start, 1, "\\*");
}

// TODO: Discord-specific stuff should be handled somewhere else
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

	if(for_discord)
		escape(ret);

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
			LOGE << e.what();
			return 1;
		}
		if(!sub.compare("NC"))
			dest |= modmap.at("DT");
		else if(!sub.compare("PF"))
			dest |= modmap.at("SD");
	}
	return 0;
}
