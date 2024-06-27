#include <time.h>
#include <iostream>
using namespace std;

int main(){

    time_t time_epoch;
    if ((time_epoch = time(NULL)) == -1){
        perror("time");
        return 1;
    }
    cout << "Tiempo desde epoch " << time_epoch << " segundos\n"; 

    return 0;
}