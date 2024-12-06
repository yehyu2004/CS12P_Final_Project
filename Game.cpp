#include "Game.h"


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
	// DC = DataCenter::get_instance();
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
	DC = DataCenter::get_instance();
	SC = SoundCenter::get_instance();
	IC = ImageCenter::get_instance();
	FC = FontCenter::get_instance();
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
	// set window icon
	game_icon = IC->get(game_icon_img_path);
	al_set_display_icon(display, game_icon);

	// register events to event_queue
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

	// init sound setting
	bgm_playing = false;
	music = nullptr;
	SC->init();

	// init font setting
	FC->init();

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


void Game::change_state(Game::STATE new_state){
	pre_state = state;
	bgm_playing = false;
	state = new_state;
}

void Game::change_music(const char* song_path){
	if(!bgm_playing) {
		if(music != nullptr) SC->toggle_playing(music);
		music = SC->play(song_path, ALLEGRO_PLAYMODE_LOOP);
		bgm_playing = true;
	}
}

void Game::check_current_state(){
	switch(state) {
		case STATE::START: {
			change_state(STATE::LEVEL);
			break;
		} case STATE::LEVEL: {
			pre_state=state;
			background = IC->get(background_img_path);
			change_music(background_sound_path);

			bool leftClicked = (DC->mouse_state[1] && !DC->prev_mouse_state[1]);

			if(leftClicked) {
				int mouse_x = DC->mouse.x;
                int mouse_y = DC->mouse.y;

                if (mouse_x >= 0 && mouse_x <= 899 && mouse_y >= 0 && mouse_y <= 671) {
					change_state(STATE::CHARACTER);
                }
				else if (mouse_x >= 0 && mouse_x <= 899 && mouse_y >= 671 && mouse_y <= 1344) {
					change_state(STATE::SONG);
                }
				else if (mouse_x >= 900 && mouse_x <= 1800 && mouse_y >= 671 && mouse_y <= 1344) {
					change_state(STATE::GALLERY);
                }
			}
			
			break;
		}case STATE::CHARACTER:{
			
			background = IC->get(character_img_path);
			change_music(character_sound_path);
	
			pre_state=state;
			if(DC->key_state[ALLEGRO_KEY_P] && !DC->prev_key_state[ALLEGRO_KEY_P]) {
				debug_log("<Game> state: change to LEVEL\n");
				ch_init=false;
				change_state(STATE::LEVEL);
			}
			break;
		}case STATE::SONG:{
			background = IC->get(song_img_path);
			//static bool BGM_played = false;
			// if(!BGM_played) {
			// 	music = SC->play(character_sound_path, ALLEGRO_PLAYMODE_LOOP);
			// 	BGM_played = true;
			// }
			if(pre_state!=STATE::SONG)
			{
				DC->character->init();
				ch_init=true;
			}
			else
			{
				DC->character->update();
			}

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
			// 	music = SC->play(character_sound_path, ALLEGRO_PLAYMODE_LOOP);
			// 	BGM_played = true;
			// }
			if(intial_time==0)
			{
				intial_time=al_get_time();
			}

			double current=al_get_time();

			if(current-intial_time<2.0)
			{
				paint=true;
			}
			else
			{
				paint=false;
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
				SC->toggle_playing(music);
				debug_log("<Game> state: change to LEVEL\n");
				state = STATE::LEVEL;
			}
			break;
		}
	}
}

/**
 * @brief The function processes all data update.
 * @details The behavior of the whole game body is determined by its state.
 * @return Whether the game should keep running (true) or reaches the termination criteria (false).
 * @see Game::STATE
 */
bool
Game::game_update() {
	// debug_log("RUN!!!\n");
	check_current_state();
	
	
	// If the game is not paused, we should progress update.
	if(state != STATE::PAUSE) {
		//DC->player->update();
		SC->update();
		//DC->character->update();
		if(state != STATE::START) {
			//DC->level->update();
		}
	}

	if(state == STATE::END){
		return false;
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
		}
	}
	switch(state) {
		case STATE::START: {
			break;
		} case STATE::LEVEL: {
			break;
		} case STATE::CHARACTER: {
			break;
		}case STATE::SONG: {
			al_draw_filled_rectangle(0, 450, 1800 ,500, al_map_rgba(100, 0, 0, 100));
			if(ch_init)
			{
				DC->character->draw();
			}
			
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
