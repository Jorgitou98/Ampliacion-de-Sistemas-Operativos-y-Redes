#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
using namespace std;

int main (int argc, char ** argv){
    if(argc < 2){
        cout << "Error, debe haber algún argumento\n";
        return 1;
    }
    int fd = open(argv[1], O_CREAT, 00645);
    if(fd == -1){
        perror("open");
        return 1;
    }
    if (close(fd) == -1){
        perror("close");
        return 1;
    }




    return 0;
}