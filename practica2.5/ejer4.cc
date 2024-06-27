// Partir del ejer2.cc y añadir los cambios 6 y 7 para multiplexación

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
// Cambio 0: incluimos iostream, time y el espacio de nombre std
#include <iostream>
#include <time.h>
// Añadida para usar select en la multiplexación
#include <sys/time.h>
using namespace std;
#define BUF_SIZE 500

int main(int argc, char *argv[]){
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    char buf[BUF_SIZE];
    // Cambio 1: Cambiamos a 3 el número de argumentos (nosotros recibimos la dirección y el puerto)
    if (argc != 3) {
        fprintf(stderr, "Usage: %s host port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    // Cambio 2: Pasamos argv[1] (el host) como primer argumento y argv[2] (el puerto) como segunod
    s = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    /* getaddrinfo() returns a list of address structures.
        Try each address until we successfully bind(2).
        If socket(2) (or bind(2)) fails, we (close the socket
        and) try the next address. */

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype,
                rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;                  /* Success */

        close(sfd);
    }

    if (rp == NULL) {               /* No address succeeded */
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);           /* No longer needed */

    /* Read datagrams and echo them back to sender */

    for (;;) {

        // Cambio 6: Multiplexar la lectura con socket y entrada estándar
        // Creamos el conjunto con los dos descriptores de fichero
        fd_set set;
        FD_ZERO(&set);
        FD_SET(sfd, &set);
        // La entrada estándar es el descriptor de fichero 1. También se puede usar STDIN_FILENO
        FD_SET(1, &set);
        // Monitorizo indefinidamente hasta que escriban en alguna
        // (realmente no hace falta hacer el máximo entre sfd y 1, porque siempre es sfd)
        if (select((sfd > 1 ? sfd : 1) + 1, &set, NULL, NULL, NULL) == -1){
            perror ("select");
            return 1;
        }
        // Si escribieron por el socket
        if (FD_ISSET(sfd, &set)){
            peer_addr_len = sizeof(struct sockaddr_storage);
            nread = recvfrom(sfd, buf, BUF_SIZE, 0,
                    (struct sockaddr *) &peer_addr, &peer_addr_len);
            if (nread == -1)
                continue;               /* Ignore failed request */

            char host[NI_MAXHOST], service[NI_MAXSERV];
            // Cambio 3: Añadimos el NI_NUMERICHOST (si no, intentará hacer DNS, que en las máquinas falla)
            s = getnameinfo((struct sockaddr *) &peer_addr,
                            peer_addr_len, host, NI_MAXHOST,
                            service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
            // Cambio 4: Cambiamos este mensaje para que coincida con el del enunciado
            if (s == 0)
                printf("%zd bytes de %s:%s\n",
                        nread, host, service);
            else
                fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
        }
        // Si escribieron por la entrada estándar
        else if(FD_ISSET(1, &set)){
            cin >> buf;
            nread = strlen(buf);
            cout << nread << " bytes from standard input\n";
        }

        // Cambio 5: Procesamos el mensaje recibido para devolver la hora, la fecha o finalizar
        // Colocamos un '\0' al final de la cadena
        buf[nread] = '\0';
        string comando_recibido = string(buf);
        // Si trae un '\n' al final, lo eliminamos
        if (comando_recibido.back() == '\n') comando_recibido.pop_back();
        // Obtenemos un estructura con el tiempo local
        time_t t;
        time(&t);
        struct tm * time_info = localtime(&t);
        if (comando_recibido == "t"){
            // Preparamos la hora en buf
            nread = strftime(buf, BUF_SIZE, "%F", time_info);
        }
        else if (comando_recibido == "d"){
            // Preparamos la fecha en buf
            nread = strftime(buf, BUF_SIZE, "%r", time_info);
        }
        else if (comando_recibido == "q"){
            cout << "Final del programa...\n";
            // Nos salimos del bucle infinito
            break;
        }
        else{
            cout << "No se soporta el comando " << comando_recibido << '\n';
            // Pasamos a la siguiente vuelta del bucle y nos saltamos la respuesta al cliente
            continue; 
        }

        // Cambio 7: Enviamos o mostramos por la salida estándar en función de por donde recibimos 
        if (FD_ISSET(sfd, &set)){
            if (sendto(sfd, buf, nread, 0,
                    (struct sockaddr *) &peer_addr,
                    peer_addr_len) != nread)
            fprintf(stderr, "Error sending response\n");
        }
        else if (FD_ISSET(1, &set)){
            cout << buf << '\n';
        }
        
        
    }
}
