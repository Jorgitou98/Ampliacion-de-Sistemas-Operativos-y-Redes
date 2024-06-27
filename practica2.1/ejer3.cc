#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
using namespace std;

int main(){
    for(int i = 1; i <= 255; ++i){
        cout << i << ": " << strerror(i) << '\n';
    }
    return 0;
}