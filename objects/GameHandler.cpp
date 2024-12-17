#include "GameHandler.h"
#include <iostream>

GameHandler::GameHandler(std::string chart_path, std::string song_config_path, std::string game_type_,
                         std::function<void(const std::string&)> scene_changer_, ALLEGRO_SAMPLE_INSTANCE* music_)
    : game_type(game_type_), scene_changer(scene_changer_), music(music_) {
    CL = ChartLoader::get_instance();
    IM = InputManager::get_instance();
    RM = ResourceManager::get_instance();

    perfect_count = 0;
    good_count = 0;
    miss_count = 0;
    start_time = al_current_time();
    recover_time = al_current_time();
    pause_time = 0;
    is_init = true;
    is_paused = false;

    health = HEALTH_MAX;

    notes = CL->load_chart(chart_path, game_type);
    song_duration = 20000; // or load from config

    std::string text_key = "caviar_dreams";
    font = RM->get_font(text_key, 45);

    std::cout << "Successfully loaded game" << std::endl;
}

void GameHandler::draw() {
    if (!is_init || is_paused) return;

    for (auto &note : current_notes) {
        note.draw();
    }

    for (auto &note : current_hold_notes) {
        note.draw();
    }

    for (auto &[display_time, score_data] : current_scores) {
        SCORE_TYPE score_type = score_data.first;
        ALLEGRO_COLOR color;
        int score_value;

        switch (score_type) {
            case SCORE_TYPE::PERFECT:
                color = al_map_rgb(49, 62, 79);
                score_value = PERFECT_SCORE;
                break;
            case SCORE_TYPE::GOOD:
                color = al_map_rgb(84, 130, 50);
                score_value = GOOD_SCORE;
                break;
            case SCORE_TYPE::MISS:
                color = al_map_rgb(193, 0, 0);
                score_value = MISS_SCORE;
                break;
            default:
                continue;
        }

        al_draw_text(font, color, 900, 800, ALLEGRO_ALIGN_RIGHT, std::to_string(score_value).c_str());
    }

    // Draw health bar
    int screen_width = al_get_display_width(al_get_current_display());
    int screen_height = al_get_display_height(al_get_current_display());

    int bar_width = 300;
    int bar_height = 80;
    int bar_x = screen_width - bar_width - 50; 
    int bar_y = screen_height - bar_height - 30;

    float health_ratio = (float)health / (float)HEALTH_MAX;
    int fill_width = (int)(bar_width * health_ratio);

    al_draw_filled_rectangle(bar_x, bar_y, bar_x + bar_width, bar_y + bar_height, al_map_rgb(100,100,100));
    al_draw_filled_rectangle(bar_x, bar_y, bar_x + fill_width, bar_y + bar_height, al_map_rgb(255,100,100));

    std::string health_text = std::to_string(health) + "%";
    al_draw_text(font, al_map_rgb(255,255,255), bar_x + bar_width/2, bar_y + bar_height/2 - 30, ALLEGRO_ALIGN_CENTRE, health_text.c_str());
}

void GameHandler::toggle_pause() {
    if (!is_paused) {
        pause_time = al_current_time();
    } else {
        start_time += al_current_time() - pause_time;
        recover_time += al_current_time() - pause_time;
    }
    is_paused = !is_paused;
}

void GameHandler::update() {
    if (!is_init || is_paused) return;

    double current_time = (al_current_time() - start_time) * 1000;
    double travel_time_ms = (FALL_LENGTH / FALL_SPEED) * (1000.0 / FPS);

    if(current_time - recover_time >= 5000){
        adjust_health(HEALTH_LOSS_MISS);
        recover_time = current_time;
    }

    while (!notes.empty()) {
        const Note &front_note = notes.front();
        double spawn_time = front_note.get_time() - travel_time_ms;

        if (spawn_time <= current_time) {
            if(front_note.is_hold_note())
                current_hold_notes.push_back(front_note);
            else
                current_notes.push_back(front_note);
            notes.pop();
        } else {
            break;
        }
    }

    // Check misses
    while (!current_notes.empty() && current_notes.front().get_y() > 1300) {
        miss_count++;
        current_scores.push_back({current_time + SCORE_TIME, {SCORE_TYPE::MISS, current_notes.front().get_col()}});
        adjust_health(-HEALTH_LOSS_MISS);
        current_notes.pop_front();
    }

    // Remove expired scores
    while (!current_scores.empty() && current_scores.front().first <= current_time) {
        current_scores.pop_front();
    }

    for (auto &note : current_notes) {
        note.update();
    }

    for (auto &note : current_hold_notes) {
        note.update();
    }
}


