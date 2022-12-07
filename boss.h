#ifndef boss_h
#define boss_h

#include <allegro5/allegro.h>
#include "Objeto.h"
#include "Enemy.h"
#include "Vector.h"

class Boss : public Enemy {
    
    public:
        Boss(int x, int y, Vector speed, int life, int size, bool dead, bool canShoot, ALLEGRO_COLOR color, bool isAlive);
        ~Boss();

    private:

}