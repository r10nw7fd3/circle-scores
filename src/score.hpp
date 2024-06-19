#pragma once

#include <string>
#include "json.hpp"
#include "time.hpp"

class Score {
private:
	int encode_mods(int&) const;
	int error_ = 0;
	std::string country_;
	std::string player_;
	std::string artist_; // Musician? Idk!
	std::string song_;
	std::string diff_;
	double acc_;
	std::string mods_;
	int misses_;
	int sb_ = 0; // TODO
	int pp_;
	unsigned long long score_id_;
	unsigned long long mapset_id_;
	std::string map_md5_;
	time_t time_;
public:
	Score(const rapidjson::Value&);
	Score() = default;

	std::string format(bool for_discord = false) const;

	int get_error() const { return error_; }
	const std::string& get_country() const { return country_; }
	const std::string& get_player() const { return player_; }
	const std::string& get_artist() const { return artist_; }
	const std::string& get_song() const { return song_; }
	const std::string& get_diff() const { return diff_; }
	double get_acc() const { return acc_; }
	const std::string& get_mods() const { return mods_; }
	int get_misses() const { return misses_; }
	int get_sb() const { return sb_; }
	int get_pp() const { return pp_; }
	unsigned long long get_score_id() const { return score_id_; }
	unsigned long long get_mapset_id() const { return mapset_id_; }
	const std::string& get_map_md5() const { return map_md5_; }
	time_t get_time() const { return time_; }

	std::string get_score_url() const;
	std::string get_cover_url() const;
};
