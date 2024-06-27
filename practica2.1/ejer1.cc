#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;

int main(){
    if (setuid(0) == -1){
        perror("setuid");
        return 1;
    }
    return 0;
}