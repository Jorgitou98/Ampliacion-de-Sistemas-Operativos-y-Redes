#include <iostream>
#include <signal.h>
#include <unistd.h>
using namespace std;


int main(){
    sigset_t set_block;
    if (sigemptyset(&set_block) == -1){
        perror("sigemptyset");
        return 1;
    }
    if (sigaddset(&set_block, SIGINT) == -1){
        perror("sigaddset SIGINT");
        return 1;
    }
    if (sigaddset(&set_block, SIGTSTP) == -1){
        perror("sigaddset SIGTSTP");
        return 1;
    }
    char* sleep_env_val = getenv("SLEEP_SECS");
    if (sleep_env_val == NULL){
        perror("getenv");
        return 1;
    }
    // Bloqueo
    if (sigprocmask(SIG_BLOCK, &set_block, NULL) == -1){
        perror("sigprocmask block");
        return 1;
    }
    sleep(atoi(sleep_env_val));
    sigset_t set_pending;
    if (sigpending(&set_pending) == -1){
        perror("sigpending");
    }
    int pending_sigint = sigismember(&set_pending, SIGINT);
    if(pending_sigint == -1){
        perror("sigismember SIGINT");
        return 1;
    }
    else if (pending_sigint == 1){
        cout << "Se recibio la señal SIGINT\n";
    }

    int pending_sigtstp = sigismember(&set_pending, SIGTSTP);
    if(pending_sigtstp == -1){
        perror("sigismember SIGTSTP");
        return 1;
    }
    else if (pending_sigtstp == 1){
        cout << "Se recibio la señal SIGTSTP\n";
        sigset_t set_unblock;
        if (sigemptyset(&set_unblock) == -1){
            perror("sigemptyset");
            return 1;
        }
        if (sigaddset(&set_unblock, SIGTSTP) == -1){
            perror("sigaddset SIGTSTP");
            return 1;
        }
        // Desbloqueo
        if (sigprocmask(SIG_UNBLOCK, &set_unblock, NULL) == -1){
            perror("sigprocmask unblock");
            return 1;
        }
    }
    // No se imprime. Si solo se recibio SIGTSTP (y no SIGINT tb) se puede reanudar con fg por terminal
    cout << "Mensaje final\n";

    return 0;
}