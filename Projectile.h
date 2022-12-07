#ifndef projectile_H
#define projectile_H

#include <allegro5/allegro.h>

#include "Sprite.h"
#include "Vector.h"
#include "Objeto.h"

class Projectile : public Objeto {

    public:

        Point centre;
        Vector speed;
        int life;
        int row;
        int col;
        bool dead;
        ALLEGRO_COLOR color;


        Projectile(Point p, ALLEGRO_COLOR c, Vector s) : centre(p), color(c), speed(s)
        {
            life = true;
        }

        ~Projectile() {};

        void draw() { }
        void update(double dt){}

    private:

};

#endif