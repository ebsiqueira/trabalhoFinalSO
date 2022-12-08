#include "Engine.h"
#include "thread.h"

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdexcept>
#include <iostream>

#include "Timer.h"
#include "Player.h"
#include "Laser.h"
#include "Missile.h"

Engine::Engine(int w, int h, int fps) : _displayWidth(w), _displayHeight(h), 
					_fps(fps),
					_timer(NULL),
					_eventQueue(NULL),
               _finish(false)
{
   
}

Engine::~Engine() {
   if (_timer != NULL) al_destroy_timer(_timer);
   if (_eventQueue != NULL) al_destroy_event_queue(_eventQueue);
   if (_display != NULL) al_destroy_display(_display);

   bg.reset();
   spaceShip.reset();
}


// initialize Allegro, the _display window, the addons, the timers, and event 
// sources
void Engine::init() {
   // initialize allegro
   al_init();
   // create the display
   if ((_display = al_create_display(_displayWidth, _displayHeight)) == NULL) {
      std::cout << "Cannot initialize the display\n";
      exit(1); 
   }   
   // initialize addons
   al_init_primitives_addon();
   al_init_font_addon();
   al_init_ttf_addon();
   al_init_image_addon();
   // initialize our timers
   if ((_timer = al_create_timer(1.0 / _fps)) == NULL) {
      std::cout << "error, could not create timer\n";
      exit(1);
   }
   if ((_eventQueue = al_create_event_queue()) == NULL) {
      std::cout << "error, could not create event queue\n";
      exit(1);
   }
   // register our allegro _eventQueue
   al_register_event_source(_eventQueue, al_get_display_event_source(_display)); 
   al_register_event_source(_eventQueue, al_get_timer_event_source(_timer));
   al_start_timer(_timer);
   // install keyboard
   playerWeapon1 = std::make_shared<Timer> (_fps);
   playerWeapon1->create();
   playerWeapon1->startTimer();
   playerWeapon2 = std::make_shared<Timer> (_fps);
   playerWeapon2->create();
   playerWeapon2->startTimer();

   if (!al_install_keyboard()) {
      std::cerr << "Could not install keyboard\n";
   }
   
   // register keyboard
   al_register_event_source(_eventQueue, al_get_keyboard_event_source());

   loadSprites();
   std::cout<<"\ncarregou tudo\n";
}


// repeatedly call the state manager function until the _state is EXIT
void Engine::run(float& prevTime) {
   // TODO: criar thread que cuida de entrada de saida (funcao input)
   gameLoop(prevTime);
}

void Engine::gameLoop(float& prevTime) {
   ALLEGRO_EVENT event;
   ALLEGRO_KEYBOARD_STATE kb;
   bool redraw = true;
   float crtTime;
   
   // input
   al_get_keyboard_state(&kb);      
   act::action a = player->input(kb); //irá retornar uma tecla de ação. TODO: necessário transformar em Thread e fazer a ação
   input(kb);
   

   // get event
   al_wait_for_event(_eventQueue, &event);
   
   // _display closes
   if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE || a == act::action::QUIT_GAME) {
      _finish = true;
      return;
   }
   // timer
   if (event.type == ALLEGRO_EVENT_TIMER) {
      crtTime = al_current_time();
      player->update(crtTime - prevTime);
      updateBackgroud(crtTime - prevTime);
      updateLaser(crtTime - prevTime);
      updateMissile(crtTime - prevTime);

      prevTime = crtTime;
      redraw = true;
   }
   
   // render
   if (redraw && al_is_event_queue_empty(_eventQueue)) {
      redraw = false;   
      draw(); 
      al_flip_display();
   }
}

void Engine::updateMissile(double dt){
   auto iterator = proj2.begin();
      while(iterator != proj2.end()){
         (*iterator)->update(dt);
         if (!(*iterator)->life) {
            iterator = proj2.erase(iterator);
         }
         else{
            *iterator++;
         }
      }
}

void Engine::updateLaser(double dt){
   auto iterator = proj.begin();
      while(iterator != proj.end()){
         (*iterator)->update(dt);
         if (!(*iterator)->life) {
            iterator = proj.erase(iterator);
         }
         else{
            *iterator++;
         }
      }
}

void Engine::updateBackgroud(double dt) {
   bgMid = bgMid + bgSpeed * dt;   
   if (bgMid.x >= 800) {
      bgMid.x = 0;
   }
}

// draws for the game mode
void Engine::draw() {
   drawBackground();
   drawProjectiles();
   drawMissiles();
   player->drawShip(spaceShip, 0);
}

void Engine::drawBackground() {
   bg->draw_parallax_background(bgMid.x, 0);
   //al_draw_filled_rectangle(0,0,800,800,al_map_rgb_f(100,0,0));
}

void Engine::loadSprites()
{
   // Create Ship
   player = std::make_shared<Player> (Point(215, 245), al_map_rgb(0, 200, 0));

   // represents the middle of the image width-wise, and top height-wise
   bgMid = Point(0, 0);
   fgMid = Point(800, 0);
   fg2Mid= Point(300, 300);
   bgSpeed = Vector(50, 0);
   fgSpeed = Vector(-90, 0);

   // Go to resources directory
   ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
   al_append_path_component(path, "resources");
   al_change_directory(al_path_cstr(path, '/'));   
   // sprites
   spaceShip = std::make_shared<Sprite> ("Sprite2.png"); //espaçonave do usuário
   bg = std::make_shared<Sprite> ("BGstars.png"); //fundo da tela - background
   // delete path 
   al_destroy_path(path);

}

void Engine::addPlayerLaserSingleShot() {
   addLaser(player->centre + Point(-20, 0), player->color, Vector(500, 0));
   playerWeapon1->srsTimer();
}

void Engine::addPlayerMissileSingleShot() {
   addMissile(player->centre, player->color, Vector(500, 0));
   playerWeapon2->srsTimer();
}

void Engine::addLaser(const Point& cen, const ALLEGRO_COLOR& col, const Vector& spd) {
   proj.push_back(std::make_shared<Laser> (cen, col, spd));
}

void Engine::addMissile(const Point& cen, const ALLEGRO_COLOR& col, const Vector& spd) {
   std::shared_ptr<Missile> missileObject = std::make_shared<Missile> (cen, col, spd);
   missileObject->load_assets();
   proj2.push_back(missileObject);
}

void Engine::drawMissiles() {
   if (!proj2.empty()) {
      for (auto it = proj2.begin(); it != proj2.end(); ++it) { 
         if((*it)->life){
            (*it)->draw();
         }
      }
   }
}

void Engine::drawProjectiles() {
   if (!proj.empty()) {
      for (auto it = proj.begin(); it != proj.end(); ++it) { 
         if((*it)->life){
            (*it)->draw();
         }
      }
   }
}

void Engine::input(ALLEGRO_KEYBOARD_STATE& kb) {
   switch (player->input(kb)) {    
      case act::action::FIRE_PRIMARY:
         if (playerWeapon1->getCount() > 12) {
            addPlayerLaserSingleShot();
         }
         break;
         
      case act::action::FIRE_SECONDARY:
         if (playerWeapon2->getCount() > 20) {
            addPlayerMissileSingleShot();
         }
         break;
         
      default:
         break;
      }
}
