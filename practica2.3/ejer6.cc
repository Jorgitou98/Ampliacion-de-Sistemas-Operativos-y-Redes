#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
using namespace std;

#define BUF_SIZE 256

void muestraAtributos(){
    cout << "pid: " << getpid() << '\n';
    cout << "ppid: " << getppid() << '\n';
    cout << "gid: " << getgid() << '\n';
    pid_t sid = getsid(0);
    if (sid == -1){
        perror("getsid");
        exit(1);
    }
    cout << "sid: " << sid << '\n';
    rlimit lim_struct;
    if (getrlimit(RLIMIT_NOFILE, &lim_struct) == -1){
        perror("getrlimit");
        exit(1);
    }
    cout << "Maximo ficheros: " << lim_struct.rlim_max << '\n';

    char buf[BUF_SIZE];
    if (getcwd(buf, BUF_SIZE) == NULL){
        perror("getcwd");
        exit(1);
    }
    cout << "Directorio actual: " << buf << '\n';
}


int main(){
    pid_t pid = fork();
    // Si hubo un error
    if (pid == -1){
        perror("fork");
        return 1;
    }
    // Si se trata del hijo
    else if (pid == 0){
        pid_t sid = setsid();
        if (sid == -1){
            perror("setid");
            return 1;
        }
        if (chdir("/tmp") == -1){
            perror("chdir");
            return 1;
        }
        muestraAtributos();
        exit(0);
    }
    // Si se trata del padre
    else {
        sleep(3000);
    }


    return 0;
}