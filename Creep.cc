#include "Creep.h"
#include "Point.h"
#include "Vector.h"
#include "Timer.h"
#include "Sprite.h"
#include <iostream>

Creep::Creep(Point cen, ALLEGRO_COLOR col, Vector spd) : Enemy(cen, col, spd),
							 projSpeed(Vector(-400, 0)),
							 fireSpeed(rand() % 50 + 30),
							 lives(1), dAnim(0),
							 dAnim_complete(false), fire(true)
{
   load_assets();
}

void Creep::load_assets() {
   fireDelay = std::make_shared<Timer> (60);   
   fireDelay->create();
   fireDelay->startTimer();
}

Creep::~Creep() {
   fireDelay.reset();
}

void Creep::hit() {
   lives = lives - 1;
   if (lives < 1)
      dead = true;
}

void Creep::draw(std::shared_ptr<Sprite> enemyShip, std::shared_ptr<Sprite> enemyDeath) {
   if (!dead) {
       std::cout << "\n\n\n 999";	
      enemyShip->draw_tinted(centre, color, 0);
   }
   else {
      if (dAnim < 5) deathAnim(enemyDeath);
      else dAnim_complete = true;
   }      
}

void Creep::deathAnim(std::shared_ptr<Sprite> enemyDeath) {
   enemyDeath->draw_death_anim(dAnim, centre, 0);
   dAnim++;
}

void Creep::update(double dt) {
   centre = centre + speed * dt;
				
   if (centre.x < 0) {
      dead = true;
      return;
   }
   
   // check y bound and adjust if out
   if (centre.y > 600 - 20 &&speed.y > 0)  
      speed.reflectY();
   if (centre.y < 0 - 20 && speed.y < 0)
      speed.reflectY();


   if (fireDelay->getCount() > fireSpeed) {
      fire = true;
      fireDelay->stopTimer();
      fireDelay->resetCount();
      fireDelay->startTimer();
   }
}

bool Creep::getDead() { return dead; }   
Vector Creep::getProjSpeed() { return projSpeed; }
bool Creep::getFire() { return fire; }
void Creep::setFire(bool f) { fire = f; }