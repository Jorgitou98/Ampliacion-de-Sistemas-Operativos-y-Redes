#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
using namespace std;

int main(int argc, char** argv){
    if(argc < 2){
        cout << "Error, debe haber algún argumento\n";
        return 1;
    }
    struct stat statbuf;
    if (stat(argv[1], &statbuf) == -1){
        perror("stat");
        return 1;
    }
    cout << "Dispositivo: " << major(statbuf.st_dev) << '/' << minor(statbuf.st_dev) << '\n';
    cout << "i-nodo: " << statbuf.st_ino << '\n';
    if(S_ISREG(statbuf.st_mode)) cout << "Es un fichero regular\n";
    else if (S_ISDIR(statbuf.st_mode)) cout << "Es un directorio\n";
    else if (S_ISLNK(statbuf.st_mode)) cout << "Es un enlace simbólico\n";
    cout << "Momento del ultimo acceso: " << ctime((const time_t *) &statbuf.st_atim) << '\n';

    return 0;
}
