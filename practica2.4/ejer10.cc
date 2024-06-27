#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>
using namespace std;

int open_or_create_fifo(char* fifo_name){
    int fd = open(fifo_name, O_NONBLOCK | O_RDONLY);
    if (fd == -1){
        // Si la tuberia no existe, la creo (no lo exige el enunciado, pero lo prefiero)
        if(errno == ENOENT){
            if(mkfifo(fifo_name, 0666)){
                perror("mkfifo");
                exit(1);
            }
            fd = open("tuberia", O_NONBLOCK | O_RDONLY);
        }
        else{
            perror("open");
            exit(1);
        }
    }
    return fd;
}

void fifo_read_logic(int fd, char* fifo_name, fd_set* set){
    char buffer[256];
    // Si han escrito en la tuberia
    if (FD_ISSET(fd, set)){
        int read_len = read(fd, buffer, 256);
        // Cuando el escritor termina y cierra la tubería, read devolverá 0
        while(read_len != 0){
            if (read_len == -1){
                perror("read fd1");
                exit(1);
            }
            // Colocamos el '\0' al final de lo leido
            buffer[read_len] = '\0';
            cout << "Leido de " << fifo_name << ":\n";
            cout << buffer << '\n';
            read_len = read(fd, buffer, 256);
        }
        // Cuando el escritor termina y obtenemos un 0 hay que cerrar y abrir la tuberia
        close(fd);
        fd = open_or_create_fifo(fifo_name);
    }
}


int main(){
    int fd1 = open_or_create_fifo((char*)"tuberia");
    int fd2 = open_or_create_fifo((char*)"tuberia2");
    
    // Añado las tuberias a los ficheros que quiero monitorizar
    fd_set set;

    // Vamos a hacer que haga lo mismo infinitamente (matar con ctr+C)
    while(true){
        FD_ZERO(&set);
        FD_SET(fd1, &set);
        FD_SET(fd2, &set);

        // Monitorizo indefinidamente hasta que escriban en alguna (el primer argumento es el máximo +1 de los fds)
        if (select((fd1 > fd2 ? fd1 : fd2) + 1, &set, NULL, NULL, NULL) == -1){
            perror ("select");
            return 1;
        }
        // Hago la lógica con cada fd
        fifo_read_logic(fd1, (char*) "tuberia", &set);
        fifo_read_logic(fd2, (char*) "tuberia2", &set);
    }




}