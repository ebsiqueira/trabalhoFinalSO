#ifndef keyevent_h
#define keyevent_h

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "Action.h"
#include "Engine.h"

class Keyevent
{
	public:
        Keyevent(Engine *engine);
        
        act::action handler(ALLEGRO_KEYBOARD_STATE& kb);
		
	private:
        Engine *_engine;
};

#endif