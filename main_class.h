#ifndef main_class_h
#define main_class_h

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdexcept>
#include <iostream>
#include "cpu.h"
#include "traits.h"
#include "thread.h"
#include "semaphore.h"
#include "Engine.h"
#include "Player.h"
#include "Keyevent.h"
#include "semaphore.h"

__BEGIN_API

class Main
{
public:
    Main() {
    }

    static void run(void *name){
        player = new Player(Point(215, 245), al_map_rgb(0, 200, 0));
        window = new Engine(800, 600, 60, player);
        event = new Keyevent(window);

        window->init();

        std::cout<<"\n\n WINDOW CRIADA: ";
        window_thread = new Thread(window_run);
        std::cout<<"\n\n EVENT CRIADA: ";
        event_thread = new Thread(event_run);
        std::cout<<"\n\n PLAYER CRIADA: ";
        player_thread = new Thread(player_run);
        std::cout<<"\n\n ENEMY CRIADA: ";
        enemy_thread = new Thread(enemy_run);

        std::cout<<"\nacabou?\n";
        window_thread->join();
        std::cout<<"\n CU?\n";
        delete window_thread;
        delete player_thread;
        delete enemy_thread;
        delete event_thread;

        delete window;

    }

    ~Main() {}

private:

static Engine *window;
static Player *player;
static Creep *enemyCreep;
static Keyevent *event;

static Thread *window_thread;
static Thread *player_thread;
static Thread *enemy_thread;
static Thread *event_thread;

static void event_run(){
    while(!window->_finish){
        std::cout<<"\n ENTROU EM EVENT\n";
        ALLEGRO_KEYBOARD_STATE kb;
        al_get_keyboard_state(&kb);
        act::action actionReturn = event->handler(kb);
        Thread::yield();
    }
}

static void window_run(){
    std::cout<<"\n ENTROU EM WINDOW";
        
        float timer = 0;

        while(!window->_finish){
            std::cout<<"\n ENTROU EM WINDOW\n";
            window->run(timer);
            std::cout<<"\n YIELD()\n";
            Thread::yield();
        }
        window_thread->thread_exit(0);
    }

static void player_run(){
    while(!window->_finish){
        std::cout<<"\n ENTROU EM PLAYER\n";
        player->move();
        Thread::yield();
    }
}

static void enemy_run(){
    while(!window->_finish){
        std::cout<<"\n ENTROU EM ENEMY\n";
        Thread::yield();
    }
}




};

__END_API

#endif
