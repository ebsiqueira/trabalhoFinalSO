#ifndef objeto_h
#define objeto_h

#include "Vector.h"
#include "Point.h"

class Objeto {
    
    public:
        Point centre;
        Vector speed;
        int life;
        int row;
        int col;
        bool dead;

    private:
};

#endif