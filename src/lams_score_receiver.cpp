#include <sstream>
#include <fstream>
#include <iomanip>
#include "lams_score_receiver.hpp"
#include "json.hpp"
#include "log.hpp"
#if defined(__unix__) || defined(__unix)
#include <sys/stat.h>
#endif

LamsScoreReceiver::LamsScoreReceiver(const std::string& address, const std::string& dir)
	: post_(address + "/api/submit"), dir_(dir) {
	try_create_dir();
}

void LamsScoreReceiver::on_score(const Score& score) {
	std::string postdata = make_postdata(score);
	post_.set_postdata(postdata);

	std::string data;
	long ret = post_.perform(data);
	if(ret < 0 || ret / 100 != 2) {
		std::cout << LOGE << "Request failed, ret = " << ret << ", postdata = " << std::endl;
		std::cout << postdata << std::endl;
		std::cout << LOGE << "Response: " << data << std::endl;
		return;
	}

	rapidjson::Document json;
	json.Parse(data.c_str());

#define ONERR \
	std::cout << LOGE << "Invalid json:" << std::endl; \
	std::cout << data << std::endl;

	JSON_VALIDATE(json, "image", ONERR, Object)
	const auto& image_object = json["image"].GetObject();
	JSON_VALIDATE(image_object, "url", ONERR, String)
	std::string image_url = image_object["url"].GetString();

	std::cout << LOGI << "Downloading image from " << image_url << std::endl;

	get_.set_endpoint(image_url);
	std::string image_data;
	ret = get_.perform(image_data);
	if(ret < 0 || ret / 100 != 2) {
		std::cout << LOGE << "Failed to download image, ret = " << ret << std::endl;
		return;
	}

	std::ofstream ofs(dir_ + "/" + std::to_string(score.get_score_id()) + ".png");
	ofs.write(image_data.c_str(), image_data.size());
	if(!ofs)
		std::cout << LOGE << "Open/Write failed" << std::endl;
}

std::string LamsScoreReceiver::make_postdata(const Score& score) {
	std::string ret;

	time_t t = score.get_time() + timezone;
	struct tm tm = *localtime(&t);
	std::ostringstream time_stream;
	time_stream << std::put_time(&tm, "%Y-%m-%dT%H:%M:%SZ");

	std::string username = score.get_player();
	for(size_t start_pos = 0; (start_pos = username.find(' ', start_pos)) != std::string::npos;
		start_pos += 3) {
		username.replace(start_pos, 1, "%20");
	}
	for(size_t start_pos = 0; (start_pos = username.find('[', start_pos)) != std::string::npos;
		start_pos += 3) {
		username.replace(start_pos, 1, "%5B");
	}
	for(size_t start_pos = 0; (start_pos = username.find(']', start_pos)) != std::string::npos;
		start_pos += 3) {
		username.replace(start_pos, 1, "%5D");
	}

	ret += "score%5Busername%5D=" + username;
	ret += "&score%5Bdate%5D=" + time_stream.str();
	int mods;
	score.encode_mods(mods);
	ret += "&score%5Benabled_mods%5D=" + std::to_string(mods);
	ret += "&score%5Bscore%5D=" + std::to_string(score.get_score());
	ret += "&score%5Bmaxcombo%5D=" + std::to_string(score.get_max_combo());
	ret += "&score%5Brank%5D=" + score.get_grade();
	ret += "&score%5Bcount50%5D=" + std::to_string(score.get_count_50());
	ret += "&score%5Bcount100%5D=" + std::to_string(score.get_count_100());
	ret += "&score%5Bcount300%5D=" + std::to_string(score.get_count_300());
	ret += "&score%5Bcountmiss%5D=" + std::to_string(score.get_misses());
	ret += "&score%5Bcountkatu%5D=0"; 
	ret += "&score%5Bcountgeki%5D=0"; 
	ret += "&score%5Bpp%5D=" + std::to_string(score.get_pp());
	ret += "&beatmap_id=" + std::to_string(score.get_map_id());

	return ret;
}

void LamsScoreReceiver::try_create_dir() {
#if defined(__unix__) || defined(__unix)
	if(mkdir(dir_.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) < 0 && errno != EEXIST)
		std::cout << LOGE << "Failed to create dir " << dir_ << std::endl;
#endif
}
