#include "GameHandler.h"
#include <iostream>

GameHandler::GameHandler(std::string chart_path, std::string song_config_path, std::string game_type){
	CL = ChartLoader::get_instance();
	IM = InputManager::get_instance(); 
	perfect_count = good_count = miss_count = 0;
	start_time = al_current_time();
	notes = CL->load_chart(chart_path, game_type);
	is_init = true;

	std::cout << "Successfully loaded game" << std::endl;
}

void GameHandler::draw(){
	if(!is_init) return;
	for(auto &note : current_notes){
		std::cout << "note: " << note.get_y() << "\n";
		note.draw();
	}
}

void GameHandler::update(){
	if(!is_init) return;
	double current_time = (al_current_time() - start_time) * 1000;

	while(!notes.empty() && notes.front().get_time() - (FALL_LENGTH / FALL_SPEED) * (1.0 / FPS) <= current_time){
		// std::cout << "NOTES ADDED TO CURRENT\n";
		current_notes.push_back(notes.front());
		notes.pop();
	}

	while(!current_notes.empty() && current_notes.front().get_y() > 1080){
		miss_count++;
		current_notes.pop_front();
	}

	for(auto &note : current_notes){
		note.update();
	}
}

void GameHandler::handle_input(){
	if(IM->was_key_pressed(ALLEGRO_KEY_D)){
		if(!current_notes.empty()){
			SCORE_TYPE current_note_score = current_notes.front().check_score(0);
			if (current_note_score != SCORE_TYPE::NONE) {
				if (current_note_score == SCORE_TYPE::MISS) {
					miss_count++;
				} else if (current_note_score == SCORE_TYPE::GOOD) {
					good_count++;
				} else if (current_note_score == SCORE_TYPE::PERFECT) {
					perfect_count++;
				}
				current_notes.pop_front();
			}
		} 
	}

	if(IM->was_key_pressed(ALLEGRO_KEY_F)){
		if(!current_notes.empty()){
			SCORE_TYPE current_note_score = current_notes.front().check_score(1);
			if (current_note_score != SCORE_TYPE::NONE) {
				if (current_note_score == SCORE_TYPE::MISS) {
					miss_count++;
				} else if (current_note_score == SCORE_TYPE::GOOD) {
					good_count++;
				} else if (current_note_score == SCORE_TYPE::PERFECT) {
					perfect_count++;
				}
				current_notes.pop_front();
			}
		} 
		
	}

	if(IM->was_key_pressed(ALLEGRO_KEY_J)){
		if(!current_notes.empty()){
			SCORE_TYPE current_note_score = current_notes.front().check_score(2);
			if (current_note_score != SCORE_TYPE::NONE) {
				if (current_note_score == SCORE_TYPE::MISS) {
					miss_count++;
				} else if (current_note_score == SCORE_TYPE::GOOD) {
					good_count++;
				} else if (current_note_score == SCORE_TYPE::PERFECT) {
					perfect_count++;
				}
				current_notes.pop_front();
			}
		} 
		
	}

	if(IM->was_key_pressed(ALLEGRO_KEY_K)){
		if(!current_notes.empty()){
			SCORE_TYPE current_note_score = current_notes.front().check_score(3);
			if (current_note_score != SCORE_TYPE::NONE) {
				if (current_note_score == SCORE_TYPE::MISS) {
					miss_count++;
				} else if (current_note_score == SCORE_TYPE::GOOD) {
					good_count++;
				} else if (current_note_score == SCORE_TYPE::PERFECT) {
					perfect_count++;
				}
				current_notes.pop_front();
			}
		} 

	}
}