#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#include <allegro5/allegro.h>
#include "UI.h"

class Character
{
public:
	void execute();
public:
	Character();
	~Character();
	void character_init();
	bool character_update();
	void character_draw();
private:
	ALLEGRO_EVENT event;
	ALLEGRO_BITMAP *game_icon;
	ALLEGRO_BITMAP *background;
private:
	ALLEGRO_DISPLAY *display;
	ALLEGRO_TIMER *timer;
	ALLEGRO_EVENT_QUEUE *event_queue;
	UI *ui;
};

#endif