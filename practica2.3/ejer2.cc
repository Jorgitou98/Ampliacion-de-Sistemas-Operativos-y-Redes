#include <iostream>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>
using namespace std;


int main(){
    int policy = sched_getscheduler(0);
    if (policy == -1){
        perror("sched_getscheduler");
        return 1;
    }
    switch(policy){
        case SCHED_FIFO:
            cout << "Política FIFO\n";
            break;
        case SCHED_RR:
            cout << "Política round-robin\n";
            break;
        case SCHED_IDLE:
            cout << "Política para procesos de background de baja prioridad\n";
            break;
        case SCHED_BATCH:
            cout << "Política para ejecución en batch de procesos\n";
            break;
        case SCHED_OTHER:
            cout << "Round-robin estándar no orientada a tiempo real\n";
            break;
        default:
            cout << "Política no identificada\n";
    }
    // ¡Esta función no está en la chuleta para el examen y getpriority() no hace lo que queremos
    struct sched_param param;
    if (sched_getparam(0, &param) == -1){
        perror("sched_getparam");
        return 1;
    }
    cout << "Prioridad: " << param.sched_priority << '\n';

    int min_priority = sched_get_priority_min(policy);
    if (min_priority == -1){
        perror("sched_get_priority_min");
        return 1;
    }

    int max_priority = sched_get_priority_max(policy);
    if (max_priority == -1){
        perror("sched_get_priority_max");
        return 1;
    }

    cout << "Prioridad mínima: " << min_priority << ". Prioridad máxima: " << max_priority << '\n';


    return 0;
}