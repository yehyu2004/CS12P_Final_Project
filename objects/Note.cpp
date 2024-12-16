#include "Note.h"
#include <iostream>

Note::Note(double time_, int col_, std::string type_, std::string game_type_, double duration_)
    : time(time_), col(col_), type(type_), game_type(game_type_), duration(duration_) {
    // Set initial coordinates based on game type
    if (game_type == "4k") {
        x = init_coords_4k[col].first;
        y = init_coords_4k[col].second;
    } else if (game_type == "6k") {
        x = init_coords_6k[col].first;
        y = init_coords_6k[col].second;
    }

    // If this is a hold note, shift it upward so the entire length falls into view
    if (is_hold_note()) {
        double hold_length_pixels = (FALL_SPEED * FPS) * (duration / 1000.0);
        // Move the note up by the hold length so that the entire hold is visible as it comes down
        y -= hold_length_pixels;
    }
}

void Note::draw() {
    int R, G, B;
    if (game_type == "4k") {
        auto color = key_note_color_4k[col];
        R = color[0]; G = color[1]; B = color[2];
    } else { // "6k"
        auto color = key_note_color_6k[col];
        R = color[0]; G = color[1]; B = color[2];
    }

    double note_visual_height = NOTE_HEIGHT;
    if (is_hold_note()) {
        double hold_length_pixels = (FALL_SPEED * FPS) * (duration / 1000.0);
        note_visual_height += hold_length_pixels;
    }

    if (game_type == "4k") {
        al_draw_filled_rectangle(x, y, x + NOTE_WIDTH_4k, y + note_visual_height, al_map_rgb(R, G, B));
    } else { // "6k"
        al_draw_filled_rectangle(x, y, x + NOTE_WIDTH_6k, y + note_visual_height, al_map_rgb(R, G, B));
    }
    
}

void Note::update() {
    y += FALL_SPEED;
}

bool Note::check_in_range(double left, double right) {
    return (y >= left && y <= right);
}

SCORE_TYPE Note::check_score(int key) {
    if (key != col) return SCORE_TYPE::NONE;

    double lower_perf = FALL_LENGTH - PERFECT_FRAME * (1.0 / FPS) * FALL_SPEED;
    double upper_perf = FALL_LENGTH + PERFECT_FRAME * (1.0 / FPS) * FALL_SPEED;
    double lower_good = FALL_LENGTH - GOOD_FRAME * (1.0 / FPS) * FALL_SPEED;
    double upper_good = FALL_LENGTH + GOOD_FRAME * (1.0 / FPS) * FALL_SPEED;
    double lower_miss = FALL_LENGTH - MISS_FRAME * (1.0 / FPS) * FALL_SPEED;
    double upper_miss = FALL_LENGTH + MISS_FRAME * (1.0 / FPS) * FALL_SPEED;

    if (check_in_range(lower_perf, upper_perf)) {
        return SCORE_TYPE::PERFECT;
    } else if (check_in_range(lower_good, upper_good)) {
        return SCORE_TYPE::GOOD;
    } else if (check_in_range(lower_miss, upper_miss)) {
        return SCORE_TYPE::MISS;
    }

    return SCORE_TYPE::NONE;
}
