#ifndef enemy_h
#define enemy_h

#include <allegro5/allegro.h>
#include "Objeto.h"
#include "Vector.h"
#include "Point.h"

class Enemy : public Objeto {

    public:
        Point centre;
        ALLEGRO_COLOR color;
        Vector speed;
        bool dead;

        Enemy(Point p, ALLEGRO_COLOR c, Vector s) : centre(p), color(c), speed(s), dead(false){}

        ~Enemy(){}

        void update(double dt) {}


    private:
};

#endif