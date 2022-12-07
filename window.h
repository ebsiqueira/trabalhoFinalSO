#ifndef window_h
#define window_h

#include <allegro5/allegro.h>
#include <memory>
#include <string>

#include "Sprite.h"
#include "Vector.h"
#include "Action.h"

class Window{

    public:
        Window(int x, int y, int fps);
        ~Window();

        void init();
        void run();
        void draw();
        void update(double dt);


    private:

}