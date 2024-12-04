#include "Game.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "data/OperationCenter.h"
#include "data/SoundCenter.h"
#include "data/ImageCenter.h"
#include "data/FontCenter.h"
#include "Player.h"
#include "Level.h"
#include "Character.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
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

//variable
bool paint;
double intial_time;
ALLEGRO_BITMAP *gallery_ima;
bool ch_init;
/**
 * @brief Game entry.
 * @details The function processes all allegro events and update the event state to a generic data storage (i.e. DataCenter).
 * For timer event, the game_update and game_draw function will be called if and only if the current is timer.
 */
void
Game::execute() {
	DataCenter *DC = DataCenter::get_instance();
	// main game loop
	bool run = true;
	while(run) {
		// process all events here
		al_wait_for_event(event_queue, &event);
		switch(event.type) {
			case ALLEGRO_EVENT_TIMER: {
				run &= game_update();
				game_draw();
				break;
			} case ALLEGRO_EVENT_DISPLAY_CLOSE: { // stop game
				run = false;
				break;
			} case ALLEGRO_EVENT_KEY_DOWN: {
				DC->key_state[event.keyboard.keycode] = true;
				break;
			} case ALLEGRO_EVENT_KEY_UP: {
				DC->key_state[event.keyboard.keycode] = false;
				break;
			} case ALLEGRO_EVENT_MOUSE_AXES: {
				DC->mouse.x = event.mouse.x;
				DC->mouse.y = event.mouse.y;
				break;
			} case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: {
				DC->mouse_state[event.mouse.button] = true;
				break;
			} case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
				DC->mouse_state[event.mouse.button] = false;
				break;
			} default: break;
		}
	}
}

/**
 * @brief Initialize all allegro addons and the game body.
 * @details Only one timer is created since a game and all its data should be processed synchronously.
 */
Game::Game() {
	DataCenter *DC = DataCenter::get_instance();
	GAME_ASSERT(al_init(), "failed to initialize allegro.");

	// initialize allegro addons
	bool addon_init = true;
	addon_init &= al_init_primitives_addon();
	addon_init &= al_init_font_addon();
	addon_init &= al_init_ttf_addon();
	addon_init &= al_init_image_addon();
	addon_init &= al_init_acodec_addon();
	GAME_ASSERT(addon_init, "failed to initialize allegro addons.");

	// initialize events
	bool event_init = true;
	event_init &= al_install_keyboard();
	event_init &= al_install_mouse();
	event_init &= al_install_audio();
	GAME_ASSERT(event_init, "failed to initialize allegro events.");

	// initialize game body
	GAME_ASSERT(
		display = al_create_display(DC->window_width, DC->window_height),
		"failed to create display.");
	GAME_ASSERT(
		timer = al_create_timer(1.0 / DC->FPS),
		"failed to create timer.");
	GAME_ASSERT(
		event_queue = al_create_event_queue(),
		"failed to create event queue.");

	debug_log("Game initialized.\n");
	game_init();
}

/**
 * @brief Initialize all auxiliary resources.
 */
void
Game::game_init() {
	DataCenter *DC = DataCenter::get_instance();
	SoundCenter *SC = SoundCenter::get_instance();
	ImageCenter *IC = ImageCenter::get_instance();
	FontCenter *FC = FontCenter::get_instance();
	// set window icon
	game_icon = IC->get(game_icon_img_path);
	al_set_display_icon(display, game_icon);

	// register events to event_queue
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

	// init sound setting
	SC->init();

	// init font setting
	FC->init();

	ui = new UI();
	ui->init();

	//DC->level->init();
	 //DC->character->init();
	// game start
	background = IC->get(background_img_path);
	gallery_ima=IC->get(get_img_path);
	debug_log("Game state: change to START\n");
	state = STATE::START;
	al_start_timer(timer);
	intial_time=0;
	paint=false;
	ch_init=false;
	
}

/**
 * @brief The function processes all data update.
 * @details The behavior of the whole game body is determined by its state.
 * @return Whether the game should keep running (true) or reaches the termination criteria (false).
 * @see Game::STATE
 */
