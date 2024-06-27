#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pwd.h>
using namespace std;

int main(){
    uid_t real_uid, efective_uid;
    passwd * real_user_info;
    passwd * efective_user_info;
    if((real_uid = getuid()) == -1){
        perror("getuid");
        return 1;
    }
    if((efective_uid = geteuid()) == -1){
        perror("geteuid");
        return 1;
    }
    if((real_user_info = getpwuid(real_uid)) == NULL){
        perror("real_user_info");
        return 1;
    }
    if((efective_user_info = getpwuid(real_uid)) == NULL){
        perror("efective_user_info");
        return 1;
    }
    cout << "ID real del usuario: " << real_uid << '\n';
    cout << "Nombre del usuario: " << real_user_info->pw_name << '\n';
    cout << "Home: " << real_user_info->pw_dir << '\n';
    cout << "Descripción: " << real_user_info->pw_gecos << '\n';
    cout << "ID efectivo del usuario: " << efective_uid << '\n';
    cout << "Nombre del usuario: " << efective_user_info->pw_name << '\n';
    cout << "Home: " << efective_user_info->pw_dir << '\n';
    cout << "Descripción: " << efective_user_info->pw_gecos << '\n';
    return 0;
}