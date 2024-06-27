#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
using namespace std;


int main(int argc, char** argv){
    if (argc < 2){
        cout << "Hace falta el nombre del fichero como argumento\n";
        return 1;
    }
    int fd = open(argv[1],O_RDWR);
    if (lockf(fd, F_TLOCK, 0)== -1){
        perror("lockf");
        close(fd);
        return 1;
    }
    time_t t = time(NULL);
    cout << "Bloqueado a las " << ctime(&t) << '\n';
    sleep(10);
    if (lockf(fd, F_ULOCK, 0)== -1){
        perror("lockf (unlock)");
        close(fd);
        return 1;
    }
    cout << "Desbloqueado\n";
    close(fd);
    sleep(10);
    return 0;
}