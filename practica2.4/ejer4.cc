#include <iostream>
#include <signal.h>
using namespace std;

int sigint_recv = 0;
int sigtstp_recv = 0;

void handler_sigint(int val){
    cout << "SIGINT signal\n";
    sigint_recv++;
}

void handler_sigtstp(int val){
    cout << "SIGTSTP signal\n";
    sigtstp_recv++;
}


int main(){

    struct sigaction action_sigint;
    action_sigint.sa_handler = handler_sigint;
    action_sigint.sa_flags = 0;
    if (sigaction(SIGINT, &action_sigint, NULL) == -1){
        perror("sigaction SIGINT");
        return 1;
    }

    struct sigaction action_sigtstp;
    action_sigtstp.sa_handler = handler_sigtstp;
    action_sigtstp.sa_flags = 0;
    if (sigaction(SIGTSTP, &action_sigtstp, NULL) == -1){
        perror("sigaction SIGTSTP");
        return 1;
    }
    while(sigint_recv + sigtstp_recv < 10);
    cout << "10 señales recibidas\n";
    cout << sigint_recv << " señales SIGINT\n";
    cout << sigtstp_recv << " señales SIGTSTP\n";

    return 0;
}