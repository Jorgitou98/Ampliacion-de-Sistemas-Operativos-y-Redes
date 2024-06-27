#include <sys/time.h>
#include <iostream>
using namespace std;

int main(){
    timeval tv_before;
    if (gettimeofday(&tv_before, NULL) == -1){
        perror("gettimeofday before");
        return 1;
    }
    int variable = 0;
    for (int i = 0; i < 1000000; ++i) variable++;

    timeval tv_after;
    if (gettimeofday(&tv_after, NULL) == -1){
        perror("gettimeofday after");
        return 1;
    }

    cout << "Microsegundos transcurridos: " << (tv_after.tv_sec - tv_before.tv_sec) * 1e6 + (tv_after.tv_usec - tv_before.tv_usec) << '\n';


    return 0;
}