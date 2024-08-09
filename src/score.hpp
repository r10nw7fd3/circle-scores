#pragma once

#include <string>
#include "json.hpp"
#include "time.hpp"

class Score {
private:
	int error_ = 0;
	std::string country_;
	std::string player_;
	std::string artist_; // Musician? Idk!
	std::string song_;
	std::string diff_;
	std::string grade_;
	double acc_;
	std::string mods_;
	// osu api does not return gekis an katus
	int count_300_;
	int count_100_;
	int count_50_;
	int misses_;
	int max_combo_;
	int sb_ = 0; // TODO
	int pp_;
	unsigned long long score_;
	unsigned long long score_id_;
	unsigned long long mapset_id_;
	unsigned long long map_id_;
	std::string map_md5_;
	time_t time_;

public:
	Score(const rapidjson::Value&);
	Score() = default;

	std::string format(bool for_discord = false) const;
	int encode_mods(int& dest) const;

	int get_error() const { return error_; }
	const std::string& get_country() const { return country_; }
	const std::string& get_player() const { return player_; }
	const std::string& get_artist() const { return artist_; }
	const std::string& get_song() const { return song_; }
	const std::string& get_diff() const { return diff_; }
	const std::string& get_grade() const { return grade_; }
	double get_acc() const { return acc_; }
	const std::string& get_mods() const { return mods_; }
	int get_count_300() const { return count_300_; }
	int get_count_100() const { return count_100_; }
	int get_count_50() const { return count_50_; }
	int get_misses() const { return misses_; }
	int get_max_combo() const { return max_combo_; }
	int get_sb() const { return sb_; }
	int get_pp() const { return pp_; }
	unsigned long long get_score() const { return score_; }
	unsigned long long get_score_id() const { return score_id_; }
	unsigned long long get_mapset_id() const { return mapset_id_; }
	unsigned long long get_map_id() const { return map_id_; }
	const std::string& get_map_md5() const { return map_md5_; }
	time_t get_time() const { return time_; }

	std::string get_score_url() const;
	std::string get_cover_url() const;
};
