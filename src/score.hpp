#pragma once

#include <string>
#include <iostream>
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
	Score();

	int get_error() const;
	const std::string& get_country() const;
	const std::string& get_player() const;
	const std::string& get_artist() const;
	const std::string& get_song() const;
	const std::string& get_diff() const;
	double get_acc() const;
	const std::string& get_mods() const;
	int get_misses() const;
	int get_sb() const;
	int get_pp() const;
	unsigned long long get_score_id() const;
	unsigned long long get_mapset_id() const;
	const std::string& get_map_md5() const;
	time_t get_time() const;

	std::string get_score_url() const;
	std::string get_cover_url() const;

	int write_dummy_replay(std::ostream&, const std::string&) const;
};
