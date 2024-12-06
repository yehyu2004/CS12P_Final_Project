#include "Game.h"

//variable
bool paint;
double intial_time;
bool ch_init;

void Game::execute() {
    IM = InputManager::get_instance();
    IM->init();

    bool run = true;
    while(run) {
        al_wait_for_event(event_queue, &event);

        switch(event.type) {
            case ALLEGRO_EVENT_TIMER: {
                run &= game_update();
                game_draw();

                // After updating the game, update input states to previous for next frame
                IM->update();
                break;
            }
            case ALLEGRO_EVENT_DISPLAY_CLOSE: {
                run = false;
                break;
            }
            case ALLEGRO_EVENT_KEY_DOWN: {
                IM->on_key_down(event.keyboard.keycode);
                break;
            }
            case ALLEGRO_EVENT_KEY_UP: {
                IM->on_key_up(event.keyboard.keycode);
                break;
            }
            case ALLEGRO_EVENT_MOUSE_AXES: {
                IM->on_mouse_move(event.mouse.x, event.mouse.y);
                break;
            }
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: {
                IM->on_mouse_button_down(event.mouse.button);
                break;
            }
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
                IM->on_mouse_button_up(event.mouse.button);
                break;
            }
            default:
                break;
        }
    }
}

Game::Game() {
    RM = ResourceManager::get_instance();
    CM = ConfigManager::get_instance();

    CM->load_from_file("configs/config.json");
    RM->load_config("configs/resources.json");

    // Initialize Allegro
    GAME_ASSERT(al_init(), "failed to initialize allegro.");

    // Initialize allegro addons
    bool addon_init = true;
    addon_init &= al_init_primitives_addon();
    addon_init &= al_init_font_addon();
    addon_init &= al_init_ttf_addon();
    addon_init &= al_init_image_addon();
    addon_init &= al_init_acodec_addon();
    GAME_ASSERT(addon_init, "failed to initialize allegro addons.");

    // Initialize events
    bool event_init = true;
    event_init &= al_install_keyboard();
    event_init &= al_install_mouse();
    event_init &= al_install_audio();
    GAME_ASSERT(event_init, "failed to initialize allegro events.");

    // Use the config manager values
    int width = CM->get_window_width();
    int height = CM->get_window_height();
    double FPS = CM->get_fps();

    RM->init_audio(); // Initialize audio through RM

    al_set_new_display_flags(ALLEGRO_NOFRAME);
    GAME_ASSERT(
        display = al_create_display(width, height),
        "failed to create display."
    );
    al_set_window_position(display, 0, 0);
    GAME_ASSERT(
        timer = al_create_timer(1.0 / FPS),
        "failed to create timer."
    );
    GAME_ASSERT(
        event_queue = al_create_event_queue(),
        "failed to create event queue."
    );

    debug_log("Game initialized.\n");
    game_init();
}

void Game::game_init() {
    // Load the game icon from RM
    game_icon = RM->get_image("game_icon");
    al_set_display_icon(display, game_icon);

    // Register events
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    bgm_playing = false;
    music = nullptr;

    // Initialize character now that IM and CM are ready if needed
    character.init();

    // Start state
    debug_log("Game state: change to START\n");
    state = STATE::START;
    al_start_timer(timer);
    intial_time = 0;
    paint = false;
    ch_init = false;
}

void Game::change_state(Game::STATE new_state) {
    pre_state = state;
    current_buttons.clear();
    bgm_playing = false;
    state = new_state;
}

void Game::change_music(const char* sound_key) {
    if (!bgm_playing) {
        if (music != nullptr) {
            RM->toggle_sound(music);
        }
        // Play sound by key defined in resources.json
        music = RM->play_sound(sound_key, ALLEGRO_PLAYMODE_LOOP);
        bgm_playing = true;
    }
}

