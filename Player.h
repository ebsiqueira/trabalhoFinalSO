#ifndef player_H
#define player_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdexcept>
#include <memory>

#include "Projectile.h"
#include "Action.h"
#include "Objeto.h"

class Player : public Objeto{

    public:

        Point centre;
        ALLEGRO_COLOR color;
        Vector speed;
        int life; 
        int row;
        int col;
        bool dead;

        Player(Point center, ALLEGRO_COLOR color);
        ~Player();

        void update(double);

        void drawShip(std::shared_ptr<Sprite> sprite, int flags);

        act::action input(ALLEGRO_KEYBOARD_STATE&);
    
    private:
        void selectShipAnimation();
        void checkBoundary();
        void drawRemainingLife();

};

#endif