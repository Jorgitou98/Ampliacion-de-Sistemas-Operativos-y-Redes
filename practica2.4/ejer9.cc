#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
using namespace std;

int main(int argc, char** argv){

    if (argc < 2){
        cerr << "Proporciona un argumento al programa que escribir en la tuberia\n";
    }
    int fd = open("tuberia", O_WRONLY);
    if (fd == -1){
        // Si la tuberia no existe, la creo (no lo exige el enunciado, pero lo prefiero)
        if(errno == ENOENT){
            if(mkfifo("tuberia", 0666)){
                perror("mkfifo");
                return 1;
            }
            fd = open("tuberia", O_WRONLY);
        }
        else{
            perror("open");
            return 1;
        }
    }

    // Escribo en la tuberia el argv[1] del programa
    if (write(fd, argv[1], strlen(argv[1])) == -1){
        perror("write");
        return 1;
    }

    close(fd);


    return 0;
}