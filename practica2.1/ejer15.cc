#include <time.h>
#include <locale.h>
#include <iostream>
using namespace std;

#define BUF_SIZE 80

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
    if(setlocale(LC_ALL, "") == NULL);
    size_t size;
    char s[80];
    if ((size = strftime(s, BUF_SIZE, "%A, %d de %B de %Y, %R", local_time)) == 0){
        cout << "Error, necesito un buffer mas grande\n";
        return 1;
    }
    cout << s << '\n';




    return 0;
}