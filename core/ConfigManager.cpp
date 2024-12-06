#include "ConfigManager.h"
#include <fstream>
#include "../library/nlohmann/json.hpp"
#include "../Utils.h" // For GAME_ASSERT or debug_log if needed

using json = nlohmann::json;

bool ConfigManager::load_from_file(const std::string &filename) {
    std::ifstream file(filename);
    GAME_ASSERT(file.is_open(), "Failed to open config file: %s", filename.c_str());

    json j;
    file >> j;

    // Load values with defaults as fallbacks if keys are missing
    fps = j.value("fps", 60.0);
    window_width = j.value("window_width", 1920);
    window_height = j.value("window_height", 1080);
    game_field_length = j.value("game_field_length", 1080);

    return true;
}
