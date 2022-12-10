#include "Keyevent.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <iostream>


Keyevent::Keyevent(Engine *engine)
{
	_engine = engine;
}

act::action Keyevent::handler(ALLEGRO_KEYBOARD_STATE& kb){

    std::cout << "\n\n ENTROU HANDLER KEYEVENT \n\n";

    if (al_key_down(&kb, ALLEGRO_KEY_UP)) {
        _engine->set_movimento(act::action::MOVE_UP);
    }
    if (al_key_down(&kb, ALLEGRO_KEY_RIGHT)) {
        _engine->set_movimento(act::action::MOVE_RIGHT);
    }
    if (al_key_down(&kb, ALLEGRO_KEY_DOWN)) {
        _engine->set_movimento(act::action::MOVE_DOWN);
    }
    if (al_key_down(&kb, ALLEGRO_KEY_LEFT)) {
        _engine->set_movimento(act::action::MOVE_LEFT);
    }
    if (al_key_down(&kb, ALLEGRO_KEY_SPACE)) {
        //_engine->input(act::action::FIRE_PRIMARY);
    }
    if (al_key_down(&kb, ALLEGRO_KEY_1)) {
        //_engine->input(act::action::FIRE_SECONDARY);
    }
    if (al_key_down(&kb, ALLEGRO_KEY_ESCAPE)) {
        //_engine->input(act::action::QUIT_GAME);
    }

    return act::action::NO_ACTION;
}