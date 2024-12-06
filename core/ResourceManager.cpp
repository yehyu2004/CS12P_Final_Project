#include "ResourceManager.h"
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <fstream>
#include "../library/nlohmann/json.hpp" // Include your chosen JSON library

using json = nlohmann::json;

bool ResourceManager::load_config(const std::string &filename) {
    std::ifstream file(filename);
    GAME_ASSERT(file.is_open(), "Failed to open resource config file: %s", filename.c_str());

    json j;
    file >> j;

    if (j.contains("fonts")) {
        for (auto &el : j["fonts"].items()) {
            fontPaths[el.key()] = el.value().get<std::string>();
        }
    }

    if (j.contains("images")) {
        for (auto &el : j["images"].items()) {
            imagePaths[el.key()] = el.value().get<std::string>();
        }
    }

    if (j.contains("gifs")) {
        for (auto &el : j["gifs"].items()) {
            gifPaths[el.key()] = el.value().get<std::string>();
        }
    }

    if (j.contains("sounds")) {
        for (auto &el : j["sounds"].items()) {
            soundPaths[el.key()] = el.value().get<std::string>();
        }
    }

    return true;
}

ALLEGRO_FONT* ResourceManager::get_font(const std::string &font_key, int size) {
    auto &font_map = loadedFonts[font_key];
    auto it = font_map.find(size);
    if (it != font_map.end()) {
        return it->second;
    }

    auto path_it = fontPaths.find(font_key);
    GAME_ASSERT(path_it != fontPaths.end(), "Font key '%s' not found in config.", font_key.c_str());

    ALLEGRO_FONT *font = al_load_ttf_font(path_it->second.c_str(), size, 0);
    GAME_ASSERT(font, "Failed to load font: %s", path_it->second.c_str());
    font_map[size] = font;
    return font;
}

ALLEGRO_BITMAP* ResourceManager::get_image(const std::string &image_key) {
    auto it = loadedImages.find(image_key);
    if (it != loadedImages.end()) {
        return it->second;
    }

    auto path_it = imagePaths.find(image_key);
    GAME_ASSERT(path_it != imagePaths.end(), "Image key '%s' not found in config.", image_key.c_str());

    ALLEGRO_BITMAP *bitmap = al_load_bitmap(path_it->second.c_str());
    GAME_ASSERT(bitmap, "Failed to load image: %s", path_it->second.c_str());
    loadedImages[image_key] = bitmap;
    return bitmap;
}

ALGIF_ANIMATION* ResourceManager::get_gif(const std::string &gif_key) {
    auto it = loadedGifs.find(gif_key);
    if (it != loadedGifs.end()) {
        return it->second;
    }

    auto path_it = gifPaths.find(gif_key);
    GAME_ASSERT(path_it != gifPaths.end(), "GIF key '%s' not found in config.", gif_key.c_str());

    ALGIF_ANIMATION *gif = algif_load_animation(path_it->second.c_str());
    GAME_ASSERT(gif, "Failed to load GIF: %s", path_it->second.c_str());
    loadedGifs[gif_key] = gif;
    return gif;
}

bool ResourceManager::init_audio() {
    bool res = true;
    res &= al_install_audio();
    res &= al_init_acodec_addon();
    res &= al_reserve_samples(16); // you can adjust the number of samples reserved
    GAME_ASSERT(res, "Failed to initialize audio components.");
    return res;
}

ALLEGRO_SAMPLE_INSTANCE* ResourceManager::play_sound(const std::string &sound_key, ALLEGRO_PLAYMODE mode) {
    auto it = loadedSounds.find(sound_key);
    if (it == loadedSounds.end()) {
        auto path_it = soundPaths.find(sound_key);
        GAME_ASSERT(path_it != soundPaths.end(), "Sound key '%s' not found in config.", sound_key.c_str());

        ALLEGRO_SAMPLE *sample = al_load_sample(path_it->second.c_str());
        GAME_ASSERT(sample, "Failed to load sound: %s", path_it->second.c_str());

        it = loadedSounds.insert({sound_key, {sample, {}}}).first;
    }

    auto &[sample, insts] = it->second;
    ALLEGRO_SAMPLE_INSTANCE *instance = al_create_sample_instance(sample);
    insts.push_back(instance);

    al_set_sample_instance_playmode(instance, mode);
    al_attach_sample_instance_to_mixer(instance, al_get_default_mixer());
    al_play_sample_instance(instance);

    return instance;
}

bool ResourceManager::is_sound_playing(ALLEGRO_SAMPLE_INSTANCE *inst) {
    return al_get_sample_instance_playing(inst);
}

void ResourceManager::toggle_sound(ALLEGRO_SAMPLE_INSTANCE *inst) {
    bool is_playing_now = al_get_sample_instance_playing(inst);
    if (is_playing_now) {
        unsigned int pos = al_get_sample_instance_position(inst);
        al_stop_sample_instance(inst);
        // restore position if you want to resume
        al_set_sample_instance_position(inst, pos);
    } else {
        al_play_sample_instance(inst);
    }
}

void ResourceManager::update_sounds() {
    for (auto &[path, audio_pair] : loadedSounds) {
        auto &[sample, insts] = audio_pair;
        for (auto it = insts.begin(); it != insts.end();) {
            ALLEGRO_SAMPLE_INSTANCE *inst = *it;
            // Check if instance finished
            if (!al_get_sample_instance_playing(inst) &&
                al_get_sample_instance_position(inst) == 0 &&
                al_get_sample_instance_playmode(inst) != ALLEGRO_PLAYMODE_LOOP) {
                al_destroy_sample_instance(inst);
                it = insts.erase(it);
            } else {
                ++it;
            }
        }
    }
}

bool ResourceManager::erase_sound(const std::string &sound_key) {
    auto it = loadedSounds.find(sound_key);
    if (it == loadedSounds.end()) return false;

    auto &[sample, insts] = it->second;
    for (auto inst : insts) {
        al_destroy_sample_instance(inst);
    }
    insts.clear();
    al_destroy_sample(sample);
    loadedSounds.erase(it);
    return true;
}