bool
Game::game_update() {
	DataCenter *DC = DataCenter::get_instance();
	OperationCenter *OC = OperationCenter::get_instance();
	ImageCenter *IC = ImageCenter::get_instance();
	SoundCenter *SC = SoundCenter::get_instance();
	static ALLEGRO_SAMPLE_INSTANCE *backgroundmusic = nullptr;
	//static ALLEGRO_SAMPLE_INSTANCE *charactermusic = nullptr;
	
	// debug_log("pre:\n");
	// switch(pre_state) {
	// 	case STATE::START: {
	// 		debug_log("start\n");
	// 		break;
	// 	} case STATE::LEVEL: {
	// 		debug_log("level\n");
	// 		break;
	// 	} case STATE::CHARACTER: {
	// 		debug_log("Character\n");
	// 		break;
	// 	}case STATE::SONG: {
	// 		debug_log("song\n");
	// 		break;
	// 	}  case STATE::GALLERY: {
	// 		debug_log("gallery\n");
	// 		break;
	// 	} case STATE::PAUSE: {
	// 		debug_log("pause");
	// 		break;
	// 	} case STATE::END: {
	// 	}
	// }
	// debug_log("now:\n");
	// switch(state) {
	// 	case STATE::START: {
	// 		debug_log("start\n");
	// 		break;
	// 	} case STATE::LEVEL: {
	// 		debug_log("level\n");
	// 		break;
	// 	} case STATE::CHARACTER: {
	// 		debug_log("Character\n");
	// 		break;
	// 	}case STATE::SONG: {
	// 		debug_log("song\n");
	// 		break;
	// 	}  case STATE::GALLERY: {
	// 		debug_log("gallery\n");
	// 		break;
	// 	} case STATE::PAUSE: {
	// 		debug_log("pause");
	// 		break;
	// 	} case STATE::END: {
	// 	}
	// }

	
	switch(state) {
		case STATE::START: {
			pre_state=state;
			static bool is_played = false;
			static ALLEGRO_SAMPLE_INSTANCE *instance = nullptr;
			if(!is_played) {
				instance = SC->play(game_start_sound_path, ALLEGRO_PLAYMODE_ONCE);
				DC->level->load_level(1);
				is_played = true;
			}

			if(!SC->is_playing(instance)) {
				debug_log("<Game> state: change to LEVEL\n");
				state = STATE::LEVEL;
			}
			
			break;
		} case STATE::LEVEL: {
			pre_state=state;
			background = IC->get(background_img_path);
			static bool BGM_played = false;
			if(!BGM_played) {
				backgroundmusic = SC->play(background_sound_path, ALLEGRO_PLAYMODE_LOOP);
				BGM_played = true;
			}
			if(DC->key_state[ALLEGRO_KEY_0]&&!DC->prev_key_state[ALLEGRO_KEY_0])
			{
				SC->toggle_playing(backgroundmusic);
				state=STATE::PAUSE;
			}
	
			if(DC->mouse_state[1] && !DC->prev_mouse_state[1]) {
				int mouse_x = DC->mouse.x;
                int mouse_y = DC->mouse.y;

                if (mouse_x >= 0 && mouse_x <= 899 && mouse_y >= 0 && mouse_y <= 671) {
					//SC->toggle_playing(backgroundmusic);
					pre_state=state;
                    state = STATE::CHARACTER;
                }
				else if (mouse_x >= 0 && mouse_x <= 899 && mouse_y >= 671 && mouse_y <= 1344) {
					pre_state=state;
                    state = STATE::SONG;
                }
				else if (mouse_x >= 900 && mouse_x <= 1800 && mouse_y >= 671 && mouse_y <= 1344) {
					pre_state=state;
                    state = STATE::GALLERY;
                }
			}
			
			break;
		}case STATE::CHARACTER:{
			
			background = IC->get(character_img_path);
			// static bool BGM_played = false;
			// if(!BGM_played) {
			// 	charactermusic = SC->play(character_sound_path, ALLEGRO_PLAYMODE_LOOP);
			// 	BGM_played = true;
			// }
			if(pre_state!=STATE::CHARACTER)
			{
				DC->character->init();
				ch_init=true;
			}
			else
			{
				DC->character->update();
			}
			pre_state=state;
			if(DC->key_state[ALLEGRO_KEY_P] && !DC->prev_key_state[ALLEGRO_KEY_P]) {
				debug_log("<Game> state: change to LEVEL\n");
				ch_init=false;
				state = STATE::LEVEL;
			}
			break;
		}case STATE::SONG:{
			background = IC->get(song_img_path);
			//static bool BGM_played = false;
			// if(!BGM_played) {
			// 	backgroundmusic = SC->play(character_sound_path, ALLEGRO_PLAYMODE_LOOP);
			// 	BGM_played = true;
			// }
			pre_state=state;
			if(DC->key_state[ALLEGRO_KEY_0]&&!DC->prev_key_state[ALLEGRO_KEY_0])
			{
				state=STATE::LEVEL;
			}
			break;
		} case STATE::GALLERY:{
			background = IC->get(gallery_img_path);
			//static bool BGM_played = false;
			// if(!BGM_played) {
			// 	backgroundmusic = SC->play(character_sound_path, ALLEGRO_PLAYMODE_LOOP);
			// 	BGM_played = true;
			// }
			if(intial_time==0)
			{
				intial_time=al_get_time();
				//debug_log("initial:",intial_time,"\n");
			}

			double current=al_get_time();

			if(current-intial_time<2.0)
			{
				paint=true;
				//debug_log("display\n");
			}
			else
			{
				paint=false;
				//debug_log("not display\n");
			}
			pre_state=state;
			if(DC->key_state[ALLEGRO_KEY_0]&&!DC->prev_key_state[ALLEGRO_KEY_0])
			{
				state=STATE::LEVEL;
			}
			break;
		} case STATE::PAUSE: {
			pre_state=state;
			if(DC->key_state[ALLEGRO_KEY_P] && !DC->prev_key_state[ALLEGRO_KEY_P]) {
				SC->toggle_playing(backgroundmusic);
				debug_log("<Game> state: change to LEVEL\n");
				state = STATE::LEVEL;
			}
			break;
		} case STATE::END: {
			return false;
		}
	}
	
	// If the game is not paused, we should progress update.
	if(state != STATE::PAUSE) {
		//DC->player->update();
		SC->update();
		ui->update();
		//DC->character->update();
		if(state != STATE::START) {
			//DC->level->update();
			OC->update();
		}
	}
	// game_update is finished. The states of current frame will be previous states of the next frame.
	memcpy(DC->prev_key_state, DC->key_state, sizeof(DC->key_state));
	memcpy(DC->prev_mouse_state, DC->mouse_state, sizeof(DC->mouse_state));
	return true;
}

