#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
using namespace std;

int main(){
    if (setuid(0) == -1){
        cout << "Número: " << errno << '\n';
        cout << "Error: " << strerror(errno) << '\n';
        return 1;
    }
    return 0;
}