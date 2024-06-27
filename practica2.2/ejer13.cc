#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;


int main(int argc, char** argv){
    if (argc < 2){
        cout << "Hace falta el nombre del fichero como argumento\n";
        return 1;
    }
    int fd = open(argv[1],O_RDWR);
    if (fd == -1){
        perror("open");
        return 1;
    }
    if (dup2(fd, STDERR_FILENO) == -1){
        perror("dup2");
        return 1;
    }
    if (dup2(fd, STDOUT_FILENO) == -1){
        perror("dup2");
        return 1;
    }
    if (close(fd) == -1){
        perror("close");
        return 1;
    }
    cout << "Estandar redireccionada a " << argv[1] << '\n';
    cerr << "Error redireccionada a " << argv[1] << '\n';


    return 0;
}