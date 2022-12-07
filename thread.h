#ifndef thread_h
#define thread_h

#include "cpu.h"
#include "traits.h"
#include "debug.h"
#include "list.h"
#include "thread.h"
#include <ctime> 
#include <chrono>

__BEGIN_API

class Thread
{
protected:
    typedef CPU::Context Context;
public:

    typedef Ordered_List<Thread> Ready_Queue;

    // Thread State
    enum State {
        RUNNING,
        READY,
        FINISHING,
        SUSPENED,
        WAITING
    };

    /*
     * Construtor vazio. Necessário para inicialização, mas sem importância para a execução das Threads.
     */ 
    Thread() { }

    /*
     * Cria uma Thread passando um ponteiro para a função a ser executada
     * e os parâmetros passados para a função, que podem variar.
     * Cria o contexto da Thread.
     * PS: devido ao template, este método deve ser implementado neste mesmo arquivo .h
     */ 
    template<typename ... Tn>
    Thread(void (* entry)(Tn ...), Tn ... an);

    /*
     * Retorna a Thread que está em execução.
     */ 
    static Thread * running() { return _running; }

    /*
     * Método para trocar o contexto entre duas thread, a anterior (prev)
     * e a próxima (next).
     * Deve encapsular a chamada para a troca de contexto realizada pela class CPU.
     * Valor de retorno é negativo se houve erro, ou zero.
     */ 
    static int switch_context(Thread * prev, Thread * next);

    /*
     * Termina a thread.
     * exit_code é o código de término devolvido pela tarefa (ignorar agora, vai ser usado mais tarde).
     * Quando a thread encerra, o controle deve retornar à main. 
     */  
    void thread_exit (int exit_code);

    /*
     * Retorna o ID da thread.
     */ 
    int id();

    /*
     * NOVO MÉTODO DESTE TRABALHO.
     * Daspachante (disptacher) de threads. 
     * Executa enquanto houverem threads do usuário.
     * Chama o escalonador para definir a próxima tarefa a ser executada.
     */
    static void dispatcher(); 

    /*
     * NOVO MÉTODO DESTE TRABALHO.
     * Realiza a inicialização da class Thread.
     * Cria as Threads main e dispatcher.
     */ 
    static void init(void (*main)(void *));


    /*
     * Devolve o processador para a thread dispatcher que irá escolher outra thread pronta
     * para ser executada.
     */
    static void yield(); 

    /*
     * Destrutor de uma thread. Realiza todo os procedimentos para manter a consistência da classe.
     */ 
    ~Thread();

    /*
     * Qualquer outro método que você achar necessário para a solução.
     */ 

    /*
     * Suspende a thread que chamou ate a outra terminar.
     */ 
    int join();

    /*
     * Da resume na thread.
     */ 
    void resume();

    /*
     * Para a thread.
     */ 
    void suspend();

    /*
     * Faz dormir.
     */ 
    void sleep();

    /*
     * Faz acordar.
     */ 
    void wakeup();

    /*
     * Retorna link.
     */ 
    Ready_Queue::Element* link() { return &_link; }

private:
    int _id;
    Context * volatile _context;
    static Thread * _running;
    static unsigned int _threadCounter;
    
    static Thread _main; 
    static CPU::Context _main_context;
    static Thread _dispatcher;
    static Ready_Queue _ready;
    Ready_Queue::Element _link;
    volatile State _state;
    static Ordered_List<Thread> _suspended;

    int _exit_code;

    Thread * _join;
};

template<typename ... Tn>
inline Thread::Thread(void (* entry)(Tn ...), Tn ... an) : /* inicialização de _link */
_link(this, (std::chrono::duration_cast<std::chrono::microseconds> (std::chrono::high_resolution_clock::now().time_since_epoch()).count()))
{
    // Inicializacoes
    
    _context = new CPU::Context(entry, an ...);
    _id = _threadCounter++;
    _join = nullptr;
    _state = READY;
    _exit_code = -1;
    db<Thread>(TRC) << "Thread nova criada n: " << _id << "\n";
    
    if(_id > 1) {
        _ready.insert(&_link);
    }
}

__END_API

#endif