void Game::check_current_state() {
    // For one-shot actions, use IM->was_key_pressed().
    // For continuous movement, use IM->is_key_down().
    switch(state) {
        case STATE::START: {
            background = RM->get_image("background");
            change_state(STATE::LEVEL);
            break;
        }
        case STATE::LEVEL: {
            if(pre_state != state) {
                Button play_music_btn(1300, 500, RM->get_image(play_music_key), [&](){change_state(STATE::SONG);});
                Button gallery_btn(1300, 700, RM->get_image(gallery_key), [&](){change_state(STATE::GALLERY);});
                Button character_btn(1300, 900, RM->get_image(character_key), [&](){change_state(STATE::CHARACTER);});
                current_buttons.push_back(play_music_btn);
                current_buttons.push_back(gallery_btn);
                current_buttons.push_back(character_btn);
                pre_state = state;
            }

            background = RM->get_image("background");
            change_music("menu_bgm");

            bool leftClicked = IM->was_mouse_pressed(1);
            int mouse_x = IM->get_mouse_x();
            int mouse_y = IM->get_mouse_y();

            for(Button &button : current_buttons) {
                button.update(mouse_x, mouse_y);
            }

            if(leftClicked) {
                for(Button &button : current_buttons) {
                    button.clicked();
                }
            }

            break;
        }
        case STATE::CHARACTER: {
            background = RM->get_image("character_img");
            change_music("character_bgm");

            pre_state = state;
            if(IM->was_key_pressed(ALLEGRO_KEY_P)) {
                debug_log("<Game> state: change to LEVEL\n");
                ch_init=false;
                change_state(STATE::LEVEL);
            }
            break;
        }
        case STATE::SONG: {
            background = RM->get_image("song_background");
            if(pre_state != STATE::SONG) {
                character.init();
                ch_init = true;
            } else {
                character.update();
            }

            pre_state = state;
            if(IM->was_key_pressed(ALLEGRO_KEY_0)) {
                change_state(STATE::LEVEL);
            }
            break;
        }
        case STATE::GALLERY: {
            background = RM->get_image("gallery_background");
            if(intial_time == 0) {
                intial_time = al_get_time();
            }

            double current = al_get_time();
            paint = (current - intial_time < 2.0);

            pre_state = state;
            if(IM->was_key_pressed(ALLEGRO_KEY_0)) {
                change_state(STATE::LEVEL);
            }
            break;
        }
        case STATE::PAUSE: {
            pre_state = state;
            if(IM->was_key_pressed(ALLEGRO_KEY_P)) {
                RM->toggle_sound(music);
                debug_log("<Game> state: change to LEVEL\n");
                state = STATE::LEVEL;
            }
            break;
        }
        case STATE::END: {
            // Handle end state
            break;
        }
    }
}

bool Game::game_update() {
    check_current_state();

    if(state != STATE::PAUSE) {
        RM->update_sounds();
        // Update other game logic if needed
    }

    if(state == STATE::END) {
        return false;
    }

    // No DataCenter used now, no need to memcpy states
    return true;
}

void Game::draw_background() {
    if(state != STATE::END) {
        al_draw_bitmap(background, 0, 0, 0);
    }
}

void Game::draw_states() {
    switch(state) {
        case STATE::START:
            break;
        case STATE::LEVEL:
            break;
        case STATE::CHARACTER:
            break;
        case STATE::SONG:
            if(ch_init) {
                character.draw();
            }
            break;
        case STATE::GALLERY:
            // Additional drawing if needed
            break;
        case STATE::PAUSE: {
            al_draw_filled_rectangle(0, 0, CM->get_window_width(), CM->get_window_height(), al_map_rgba(50, 50, 50, 64));
            ALLEGRO_FONT* pause_font = RM->get_font("caviar_dreams", 36);
            al_draw_text(
                pause_font, al_map_rgb(255, 255, 255),
                CM->get_window_width()/2.0, CM->get_window_height()/2.0,
                ALLEGRO_ALIGN_CENTRE, "GAME PAUSED"
            );
            break;
        }
        case STATE::END:
            break;
    }
}

void Game::draw_buttons() {
    for(Button &button : current_buttons) {
        button.draw();
    }
}

void Game::game_draw() {
    al_clear_to_color(al_map_rgb(100, 100, 100));
    draw_background();
    draw_states();
    draw_buttons();

    ALLEGRO_COLOR green = al_map_rgb(0, 255, 0);
    // Draw mouse position
    int mx = IM->get_mouse_x();
    int my = IM->get_mouse_y();
    al_draw_circle(mx, my, 5, green, 5.0);

    al_flip_display();
}

Game::~Game() {
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
}