/**
 * @brief Draw the whole game and objects.
 */
void
Game::game_draw() {
	DataCenter *DC = DataCenter::get_instance();
	OperationCenter *OC = OperationCenter::get_instance();
	FontCenter *FC = FontCenter::get_instance();

	// Flush the screen first.
	al_clear_to_color(al_map_rgb(100, 100, 100));
	if(state != STATE::END) {
		// background
		al_draw_bitmap(background, 0, 0, 0);
		if(DC->game_field_length < DC->window_width)
			al_draw_filled_rectangle(
				DC->game_field_length, 0,
				DC->window_width, DC->window_height,
				al_map_rgb(100, 100, 100));
		if(DC->game_field_length < DC->window_height)
			al_draw_filled_rectangle(
				0, DC->game_field_length,
				DC->window_width, DC->window_height,
				al_map_rgb(100, 100, 100));
		// user interface
		if(state != STATE::START) {
			//DC->level->draw();
			//DC->character->draw();
			ui->draw();
			OC->draw();
		}
	}
	switch(state) {
		case STATE::START: {
			break;
		} case STATE::LEVEL: {
			break;
		} case STATE::CHARACTER: {
			if(ch_init)
			{
				DC->character->draw();
			}
			break;
		}case STATE::SONG: {
			break;
		}  case STATE::GALLERY: {
			 if(paint)
			 {
				al_draw_filled_rectangle(0, 0, DC->window_width, DC->window_height, al_map_rgba(100, 0, 0, 64));
				al_draw_bitmap(gallery_ima, 0, 0, 0);
			 }
			
			break;
		} case STATE::PAUSE: {
			// game layout cover
			al_draw_filled_rectangle(0, 0, DC->window_width, DC->window_height, al_map_rgba(50, 50, 50, 64));
			al_draw_text(
				FC->caviar_dreams[FontSize::LARGE], al_map_rgb(255, 255, 255),
				DC->window_width/2., DC->window_height/2.,
				ALLEGRO_ALIGN_CENTRE, "GAME PAUSED");
			break;
		} case STATE::END: {
		}
	}
	al_flip_display();
}

Game::~Game() {
	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
}
