#include "semaphore.h"
#include <stdio.h>
#include "traits.h"
#include "system.h"
#include <iostream>

__BEGIN_API

Semaphore::~Semaphore(){
    wakeup_all();
}

void Semaphore::p(){
    db<Semaphore>(TRC) << "Semaphore p(): Thread id="<< Thread::running()->id() << " count:" << _locks << "\n";
    if (_locks == 0)
        sleep();
    else
        fdec(_locks);
}

void Semaphore::v(){
    db<Semaphore>(TRC) << "Semaphore v(): Thread id="<< Thread::running()->id() << " count:" << _locks << "\n";
    if (_locks == 0 && !_threads.empty())
        wakeup();
    else
        finc(_locks);
}

int Semaphore::finc(volatile int & x) {
    db<Semaphore>(TRC) << "Semaphore finc():\n";
    return CPU::finc(x);
}
int Semaphore::fdec(volatile int & x) {
    db<Semaphore>(TRC) << "Semaphore fdec():\n";
    return CPU::fdec(x);
}

void Semaphore::sleep() {
    db<Semaphore>(TRC) << "Semaphore sleep(): Thread id="<< Thread::running()->id() << " \n";
    _threads.insert(Thread::running()->link());
    Thread::running()->sleep();
}
void Semaphore::wakeup() {
    Thread* waking_thread = _threads.remove()->object();
    db<Semaphore>(TRC) << "Semaphore wakeup(): Thread id="<< waking_thread->id() << " \n";
    waking_thread->wakeup();
}
void Semaphore::wakeup_all() {
    db<Semaphore>(TRC) << "Semaphore wakeup_all()\n";
    while (!_threads.empty()) {
        _threads.remove()->object()->wakeup();
    }
} 

__END_API