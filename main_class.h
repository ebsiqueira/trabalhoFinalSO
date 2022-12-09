#ifndef main_class_h
#define main_class_h

#include <iostream>
#include "cpu.h"
#include "traits.h"
#include "thread.h"
#include "semaphore.h"
#include "Engine.h"
#include "Player.h"

__BEGIN_API

class Main
{
public:
    Main() {
    }

    static void run(void *name){
        
        std::cout<<"\n\n WINDOW CRIADA: ";
        window_thread = new Thread(window_run);
        std::cout<<"\n\n PLAYER CRIADA: ";
        player_thread = new Thread(player_run);
        std::cout<<"\n\n ENEMY CRIADA: ";
        enemy_thread = new Thread(enemy_run);

        std::cout<<"\nacabou?\n";
        window_thread->join();
        std::cout<<"\n CU?\n";
        delete window_thread;
        delete player_thread;

        delete window;

    }

    ~Main() {}

private:

static Engine *window;
static Player *player;
static Enemy *enemy;

static Thread *window_thread;
static Thread *player_thread;
static Thread *enemy_thread;


static void window_run(){
    std::cout<<"\n ENTROU EM WINDOW";
        window = new Engine(800, 600, 60);
        window->init();

        float timer = 0;

        while(!window->_finish){
            std::cout<<"\n ENTROU EM LOOP de run";
            window->run(timer);
            std::cout<<"\n YIELD()\n";
            Thread::yield();
        }
        window_thread->thread_exit(0);
    }

static void player_run(){
    //player = new Player(Point(215, 245), al_map_rgb(0, 200, 0));
    while(!window->_finish){
        std::cout<<"\n ENTROU EM PLAYER";
        Thread::yield();
    }
}

static void enemy_run(){
    while(!window->_finish){
        std::cout<<"\n ENTROU EM PLAYER";
        Thread::yield();
    }
}




};

__END_API

#endif
