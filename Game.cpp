#include "Game.h"
#include "scenes/MenuScene/MenuScene.h"
#include "scenes/CharacterScene/CharacterScene.h"
#include "scenes/SongScene/SongScene.h"
#include "scenes/GalleryScene/GalleryScene.h"
#include "Utils.h"

Game::Game() {
    RM = ResourceManager::get_instance();
    CM = ConfigManager::get_instance();
    IM = InputManager::get_instance();

    CM->load_from_file("configs/game_config.json");
    RM->load_config("configs/resources.json");

    GAME_ASSERT(al_init(), "failed to initialize allegro.");

    init_addons();
    init_events();
    allegro_setup();
}

Game::~Game() {
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
}

void Game::init_addons() {
    bool addon_init = true;
    addon_init &= al_init_primitives_addon();
    addon_init &= al_init_font_addon();
    addon_init &= al_init_ttf_addon();
    addon_init &= al_init_image_addon();
    addon_init &= al_init_acodec_addon();
    GAME_ASSERT(addon_init, "failed to initialize allegro addons.");
}

void Game::init_events() {
    bool event_init = true;
    event_init &= al_install_keyboard();
    event_init &= al_install_mouse();
    event_init &= al_install_audio();
    GAME_ASSERT(event_init, "failed to initialize allegro events.");
}

void Game::allegro_setup() {
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
    // Start with the MENU scene
    change_scene("MENU");

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_start_timer(timer);
    running = true;
}

void Game::execute() {
    IM->init();

    while(running) {
        al_wait_for_event(event_queue, &event);

        switch(event.type) {
            case ALLEGRO_EVENT_TIMER: {
                running &= game_update();
                game_draw();
                IM->update();
                break;
            }
            case ALLEGRO_EVENT_DISPLAY_CLOSE: {
                running = false;
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

bool Game::game_update() {
    if (current_scene) {
        current_scene->handle_input();
        return current_scene->update();
    }
    return false; 
}

void Game::game_draw() {
    al_clear_to_color(al_map_rgb(100, 100, 100));
    if (current_scene) {
        current_scene->draw();
    }
    al_flip_display();
}

// Define a template function, not a lambda
template <typename SceneType>
std::unique_ptr<Scene> make_scene(ResourceManager* RM, ConfigManager* CM, InputManager* IM, std::function<void(const std::string&)> changer) {
    return std::make_unique<SceneType>(RM, CM, IM, changer);
}

void Game::change_scene(const std::string& scene_name) {
    current_scene.reset();

    if (scene_name == "MENU") {
        current_scene = make_scene<MenuScene>(RM, CM, IM, [this](const std::string &s){change_scene(s);});
    } else if (scene_name == "CHARACTER") {
        current_scene = make_scene<CharacterScene>(RM, CM, IM, [this](const std::string &s){change_scene(s);});
    } else if (scene_name == "SONG") {
        current_scene = make_scene<SongScene>(RM, CM, IM, [this](const std::string &s){change_scene(s);});
    } else if (scene_name == "GALLERY") {
        current_scene = make_scene<GalleryScene>(RM, CM, IM, [this](const std::string &s){change_scene(s);});
    }

    if (current_scene) {
        current_scene->init();
    }
}
