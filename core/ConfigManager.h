#ifndef CONFIGMANAGER_H_INCLUDED
#define CONFIGMANAGER_H_INCLUDED

#include <string>

class ConfigManager {
public:
    static ConfigManager *get_instance() {
        static ConfigManager instance;
        return &instance;
    }

    // Load configuration from the given filename
    bool load_from_file(const std::string &filename);

    // Getters for your configuration values
    double get_fps() const { return fps; }
    int get_window_width() const { return window_width; }
    int get_window_height() const { return window_height; }
    int get_game_field_length() const { return game_field_length; }

private:
    ConfigManager() = default;
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    // Configuration variables (from your original code)
    double fps;
    int window_width;
    int window_height;
    int game_field_length;
};

#endif
