#include <iostream>
#include <unistd.h>
#include <signal.h>
using namespace std;

volatile char* exec_name;


void handler_sigalarm(int val){
    // Borro el ejecutable
    unlink((const char*)exec_name);
    cout << "Ejecutable " << (const char*)exec_name  << " borrado!\n";
}

void handler_sigusr1(int val){
    cout << "Programa salvado!\n";
}


int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Hace falta al menos un argumento con los segundos antes de borrar el ejecutable\n";
        return 1;
    }
    exec_name = argv[0];

    struct sigaction action_sigalarm;
    action_sigalarm.sa_handler = handler_sigalarm;
    sigemptyset(&action_sigalarm.sa_mask);
    action_sigalarm.sa_flags = 0;

    struct sigaction action_sigusr1;
    action_sigusr1.sa_handler = handler_sigusr1;
    sigemptyset(&action_sigusr1.sa_mask);
    action_sigusr1.sa_flags = 0;

    if (sigaction(SIGALRM, &action_sigalarm, NULL) == -1 || sigaction(SIGUSR1, &action_sigusr1, NULL) == -1){
        perror("sigaction");
        return 1;
    }
    
    int secs = atoi(argv[1]);
    alarm(secs);
    cout << "Establecida alarma que borrara el programa en " << secs << " segundos\n";

    // Suspendemos el proceso hasta que se reciba una de las señales (usando sigsuspend)
    // Ponemos todas a 1 (bloqueadas que no nos hacen salir de la suspensión), salvo estas dos a 0 en la mask
    sigset_t signal_set;
    if (sigfillset(&signal_set)){
        perror("sigfillset");
        return 1;
    }
    if (sigdelset(&signal_set, SIGALRM) == -1 || sigdelset(&signal_set, SIGUSR1) == -1){
        perror("sigdelset");
        return 1;
    }
    sigsuspend(&signal_set);

    return 0;
}