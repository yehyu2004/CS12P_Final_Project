#include "Game.h"
#include "scenes/MenuScene/MenuScene.h"
#include "scenes/CharacterScene/CharacterScene.h"
#include "scenes/SongScene/SongScene.h"
#include "scenes/NyancatScene4k/NyancatScene4k.h"
#include "scenes/NyancatScene6k/NyancatScene6k.h"
#include "scenes/SuccessScene/SuccessScene.h"
#include "scenes/LoseScene/LoseScene.h"
#include "scenes/GalleryScene/GalleryScene.h"
#include "scenes/MusicstartScene/MusicstartScene.h"
#include "scenes/Musicstart2Scene/Musicstart2Scene.h"
#include "Utils.h"
#include <iostream>
Game::Game() {
    // Initialize resource, config, and input managers
    RM = ResourceManager::get_instance();
    CM = ConfigManager::get_instance();
    IM = InputManager::get_instance();

    // Load configuration and resources
    CM->load_from_file("configs/game_config.json");
    RM->load_config("configs/resources.json");

    GAME_ASSERT(al_init(), "Failed to initialize Allegro.");

    init_addons();    // Initialize Allegro addons
    init_events();    // Initialize input/audio events
    allegro_setup();  // Setup Allegro display, timer, etc.

    debug_log("Game initialized.\n");
    game_init();
}

Game::~Game() {
    // Clean up allocated resources
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
    GAME_ASSERT(addon_init, "Failed to initialize Allegro addons.");
}

void Game::init_events() {
    bool event_init = true;
    event_init &= al_install_keyboard();
    event_init &= al_install_mouse();
    event_init &= al_install_audio();
    GAME_ASSERT(event_init, "Failed to initialize Allegro events.");
}

void Game::allegro_setup() {
    int width = CM->get_window_width();
    int height = CM->get_window_height();
    double FPS = CM->get_fps();

    // Initialize audio
    RM->init_audio(); 

    al_set_new_display_flags(ALLEGRO_NOFRAME);
    GAME_ASSERT((display = al_create_display(width, height)), "Failed to create display.");
    al_set_window_position(display, 0, 0);

    GAME_ASSERT((timer = al_create_timer(1.0 / FPS)), "Failed to create timer.");
    GAME_ASSERT((event_queue = al_create_event_queue()), "Failed to create event queue.");
}

void Game::game_init() {
    // Start the game with the MENU scene
    change_scene("MENU");

    // Register event sources
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    // Start the main timer and set running state to true
    al_start_timer(timer);
    running = true;
}

void Game::execute() {
    IM->init();

    while (running) {
        al_wait_for_event(event_queue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                running &= game_update();
                game_draw();
                IM->update();
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                running = false;
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                IM->on_key_down(event.keyboard.keycode);
                break;
            case ALLEGRO_EVENT_KEY_UP:
                IM->on_key_up(event.keyboard.keycode);
                break;
            case ALLEGRO_EVENT_MOUSE_AXES:
                IM->on_mouse_move(event.mouse.x, event.mouse.y);
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                IM->on_mouse_button_down(event.mouse.button);
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                IM->on_mouse_button_up(event.mouse.button);
                break;
            default:
                // Unhandled event
                break;
        }
    }
}

bool Game::game_update() {
    // Update current scene if it exists
    if (current_scene) {
        current_scene->handle_input();
        return current_scene->update();
    }
    return false;
}

void Game::game_draw() {
    // Clear background
    al_clear_to_color(al_map_rgb(100, 100, 100));

    // Draw current scene
    if (current_scene) {
        current_scene->draw();
    }

    // Flip display buffers
    al_flip_display();
}

// Template function for creating scenes
template <typename SceneType>
std::unique_ptr<Scene> make_scene(ResourceManager* RM, ConfigManager* CM, InputManager* IM, std::function<void(const std::string&)> changer) {
    return std::make_unique<SceneType>(RM, CM, IM, changer);
}

void Game::change_scene(const std::string& scene_name) {
    current_scene.reset();

    // Capture 'this' once for use in lambda
    auto scene_changer = [this](const std::string &s) {
        this->change_scene(s);
    };
    std::cout<<scene_name<<'\n';
    // Decide which scene to create based on scene_name
    if (scene_name == "MENU") {
        current_scene = make_scene<MenuScene>(RM, CM, IM, scene_changer);
    } else if (scene_name == "CHARACTER") {
        current_scene = make_scene<CharacterScene>(RM, CM, IM, scene_changer);
    } else if (scene_name == "SONG") {
        current_scene = make_scene<SongScene>(RM, CM, IM, scene_changer);
    } else if (scene_name == "GALLERY") {
        current_scene = make_scene<GalleryScene>(RM, CM, IM, scene_changer);
    } else if (scene_name == "MUSICSTART") {
        current_scene = make_scene<MusicstartScene>(RM, CM, IM, scene_changer);
    } else if (scene_name == "MUSICSTART2") {
        current_scene = make_scene<Musicstart2Scene>(RM, CM, IM, scene_changer);
    } else if (scene_name == "NYANCAT4K") {
        current_scene = make_scene<Nyancat4kScene>(RM, CM, IM, scene_changer);
    } else if (scene_name == "NYANCAT6K") {
        current_scene = make_scene<Nyancat6kScene>(RM, CM, IM, scene_changer);
    } else if (scene_name == "SUCCESS") {
        current_scene = make_scene<SuccessScene>(RM, CM, IM, scene_changer);
    } else if (scene_name == "LOSE") {
        current_scene = make_scene<LoseScene>(RM, CM, IM, scene_changer);
    }

    // Initialize the newly created scene if any
    if (current_scene) {
        current_scene->init();
    }
}
