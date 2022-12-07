#ifndef mine_h
#define mine_h

#include <allegro5/allegro.h>
#include "Objeto.h"
#include "Vector.h"
#include "Timer.h"

class Mine : public Objeto {

    public:
        Mine(int x, int y, Vector speed, int life, int size, bool dead, Timer timer);
        ~Mine();

    private:

}