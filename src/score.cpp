#include "score.hpp"
#include "uleb128.hpp"
#include "log.hpp"
#include <fstream>
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

namespace osu {
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

	Score::Score() {}

	int Score::get_error() const {
		return error_;
	}

	const std::string& Score::get_country() const {
		return country_;
	}

	const std::string& Score::get_player() const {
		return player_;
	}

	const std::string& Score::get_artist() const {
		return artist_;
	}

	const std::string& Score::get_song() const {
		return song_;
	}

	const std::string& Score::get_diff() const {
		return diff_;
	}

	double Score::get_acc() const {
		return acc_;
	}

	const std::string& Score::get_mods() const {
		return mods_;
	}

	int Score::get_misses() const {
		return misses_;
	}

	int Score::get_sb() const {
		return sb_;
	}

	int Score::get_pp() const {
		return pp_;
	}

	unsigned long long Score::get_score_id() const {
		return score_id_;
	}

	unsigned long long Score::get_mapset_id() const {
		return mapset_id_;
	}

	const std::string& Score::get_map_md5() const {
		return map_md5_;
	}

	time_t Score::get_time() const {
		return time_;
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

	// TODO: Rewrite this with a custom ofstream class
	// Danser is very loyal to the data it receives, which makes
	// it easy to construct minimal replay file without getting a lot of values from api
	// and computing anything ourselves
	int Score::write_dummy_replay(std::ostream& fs, const std::string& replay_data) const {
#define TMP_BUF_SIZE 80
		char tmp_buf[TMP_BUF_SIZE];
		int8_t tmp_i8 = 0;
		int32_t tmp_i32 = 20230727;
		uint64_t long tmp_i64;
		char md5pref[2] = { 11, 32 };

		// Mode
		fs.write(reinterpret_cast<const char*>(&tmp_i8), 1);
		// Version
		fs.write(reinterpret_cast<const char*>(&tmp_i32), 4);

		// Beatmap md5
		fs.write(md5pref, 2);
		fs.write(&map_md5_[0], md5pref[1]);

		// Player name
		size_t nbytes = uleb128_encode(player_.size(), tmp_buf, TMP_BUF_SIZE);
		if(!nbytes)
			return 1;
		// Player name: identifier
		fs.write(md5pref, 1);
		// Player name: uleb
		fs.write(tmp_buf, nbytes);
		// Player name: name
		fs.write(&player_[0], player_.size());

		// Replay md5 (empty)
		fs.write(reinterpret_cast<const char*>(&tmp_i8), 1);

		// 300, 100, 50, geki, katu, misses, score, max combo, pc
		// It doesn't matter what's going to be in those values, so we won't even 0 init them
		fs.write(tmp_buf, 19);

		// Mods
		if(encode_mods(tmp_i32))
			return 1;
		fs.write(reinterpret_cast<const char*>(&tmp_i32), 4);

		// HP bar (empty)
		fs.write(reinterpret_cast<const char*>(&tmp_i8), 1);

		// Time
		tmp_i64 = unix_to_winticks(time_);
		fs.write(reinterpret_cast<const char*>(&tmp_i64), sizeof(time_t));

		// Replay data size
		tmp_i32 = replay_data.size();
		fs.write(reinterpret_cast<const char*>(&tmp_i32), 4);
		// Replay data
		fs.write(&replay_data[0], replay_data.size());

		// Online score id. Generally not needed, but i suppose		
		// it's going to be used for a leaderboard in danser
		fs.write(reinterpret_cast<const char*>(&score_id_), 8);

		fs.flush();

		return !fs.good();
	}
}
