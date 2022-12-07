#include "thread.h"
#include "cpu.h"
#include "traits.h"
#include "debug.h"
#include <iostream>

__BEGIN_API

// Inicialização do ponteiro para a thread em execução
Thread* Thread::_running = nullptr;
// Contador em 0 (unsigned para evitar contagem negativa)
unsigned int Thread::_threadCounter = 0;
//Thread principal
Thread Thread::_main = Thread();
//Contexto principal
CPU::Context Thread::_main_context;
Thread Thread::_dispatcher = Thread();
Thread::Ready_Queue Thread::_ready;
Ordered_List<Thread> Thread::_suspended;

// Cria thread main e dispatcher
void Thread::init(void (*main)(void *)){
    new (&_main_context) CPU::Context();
    new (&_main) Thread(main,(void *) "main" );
    new (&_dispatcher) Thread((void (*) (void *)) &Thread::dispatcher, (void *) NULL);
    _ready.insert(&_dispatcher._link);
    _running = &_main;
    _main._state = RUNNING;
    _main._context->load();
}

// Tenta trocar o contexto e definir a próxima thread como thread em execução
int Thread::switch_context(Thread * prev, Thread * next) {
    
    // importante verificar se são diferentes (eu acho)
    if (prev != next) {
        _running = next;
        next->_state = RUNNING;
        db<Thread>(TRC)<<"Thread::switch_context()" << prev->id() << " : " << next->id() << "\n";
        return CPU::switch_context(prev->_context, next->_context);
    }
    return 0;
}

// Sai da thread (decremento no contador ficou no destrutor)
void Thread::thread_exit(int exit_code){
    db<Thread>(TRC) << "Thread::thread_exit(exit_code=" << exit_code << ", id=" << _id << ")\n";
    _exit_code = exit_code;
    _state = FINISHING;
    if (_join) {
        _join->resume();
        _join = 0;
    }
    Thread::yield();
}

void Thread::dispatcher() {
    db<Thread>(TRC) << "Thread::dispatcher()" << "\n";
    while(!_ready.empty() && _ready.head()->object() != &_dispatcher) {
        Thread* next = Thread::_ready.remove()->object();
        db<Thread>(TRC) << "Dispatcher: running=" << Thread::_running->_id << " next="<< next->_id <<"\n";
        _dispatcher._state = READY;
        _ready.insert(&_dispatcher._link);
    
        switch_context(&Thread::_dispatcher, next);

        if (next->_state == FINISHING)
            _ready.remove(next);
    }
    _dispatcher._state = FINISHING;
    _ready.remove(&_dispatcher);
    switch_context(&_dispatcher, &_main);
}

Thread::~Thread(){
    db<Thread>(TRC) << "\n\n\nDELETANDO ESSA BOSTA: " << this->id() <<"\n\n\n";     
    _threadCounter--;
    _ready.remove(this);
    db<Thread>(TRC) << "ESTADO LISTA _READY: "<< _threadCounter << "\n";
    delete _context;
}

void Thread::yield() {
    
    Thread* prev = _running;
    Thread* next = _ready.remove()->object();
    db<Thread>(TRC) << "Thread id="<< prev->_id << " yield()" << "\n";

    if (prev != &_main && prev != &_dispatcher  && prev->_state != FINISHING) {
        prev->_state = READY;
        int realTime = std::chrono::duration_cast<std::chrono::microseconds>
            (std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        prev->_link.rank(realTime);
    }
    if (prev != &_main && prev->_state != SUSPENED){
        _ready.insert(&prev->_link);
    }

    switch_context(prev, next);
}

int Thread::join(){
    db<Thread>(TRC) << "Thread " << _running->_id << " chamou join na thread "<< this->_id << "\n";
    if (_state != FINISHING) {
        _join = _running;
        _running->suspend();
    }
    return _exit_code;
}

void Thread::resume(){
    db<Thread>(TRC) << "thread id ="<< this->_id << " resume\n";
    _suspended.remove(&_link);
    _state = READY;
    _ready.insert(&_link);
}

void Thread::suspend(){
    db<Thread>(TRC) << "thread id ="<< this->_id << " suspend\n";
    //_ready.remove(&_link);
    _state = SUSPENED;
    _suspended.insert(&_link);
    yield();
}

// Retorna id da thread
int Thread::id() {
    return _id;
}

void Thread::sleep(){
    db<Thread>(TRC) << "Thread() id="<< this->_id << " sleep()\n";
    Thread* next = _ready.remove()->object();
    _state = WAITING;
    switch_context(_running, next);
}

void Thread::wakeup(){
    db<Thread>(TRC) << "Thread() id="<< this->_id << " wakeup()\n";
    _state = READY;
    _ready.insert(&_link);
    yield();
}

__END_API