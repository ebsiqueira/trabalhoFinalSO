#ifndef missile_H
#define missile_H

#include <allegro5/allegro.h>

#include <memory>
#include <vector>

#include "Point.h"
#include "Vector.h"
#include "Projectile.h"
#include "Sprite.h"

class Missile : public Projectile{

    public:
        Missile(Point p, ALLEGRO_COLOR c, Vector s);
        ~Missile();

        void load_assets();
        void draw();
        void update(double dt);
   
  private:
        std::vector< std::shared_ptr<Sprite> > mvec;
        int mAnim;   
        bool in_bound();
        double angle;
};

#endif