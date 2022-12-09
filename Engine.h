/**
 * @file Engine.h
 * @brief
 *
 * @author
 * @bug
 */
#ifndef ENGINE_H
#define ENGINE_H

#include <allegro5/allegro.h>
#include <memory>
#include <string>
#include <list>

#include "Sprite.h"
#include "Vector.h"
#include "Player.h"
#include "Action.h"
#include "Timer.h"
#include "Laser.h"
#include "Missile.h"
#include "Enemy.h"
#include "Creep.h"

// forward declarations
class Menu;
class Root;

class Engine {
      
  public:
   Engine(int w, int h, int fps);
   ~Engine();
   bool _finish;
   float prevTime;
   
   void init();
   void run(float& prevTime);
   void draw();
   void spawn();
   void update(double dt);
   void input(ALLEGRO_KEYBOARD_STATE& kb);
   void addPlayerLaserSingleShot();
   void addPlayerMissileSingleShot();
   void addCreep(const Point& cen, const ALLEGRO_COLOR& col, const Vector& spd);
   void addLaser(const Point&, const ALLEGRO_COLOR&, const Vector&);
   void addMissile(const Point&, const ALLEGRO_COLOR&, const Vector&);
   bool doColorsMatch(const ALLEGRO_COLOR& a, const ALLEGRO_COLOR& b);

   void gameLoop(float& prevTime);
   void drawBackground();

   inline int getWidth() const {
      return _displayWidth;
   }
   inline int getHeight() const {
      return _displayHeight;
   }
   inline int getFps() const {
      return _fps;
   }

  private:
   void loadSprites();
   //Checks data of the spaceship
   void checkBoundary();
   void selectShipAnimation();
   void drawProjectiles();
   void drawMissiles();
   void drawEnemy();
   void updateBackgroud(double dt);
   void updateLaser(double dt);
   void updateMissile(double dt);
   void updateEnemy(double dt);

   std::shared_ptr<Player> player;
   std::shared_ptr<Sprite> spaceShip;
   std::shared_ptr<Sprite> enemyShip;
   std::shared_ptr<Sprite> enemyDeath;
   std::shared_ptr<Timer> playerWeapon1;
   std::shared_ptr<Timer> playerWeapon2;
   std::list< std::shared_ptr<Laser> > proj;
   std::list< std::shared_ptr<Missile> > proj2;
   std::list< std::shared_ptr<Creep> > enem;

   //Background
   Point bgMid;/**<point used by the background to draw from */
   Point fgMid;
   Point fg2Mid;
   Vector bgSpeed;/**<background movement speed */
   Vector fgSpeed;
   std::shared_ptr<Sprite> bg;/**<shared pointer to background animation */
   std::shared_ptr<Sprite> fg;

   // general game variables
   int _displayWidth;
   int _displayHeight;
   int _fps;
   // allegro objects
   ALLEGRO_TIMER *_timer;
   ALLEGRO_EVENT_QUEUE *_eventQueue;
   ALLEGRO_DISPLAY *_display;

   
   
};





#endif
