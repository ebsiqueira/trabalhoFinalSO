#include "main_class.h"

__BEGIN_API

Engine *Main::window;
Player *Main::player;
Creep *Main::enemyCreep;
Keyevent *Main::event;


Thread *Main::window_thread;
Thread *Main::player_thread;
Thread *Main::enemy_thread;
Thread *Main::event_thread;


__END_API