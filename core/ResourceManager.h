#ifndef RESOURCEMANAGER_H_INCLUDED
#define RESOURCEMANAGER_H_INCLUDED

#include <string>
#include <map>
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include "../algif5/algif.h" // For GIF loading with algif
#include "../Utils.h"        // For GAME_ASSERT or logging

class ResourceManager {
public:
    static ResourceManager *get_instance() {
        static ResourceManager instance;
        return &instance;
    }

    bool load_config(const std::string &filename);

    // Fonts
    ALLEGRO_FONT* get_font(const std::string &font_key, int size);

    // Images
    ALLEGRO_BITMAP* get_image(const std::string &image_key);

    // GIFs
    ALGIF_ANIMATION* get_gif(const std::string &gif_key);

    // Sounds
    bool init_audio();
    void update_sounds();
    ALLEGRO_SAMPLE_INSTANCE* play_sound(const std::string &sound_key, ALLEGRO_PLAYMODE mode);
    bool is_sound_playing(ALLEGRO_SAMPLE_INSTANCE *inst);
    void toggle_sound(ALLEGRO_SAMPLE_INSTANCE *inst);
    bool erase_sound(const std::string &sound_key);

private:
    ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // Config maps
    std::map<std::string, std::string> fontPaths;    // Key = font name, value = font file path
    std::map<std::string, std::string> imagePaths;   // Key = image name, value = image file path
    std::map<std::string, std::string> gifPaths;     // Key = gif name, value = gif file path
    std::map<std::string, std::string> soundPaths;   // Key = sound name, value = sound file path

    // Loaded resources
    // Fonts: A map of font-key to another map of {size: ALLEGRO_FONT*}
    std::map<std::string, std::map<int, ALLEGRO_FONT*>> loadedFonts;
    std::map<std::string, ALLEGRO_BITMAP*> loadedImages;
    std::map<std::string, ALGIF_ANIMATION*> loadedGifs;
    std::map<std::string, std::pair<ALLEGRO_SAMPLE*, std::vector<ALLEGRO_SAMPLE_INSTANCE*>>> loadedSounds;
};

#endif
