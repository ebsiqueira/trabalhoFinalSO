#include "Player.h"
#include "Point.h"
#include "Engine.h"
#include <iostream>

Player::Player(Point v, ALLEGRO_COLOR c) : centre(v), color(c), speed(Vector(0, 0)), life(3), row(0), col(0), dead(false) {}

Player::~Player() {}

act::action Player::input(ALLEGRO_KEYBOARD_STATE& kb) {
  if (al_key_down(&kb, ALLEGRO_KEY_UP)) {
    speed.y -= 250;
  }
  if (al_key_down(&kb, ALLEGRO_KEY_RIGHT)) {
    speed.x += 250;
  }
  if (al_key_down(&kb, ALLEGRO_KEY_DOWN)) {
    speed.y += 250;
  }
  if (al_key_down(&kb, ALLEGRO_KEY_LEFT)) {
    speed.x -= 250;
  }
  if (al_key_down(&kb, ALLEGRO_KEY_SPACE)) {
    return act::action::FIRE_PRIMARY;
  }
  if (al_key_down(&kb, ALLEGRO_KEY_1)) {
    return act::action::FIRE_SECONDARY;
  }
  if (al_key_down(&kb, ALLEGRO_KEY_ESCAPE)) {
    return act::action::QUIT_GAME;
  }

  return act::action::NO_ACTION;
}

void Player::drawShip(std::shared_ptr<Sprite> sprite, int flags){
    sprite->draw_region(row, col, 47.0, 40.0, centre, flags);
    drawRemainingLife();
}

void Player::update(double dt) {
   centre = centre + speed * dt;
   selectShipAnimation(); // must happen before we reset our speed
   speed = Vector(0, 0); // reset our speed
   checkBoundary();
}

void Player::selectShipAnimation() {
   if (speed.x > 0) {
      col = 1;
      if (speed.y > 0) row = 2;
      else if (speed.y < 0) row = 0;
      else row = 1;
   } else {
      col = 0;
      if (speed.y > 0) row = 2;
      else if (speed.y < 0) row = 0;
      else row = 1;
   }
}


void Player::checkBoundary() {
   if (centre.x > 800 - 16)
      centre.x = 800 - 16;
   else if (centre.x < 16)
      centre.x = 16;
   if (centre.y > 600 - 16)
      centre.y = 600 - 16;
   else if (centre.y < 16)
      centre.y = 16;
}

void Player::drawRemainingLife() {   
   al_draw_line(centre.x - 16*2, centre.y + 16*2,
		(centre.x - 16*2) + (life / 3) * (16*4),
		centre.y + 16*2,
		al_map_rgb(255 * (1.0 - life / 3),
			   200 * (life / 3),
			   0), 5);
}