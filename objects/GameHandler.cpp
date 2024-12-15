#include "GameHandler.h"
#include <iostream>

GameHandler::GameHandler(std::string chart_path, std::string song_config_path, std::string game_type) {
	CL = ChartLoader::get_instance();
	IM = InputManager::get_instance();
	RM = ResourceManager::get_instance();
	perfect_count = good_count = miss_count = 0;
	start_time = al_current_time();
	notes = CL->load_chart(chart_path, game_type);


	std::string text_key = "caviar_dreams"; // change this later
	font = RM->get_font(text_key, 45);


	is_init = true;

	std::cout << "Successfully loaded game" << std::endl;
}

void GameHandler::draw() {
	if (!is_init) return;
	for (auto &note : current_notes) {
		std::cout << "note: " << note.get_y() << "\n";
		note.draw();
	}

	for (auto [time, p] : current_scores) { // change later
		SCORE_TYPE score_type = p.first;
		int col = p.second;
		if (score_type == SCORE_TYPE::PERFECT) {
			al_draw_text(font, al_map_rgb(49, 62, 79), 900, 800, ALLEGRO_ALIGN_RIGHT, std::to_string(PERFECT_SCORE).c_str()); //perfect
		} else if (score_type == SCORE_TYPE::GOOD) {
			al_draw_text(font, al_map_rgb(84, 130, 50), 900, 800, ALLEGRO_ALIGN_RIGHT, std::to_string(GOOD_SCORE).c_str()); //good
		} else if (score_type == SCORE_TYPE::MISS) {
			al_draw_text(font, al_map_rgb(193, 0, 0), 900, 800, ALLEGRO_ALIGN_RIGHT, std::to_string(MISS_SCORE).c_str()); //miss
		}
	}
}

void GameHandler::update() {
	if (!is_init) return;
	double current_time = (al_current_time() - start_time) * 1000;

	while (!notes.empty() && notes.front().get_time() - (FALL_LENGTH / FALL_SPEED) * (1.0 / FPS) <= current_time) {
		// std::cout << "NOTES ADDED TO CURRENT\n";
		current_notes.push_back(notes.front());
		notes.pop();
	}

	while (!current_notes.empty() && current_notes.front().get_y() > 1300) {
		miss_count++;
		current_scores.push_back({current_time + SCORE_TIME, {SCORE_TYPE::MISS, current_notes.front().get_col()}});
		current_notes.pop_front();
	}

	while (!current_scores.empty() && current_scores.front().first <= current_time) {
		current_scores.pop_front();
	}

	for (auto &note : current_notes) {
		note.update();
	}

}


void GameHandler::handle_input() {
	double current_time = (al_current_time() - start_time) * 1000;
	if (IM->was_key_pressed(ALLEGRO_KEY_D)) {
		if (!current_notes.empty()) {
			SCORE_TYPE current_note_score = current_notes.front().check_score(0);
			if (current_note_score != SCORE_TYPE::NONE) {
				if (current_note_score == SCORE_TYPE::MISS) {
					miss_count++;
					current_scores.push_back({current_time + SCORE_TIME , {SCORE_TYPE::MISS, 0}});
				} else if (current_note_score == SCORE_TYPE::GOOD) {
					good_count++;
					current_scores.push_back({current_time + SCORE_TIME , {SCORE_TYPE::GOOD, 0}});
				} else if (current_note_score == SCORE_TYPE::PERFECT) {
					perfect_count++;
					current_scores.push_back({current_time + SCORE_TIME , {SCORE_TYPE::PERFECT, 0}});
				}
				current_notes.pop_front();
			}
		}
	}

	if (IM->was_key_pressed(ALLEGRO_KEY_F)) {
		if (!current_notes.empty()) {
			SCORE_TYPE current_note_score = current_notes.front().check_score(1);
			if (current_note_score != SCORE_TYPE::NONE) {
				if (current_note_score == SCORE_TYPE::MISS) {
					miss_count++;
					current_scores.push_back({current_time + SCORE_TIME , {SCORE_TYPE::MISS, 1}});
				} else if (current_note_score == SCORE_TYPE::GOOD) {
					good_count++;
					current_scores.push_back({current_time + SCORE_TIME , {SCORE_TYPE::GOOD, 1}});
				} else if (current_note_score == SCORE_TYPE::PERFECT) {
					perfect_count++;
					current_scores.push_back({current_time + SCORE_TIME , {SCORE_TYPE::PERFECT, 1}});
				}
				current_notes.pop_front();
			}
		}

	}

	if (IM->was_key_pressed(ALLEGRO_KEY_J)) {
		if (!current_notes.empty()) {
			SCORE_TYPE current_note_score = current_notes.front().check_score(2);
			if (current_note_score != SCORE_TYPE::NONE) {
				if (current_note_score == SCORE_TYPE::MISS) {
					miss_count++;
					current_scores.push_back({current_time + SCORE_TIME , {SCORE_TYPE::MISS, 2}});
				} else if (current_note_score == SCORE_TYPE::GOOD) {
					good_count++;
					current_scores.push_back({current_time + SCORE_TIME , {SCORE_TYPE::GOOD, 2}});
				} else if (current_note_score == SCORE_TYPE::PERFECT) {
					perfect_count++;
					current_scores.push_back({current_time + SCORE_TIME , {SCORE_TYPE::PERFECT, 2}});
				}
				current_notes.pop_front();
			}
		}

	}

	if (IM->was_key_pressed(ALLEGRO_KEY_K)) {
		if (!current_notes.empty()) {
			SCORE_TYPE current_note_score = current_notes.front().check_score(3);
			if (current_note_score != SCORE_TYPE::NONE) {
				if (current_note_score == SCORE_TYPE::MISS) {
					miss_count++;
					current_scores.push_back({current_time + SCORE_TIME , {SCORE_TYPE::MISS, 3}});
				} else if (current_note_score == SCORE_TYPE::GOOD) {
					good_count++;
					current_scores.push_back({current_time + SCORE_TIME , {SCORE_TYPE::GOOD, 3}});
				} else if (current_note_score == SCORE_TYPE::PERFECT) {
					perfect_count++;
					current_scores.push_back({current_time + SCORE_TIME , {SCORE_TYPE::PERFECT, 3}});
				}
				current_notes.pop_front();
			}
		}

	}
}