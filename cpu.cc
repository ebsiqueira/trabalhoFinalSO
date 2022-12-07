#include "cpu.h"
#include <iostream>


__BEGIN_API

void CPU::Context::save()
{
    db<CPU>(TRC)<<"CPU::Context::save()\n";
    getcontext(&_context);
}

void CPU::Context::load()
{
    db<CPU>(TRC)<<"CPU::Context::load()\n";
    setcontext(&_context);
}

CPU::Context::~Context()
{
    db<CPU>(TRC)<<"CPU::Context::~Context()\n";
    if(_stack) {
        delete _stack;
    }
}

int CPU::switch_context(Context *from, Context *to)
{
    try {
        db<CPU>(TRC)<<"CPU::Context::switch_context()\n";
        swapcontext(&from->_context, &to->_context);
    } catch (...) {
        return -1;
    }
    return 0;
}

int CPU::finc(volatile int & x) {
    __asm__ __volatile__ ("lock xadd %1, %0;"
        : "+m" (x)
        : "r" (one));
}
int CPU::fdec(volatile int & x) {
    __asm__ __volatile__ ("lock subl %1, %0;"
        : "+m" (x)
        : "r" (one));
}

__END_API
