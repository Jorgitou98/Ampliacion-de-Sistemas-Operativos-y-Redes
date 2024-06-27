#include <sys/utsname.h>
#include <stdio.h>
#include <iostream>
using namespace std;

int main(){
    utsname buf;
    if(uname(&buf) == -1){
        perror("uname");
        return 1;
    }
    cout << "Operating system: " << buf.sysname << '\n';
    cout << "Node name: " << buf.nodename << '\n';


    return 0;
}