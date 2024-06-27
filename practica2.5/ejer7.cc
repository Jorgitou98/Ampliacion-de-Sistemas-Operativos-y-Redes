// Copiar el cliente UDP de "man getaddr" y hacer los cambios que se indican en los comentarios
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
// Cambio 0: Incluir iostream y el espacio de nombres std para usar c++
#include <iostream>
using namespace std;


#define BUF_SIZE 500

int main(int argc, char *argv[]){
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    ssize_t nread;
    char buf[BUF_SIZE];

    if (argc < 3) {
        fprintf(stderr, "Usage: %s host port...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Obtain address(es) matching host/port */

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    // Cambio 1: Ponemos SOCK_STREAM (TCP) en lugar de SOCK_DGRAM (UDP)
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* Any protocol */

    s = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    /* getaddrinfo() returns a list of address structures.
        Try each address until we successfully connect(2).
        If socket(2) (or connect(2)) fails, we (close the socket
        and) try the next address. */

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype,
                    rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;                  /* Success */

        close(sfd);
    }

    if (rp == NULL) {               /* No address succeeded */
        fprintf(stderr, "Could not connect\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);           /* No longer needed */


    // Cambio 3: Leemos la entrada del usuario, la mandamos, recibimos la respuesta y la mostramos
    // Cuando recibamos el caracter 'Q' mandamos al servidor un mensaje vacío para que cierre la conexión y finalizamos
    while(1){
        // Leemos la entrada
        string linea;
        getline(cin, linea);
        // Si es !
        if (linea == "Q"){
            // Deshabilitamos el descriptor para lectura y escritura de forma que el servidor reciba 0 caracteres
            shutdown(sfd, SHUT_RDWR);
            // Salimos del bucle
            break;
        }
        // Enviamos lo leido
        if (send(sfd, linea.c_str(), linea.size(), 0) != (long int) linea.size()){
            perror("send");
            return 1;
        }
        // Recibimos la respuesta
        nread = recv(sfd, buf, BUF_SIZE, 0);
        if(nread == -1){
            perror("recv");
            return 1;
        }
        // Colocamos un '\0' al final
        buf[nread] = '\0';
        // Mostramos la respuesta
        cout << buf << '\n';
    }

    close(sfd);

    exit(EXIT_SUCCESS);
}
