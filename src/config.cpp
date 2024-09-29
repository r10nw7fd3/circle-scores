#include <fstream>
#include "json.hpp"
#include "config.hpp"
#include "log.hpp"

Config::Config(const std::string& filename) {
	std::ifstream ifs(filename);
	if(!ifs) {
		LOGE << "Failed to open " << filename << std::endl;
		return;
	}
	rapidjson::IStreamWrapper isw(ifs);

	rapidjson::Document doc;
	doc.ParseStream(isw);

	try {
		interval_ = doc["interval"].GetInt();
		page_ = doc["page"].GetInt();
		lower_pp_bound_ = doc["lower_pp_bound"].GetInt();
		catch_sigint_ = doc["catch_sigint"].GetBool();
		save_token_to_file_ = doc["save_token_to_file"].GetBool();
		token_filename_ = doc["token_filename"].GetString();

		const auto& ids_array = doc["excluded_ids"].GetArray();

		for(const auto& id : ids_array) {
			excluded_ids_.push_back(id.GetInt());
		}

#ifdef ENABLE_LAMS
		lams_enabled_ = doc["lams_enabled"].GetBool();
		lams_dir_ = doc["lams_address"].GetString();
		lams_dir_ = doc["lams_dir"].GetString();
#endif

	}
	catch(std::exception& e) {
		LOGE << "Failed to parse config" << std::endl;
		return;
	}

	valid_ = true;
}
