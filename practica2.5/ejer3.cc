// Partir del código de man getaddrinfo para el cliente y hacer los cambios que se indican en los comentarios

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
// Cambio 0: Incluir iostream y el espacio de nombres std
#include <iostream>
using namespace std;

#define BUF_SIZE 500


int main(int argc, char *argv[]){
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    char buf[BUF_SIZE];
    // Cambio 0: Exigimos al menos 4 argumentos (programa, host, puerto y mensaje)
    if (argc < 4) {
        fprintf(stderr, "Usage: %s host port msg...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Obtain address(es) matching host/port */

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
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

    // Cambio 2: Lógica para enviar el comando, recibir la respuesta y mostrarla
    // Enviamos el comando por el socket establecido con el servidor
    if(send(sfd, argv[3], strlen(argv[3]), 0) == -1){
        perror("send");
        return 1;
    }
    // Si el argumento es una 'q' el servidor termina y no responde. Finalizamos también el cliente
    if (string(argv[3]) == "q") return 0;

    // Recibimos la respuesta del servidor por el socket
    ssize_t recv_size = recv(sfd, buf, BUF_SIZE, 0);
    if(recv_size == -1){
        perror("recv");
        return 1;
    }
    // Colocamos un '\0' al final del mensaje
    buf[recv_size] = '\0';
    // Mostramos lo que se recibe del servidor
    cout << buf << '\n';

    exit(EXIT_SUCCESS);
}
