#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED

class Scene {
public:
    virtual ~Scene() {}
    virtual void init() = 0;       // Called when the scene is first set
    virtual bool update() = 0;     // Return false if the scene wants to end the game
    virtual void draw() = 0;       // Draw the scene
    virtual void handle_input() = 0; // Process input (if not handled globally)
};

#endif
