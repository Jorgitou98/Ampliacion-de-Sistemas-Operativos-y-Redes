#include <iostream>
//#include <stdlib.h>
#include <unistd.h>
using namespace std;


int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Debes pasar un argumento con el comando\n";
        return 1;
    }
    // Versión 1
    /*int err = system(argv[1]);
    if (err == -1){
        perror("system");
        return 1;
    }
    else if (err == 127){
        cerr << "Shell could not b executed in child\n";
        return 1;
    }
    else{
        cerr << "Hubo otro error\n";
        return 1;
    }*/
    
    // Versión 2
    //char* exec_args[] = {argv[1], NULL};
    if (execvp(argv[1], argv+1) == -1){
        perror("execvp");
        return 1;
    }

    cout << "El comando termino de ejecutarse\n";

    return 0;
}