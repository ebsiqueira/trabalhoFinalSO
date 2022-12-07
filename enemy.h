#ifndef enemy_h
#define enemy_h

#include <allegro5/allegro.h>
#include "Objeto.h"
#include "Vector.h"

class Enemy : public Objeto {

    public:
        Enemy(int x, int y, Vector speed, int life, int size, bool dead, bool canShoot, ALLEGRO_COLOR color);
        ~Enemy();

    private:
}