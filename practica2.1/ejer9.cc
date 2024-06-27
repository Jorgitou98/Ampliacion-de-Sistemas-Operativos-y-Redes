#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
using namespace std;

int main(){
    uid_t real_uid, efective_uid;
    if((real_uid = getuid()) == -1){
        perror("getuid");
        return 1;
    }
    if((efective_uid = geteuid()) == -1){
        perror("geteuid");
        return 1;
    }
    cout << "ID real del usuario: " << real_uid << '\n';
    cout << "ID efectivo del usuario: " << efective_uid << '\n';
    return 0;
}