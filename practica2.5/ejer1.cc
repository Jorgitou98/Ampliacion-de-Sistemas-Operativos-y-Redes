#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
using namespace std;

// Basarse en el ejemplo que hay al final de "man getaddrinfo"
int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Hace falta un argumento con el host\n";
        return 1;
    }
    struct addrinfo hints;
    struct addrinfo *result;

    // Ponemos la estructura de hints a 0 para no fijar nada
    // y devuelva todas las posibles opciones para ese host
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC; // No especificamos si es ipv4 o ipv6

    // Llamamos a getaddrinfo con el host argv[1] y recibimos el resultado en result
    int s = getaddrinfo(argv[1], NULL, &hints, &result); 
    if (s != 0){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }
    // Recorremos la lista enlazada de posibilidades
    for (struct addrinfo * iter = result; iter != NULL; iter = iter->ai_next){
        char host_name[NI_MAXHOST];
        // Llamamos a getnameinfo para obtener el nombre a partir de la dirección
        if (getnameinfo(iter->ai_addr, iter->ai_addrlen, host_name, NI_MAXHOST, NULL, 0, NI_NUMERICHOST) != 0){
            perror("getnameinfo");
            return 1;
        }
        // Mostramos la información
        cout << host_name << '\t' << iter->ai_family << '\t' << iter->ai_socktype << '\n';
    }

    return 0;
}