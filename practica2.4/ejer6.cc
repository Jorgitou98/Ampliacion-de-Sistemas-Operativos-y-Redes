#include <iostream>
#include <unistd.h>
#include <sys/types.h>
using namespace std;


int main(int argc, char** argv){
    if (argc < 5){
        cerr << "Faltan argymentos, espero: ./programa cmd1 arg1 cmd1 arg2\n";
        return 1;
    }
    int pipefd[2];
    if(pipe(pipefd)== -1){
        perror("pipe");
        return 1;
    }
    pid_t pid = fork();
    if(pid == -1){
        perror("fork");
        return 1;
    }
    // En el hijo
    else if(pid == 0){
        // Cerramos la escritura (solo usamos lectura)
        close(pipefd[1]);
        // Redireccionamos la entrada estandar a la lectura de la tuberia
        if (dup2(pipefd[0], STDIN_FILENO) == -1){
            perror("dup2 child");
            return 1;
        }
        // Cerramos la lectura (ya hemos copiado el descriptor)
        close(pipefd[0]);
        // Ejecutamos el comando del padre (el que lee)
        execlp(argv[3], argv[3], argv[4], NULL);
        perror("execlp child");
    }
    // En el padre
    else{
        // Cerramos la lectura (el padre escribe)
        close(pipefd[0]);
        // Redireccionamos la salida estándar a la escritura en la tuberia
        if (dup2(pipefd[1], STDOUT_FILENO) == -1){
            perror("dup2 parent");
            return 1;
        }
        // Cerramos la escritura (ya hemos copiado el descriptor)
        close(pipefd[1]);
        // Ejecutamos el comando del hijo (el que escribe)
        execlp(argv[1], argv[1], argv[2], NULL);
        perror("execlp child");
    }


    return 0;
}