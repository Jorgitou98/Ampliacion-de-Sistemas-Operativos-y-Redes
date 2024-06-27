#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

void open_and_dup2(char* file_name, int new_fd){
    int fd = open(file_name, O_CREAT | O_RDWR, 0777);
    if (fd == -1){
        perror("open");
        exit(1);
    }
    if (dup2(fd, new_fd) == -1){
        perror("dup2");
        exit(1);
    }
}


int main(int argc, char** argv){

    if (argc < 2){
        cerr << "Hace falta un argumento con el programa a ejecutar\n";
        return 1;
    }
    pid_t pid = fork();
    // Si error
    if (pid == -1){
        perror("fork");
        return 1;
    }
    // Si es el hijo
    else if (pid == 0){
        pid_t sesion = setsid();
        if(sesion == -1){
            perror("setsid");
            exit(1);
        }
        open_and_dup2((char*) "/tmp/daemon.out", STDOUT_FILENO);
        open_and_dup2((char*) "/tmp/daemon.err", STDERR_FILENO);
        open_and_dup2((char*) "/dev/null", STDIN_FILENO);
        if (execvp(argv[1], argv + 1) == -1){
            perror("execvp");
            return 1;
        }
    }
    // Si es el padre
    else {
        // No hacemos nada
    }


    return 0;
}