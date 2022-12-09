#ifndef creep_h
#define creep_h

#include "Enemy.h"
#include <allegro5/allegro.h>
#include <memory>
#include <string>
#include <list>

#include "Timer.h"
#include "Sprite.h"

class Creep : public Enemy {

    public:
    Creep(Point p, ALLEGRO_COLOR c, Vector s);
    ~Creep();
   
   void update(double dt);
   void load_assets();
   void deathAnim(std::shared_ptr<Sprite>);
   void hit();
   void draw(std::shared_ptr<Sprite> ship, std::shared_ptr<Sprite> death);

   void setFire(bool f);	
   bool getFire();
   bool getDead();  
   Vector getProjSpeed();


    private:
    std::shared_ptr<Timer> fireDelay;   
    Vector projSpeed;
    int fireSpeed;
    int lives;
    int dAnim;   
    bool dAnim_complete;
    bool fire;

};

#endif