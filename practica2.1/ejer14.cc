#include <time.h>
#include <iostream>
using namespace std;


int main(){
    time_t time_epoch;
    tm * local_time;
    if ((time_epoch = time(NULL)) == -1){
        perror("time");
        return 1;
    }
    if ((local_time = localtime(&time_epoch))== NULL){
        perror("local_time");
        return 1;
    }
    cout << "Estamos en el año " << 1900 + local_time->tm_year << '\n';

    return 0;
}
