#include <unistd.h>
#include <iostream>
using namespace std;


int main(){
    long long_max;
    if((long_max = sysconf(_SC_ARG_MAX))== -1){
        perror ("sysconf ARG_MAX");
        return 1;
    }
    cout << "Longitud maxima de los argumentos: " << long_max << '\n';
    return 0;
}