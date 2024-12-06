#ifndef INPUTMANAGER_H_INCLUDED
#define INPUTMANAGER_H_INCLUDED

#include <allegro5/keyboard.h>
#include <allegro5/mouse.h>
#include <cstring>

/**
 * @brief Manages keyboard and mouse input states.
 * 
 * This class keeps track of which keys and mouse buttons are currently pressed.
 * It also provides functions to update these states each frame and to check for
 * "just pressed" or "just released" events by comparing the current and previous states.
 */
class InputManager {
public:
    static InputManager *get_instance() {
        static InputManager instance;
        return &instance;
    }

    void init() {
        memset(key_state, false, sizeof(key_state));
        memset(prev_key_state, false, sizeof(prev_key_state));
        memset(mouse_state, false, sizeof(mouse_state));
        memset(prev_mouse_state, false, sizeof(prev_mouse_state));
        mouse_x = 0; mouse_y = 0;
    }

    // Call this each frame after processing events.
    void update() {
        memcpy(prev_key_state, key_state, sizeof(key_state));
        memcpy(prev_mouse_state, mouse_state, sizeof(mouse_state));
    }

    // Event handlers to be called from the main event loop:
    void on_key_down(int keycode) {
        if(keycode >= 0 && keycode < ALLEGRO_KEY_MAX)
            key_state[keycode] = true;
    }

    void on_key_up(int keycode) {
        if(keycode >= 0 && keycode < ALLEGRO_KEY_MAX)
            key_state[keycode] = false;
    }

    void on_mouse_move(int x, int y) {
        mouse_x = x;
        mouse_y = y;
    }

    void on_mouse_button_down(int button) {
        if(button > 0 && button < ALLEGRO_MOUSE_MAX_EXTRA_AXES)
            mouse_state[button] = true;
    }

    void on_mouse_button_up(int button) {
        if(button > 0 && button < ALLEGRO_MOUSE_MAX_EXTRA_AXES)
            mouse_state[button] = false;
    }

    // Query functions:
    bool is_key_down(int keycode) const {
        return (keycode >= 0 && keycode < ALLEGRO_KEY_MAX) ? key_state[keycode] : false;
    }

    bool was_key_pressed(int keycode) const {
        return is_key_down(keycode) && !prev_key_state[keycode];
    }

    bool was_key_released(int keycode) const {
        return !is_key_down(keycode) && prev_key_state[keycode];
    }

    bool is_mouse_down(int button) const {
        return (button > 0 && button < ALLEGRO_MOUSE_MAX_EXTRA_AXES) ? mouse_state[button] : false;
    }

    bool was_mouse_pressed(int button) const {
        return is_mouse_down(button) && !prev_mouse_state[button];
    }

    bool was_mouse_released(int button) const {
        return !is_mouse_down(button) && prev_mouse_state[button];
    }

    int get_mouse_x() const { return mouse_x; }
    int get_mouse_y() const { return mouse_y; }

private:
    InputManager() = default;
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    bool key_state[ALLEGRO_KEY_MAX];
    bool prev_key_state[ALLEGRO_KEY_MAX];
    bool mouse_state[ALLEGRO_MOUSE_MAX_EXTRA_AXES];
    bool prev_mouse_state[ALLEGRO_MOUSE_MAX_EXTRA_AXES];

    int mouse_x, mouse_y;
};

#endif