bool GameHandler::game_ended() {
    double current_time = (al_current_time() - start_time) * 1000;
    // Game ends if song duration done or health <= 0
    return (current_time >= song_duration);
}

void GameHandler::handle_input() {
    if (is_paused) return;

    double current_time = (al_current_time() - start_time) * 1000;

    auto hit_note = [&](int col_index) {
        if (!current_notes.empty()) {
            SCORE_TYPE current_note_score = current_notes.front().check_score(col_index);
            if (current_note_score != SCORE_TYPE::NONE) {
                if (current_note_score == SCORE_TYPE::MISS) {
                    miss_count++;
                    current_scores.push_back({current_time + SCORE_TIME, {SCORE_TYPE::MISS, col_index}});
                    adjust_health(-HEALTH_LOSS_MISS);
                } else if (current_note_score == SCORE_TYPE::GOOD) {
                    good_count++;
                    current_scores.push_back({current_time + SCORE_TIME, {SCORE_TYPE::GOOD, col_index}});
                    adjust_health(HEALTH_GAIN_GOOD);
                } else if (current_note_score == SCORE_TYPE::PERFECT) {
                    perfect_count++;
                    current_scores.push_back({current_time + SCORE_TIME, {SCORE_TYPE::PERFECT, col_index}});
                    adjust_health(HEALTH_GAIN_PERFECT);
                }
                current_notes.pop_front();
            }
        }

        for(Note &note : current_hold_notes){
            SCORE_TYPE current_note_score = note.check_score(col_index);
            if (current_note_score != SCORE_TYPE::NONE) {
                if (current_note_score == SCORE_TYPE::MISS) {
                    miss_count++;
                    current_scores.push_back({current_time + SCORE_TIME, {SCORE_TYPE::MISS, col_index}});
                    adjust_health(-HEALTH_LOSS_MISS);
                } else if (current_note_score == SCORE_TYPE::GOOD) {
                    good_count++;
                    current_scores.push_back({current_time + SCORE_TIME, {SCORE_TYPE::GOOD, col_index}});
                    adjust_health(HEALTH_GAIN_GOOD);
                } else if (current_note_score == SCORE_TYPE::PERFECT) {
                    perfect_count++;
                    current_scores.push_back({current_time + SCORE_TIME, {SCORE_TYPE::PERFECT, col_index}});
                    adjust_health(HEALTH_GAIN_PERFECT);
                }
            }
        }
    };

    if (game_type == "4k") {
        if (IM->was_key_pressed(ALLEGRO_KEY_D)) hit_note(0);
        if (IM->was_key_pressed(ALLEGRO_KEY_F)) hit_note(1);
        if (IM->was_key_pressed(ALLEGRO_KEY_J)) hit_note(2);
        if (IM->was_key_pressed(ALLEGRO_KEY_K)) hit_note(3);
    } else if (game_type == "6k") {
        if (IM->was_key_pressed(ALLEGRO_KEY_D)) hit_note(0);
        if (IM->was_key_pressed(ALLEGRO_KEY_F)) hit_note(1);
        if (IM->was_key_pressed(ALLEGRO_KEY_G)) hit_note(2);
        if (IM->was_key_pressed(ALLEGRO_KEY_H)) hit_note(3);
        if (IM->was_key_pressed(ALLEGRO_KEY_J)) hit_note(4);
        if (IM->was_key_pressed(ALLEGRO_KEY_K)) hit_note(5);
    }
}
