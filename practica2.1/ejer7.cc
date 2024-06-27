#include <unistd.h>
#include <iostream>
using namespace std;


int main(){
    long link_max;
    if((link_max = pathconf(".", _PC_LINK_MAX))== -1){
        perror ("pathconf LINK_MAX");
        return 1;
    }
    cout << "Longitud maxima de los argumentos: " << link_max << '\n';
    return 0;
}