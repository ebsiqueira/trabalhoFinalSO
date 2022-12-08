/**
 * @file main.cc
 * @brief
 * 
 */


#include "Engine.h"
#include "main_class.h"
#include "system.h"
#include <allegro5/allegro.h>
#include <ctime>
#include <cstdlib>
#include <string>
#include <iostream>

__USING_API

int main() {
   srand(time(0));
   
   System::init(&Main::run);

   return 0;

}

