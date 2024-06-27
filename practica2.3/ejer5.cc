#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
using namespace std;

#define BUF_SIZE 256

int main(){
    cout << "pid: " << getpid() << '\n';
    cout << "ppid: " << getppid() << '\n';
    cout << "gid: " << getgid() << '\n';
    pid_t sid = getsid(0);
    if (sid == -1){
        perror("getsid");
        return 1;
    }
    cout << "sid: " << sid << '\n';
    rlimit lim_struct;
    if (getrlimit(RLIMIT_NOFILE, &lim_struct) == -1){
        perror("getrlimit");
        return 1;
    }
    cout << "Maximo ficheros: " << lim_struct.rlim_max << '\n';

    char buf[BUF_SIZE];
    if (getcwd(buf, BUF_SIZE) == NULL){
        perror("getcwd");
        return 1;
    }
    cout << "Directorio actual: " << buf << '\n';

    return 0;
}