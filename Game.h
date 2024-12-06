#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED


#include "data/DataCenter.h"
#include "data/OperationCenter.h"
#include "data/SoundCenter.h"
#include "data/ImageCenter.h"
#include "data/FontCenter.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include "Utils.h"
#include "Character.h"
#include <vector>
#include <cstring>
#include <iostream>

// fixed settings image
constexpr char game_icon_img_path[] = "./assets/image/game_icon.png";
constexpr char background_img_path[] = "./assets/image/StartBackground.jpg";
constexpr char character_img_path[] = "./assets/image/doki_character.jpg";
constexpr char song_img_path[] = "./assets/image/song.png";
constexpr char gallery_img_path[] = "./assets/image/gallery.png";
constexpr char get_img_path[] = "./assets/image/get.png";

//sound
constexpr char game_start_sound_path[] = "./assets/sound/growl.wav";
constexpr char background_sound_path[] = "./assets/sound/menu.mp3";
constexpr char character_sound_path[] = "./assets/sound/characterbgm.mp3";


/**
 * @brief Main class that runs the whole game.
 * @details All game procedures must be processed through this class.
 */
class Game
{
private:
	/**
	 * @brief States of the game process in game_update.
	 * @see Game::game_update()
	 */
	enum class STATE {
		START, // -> LEVEL
		LEVEL, // -> PAUSE, END
		CHARACTER,
		SONG,
		GALLERY,
		PAUSE, // -> LEVEL
		END
	};
	bool bgm_playing;
	STATE state;
	STATE pre_state;
	ALLEGRO_EVENT event;
	ALLEGRO_BITMAP *game_icon;
	ALLEGRO_BITMAP *background;
	ALLEGRO_SAMPLE_INSTANCE *music;
	DataCenter *DC;
	SoundCenter *SC;
	ImageCenter *IC;
	FontCenter *FC;
private:
	ALLEGRO_DISPLAY *display;
	ALLEGRO_TIMER *timer;
	ALLEGRO_EVENT_QUEUE *event_queue;
public:
	void execute();
public:
	Game();
	~Game();
	void game_init();
	bool game_update();
	void game_draw();
	void change_state(Game::STATE new_state);
	void change_music(const char* song_path);
	void check_current_state();
};

#endif
