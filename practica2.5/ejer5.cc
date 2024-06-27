// Copy+paste del ejercicio 2 añadiendo los cambios 6, 7, 8, 9 y 10

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
// Cambio 6: Incluimos wait y signal, y una macro con el nº de hijos para el ejer5
#include <sys/wait.h>
#include <signal.h>
using namespace std;
#define NUM_CHILDREN 5

#define BUF_SIZE 500

// Función que ejecutan los procesos hijos creados
void atiendeCliente(int sfd);

int main(int argc, char *argv[]){
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
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

    // Cambio 7: Crear procesos que atiendan mensajes concurrentemente
    for (int i = 0; i < NUM_CHILDREN; i++){
        pid_t pid = fork();
        // En el hijo (nos llevamos la funcionalidad del resto del main a atiendeCliente)
        if(pid == 0) atiendeCliente(sfd);
        else if (pid == -1){
            perror("fork");
            return 1;
        }
    }
    // Cambio 8: Esperamos a que un hijo acabe. Eso significa que ha recibido 'q' y hay
    // que finalizar el servidor: mandarle una señal de SIGTERM al resto de hijos
    // del grupo (pasar un 0 a kill)
    wait(NULL);
    kill(0, SIGTERM);
}

// Cambio 9: Nueva función que ejecutan los hijos creados
void atiendeCliente(int sfd){
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    char buf[BUF_SIZE];
    /* Read datagrams and echo them back to sender */

    for (;;) {
        peer_addr_len = sizeof(struct sockaddr_storage);
        nread = recvfrom(sfd, buf, BUF_SIZE, 0,
                (struct sockaddr *) &peer_addr, &peer_addr_len);
        if (nread == -1)
            continue;               /* Ignore failed request */

        // Cambio 10: Mostramos el pid del proceso que está atendiendo
        cout << "Atiende el proceso " << getpid() << '\n';

        char host[NI_MAXHOST], service[NI_MAXSERV];
        // Cambio 3: Añadimos el NI_NUMERICHOST (si no, intentará hacer DNS, que en las máquinas falla)
        int s = getnameinfo((struct sockaddr *) &peer_addr,
                        peer_addr_len, host, NI_MAXHOST,
                        service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
        // Cambio 4: Cambiamos este mensaje para que coincida con el del enunciado
        if (s == 0)
            printf("%zd bytes de %s:%s\n",
                    nread, host, service);
        else
            fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));

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
        cout << comando_recibido << '\n';
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
            // Cambio 0 finalizamos la ejecución del hijo.
            // Así el padre también sabrá que un hijo recibió 'q' y mandará a los demás hijos finalizar
            // terminando así con el servidor
            exit(0);
        }
        else{
            cout << "No se soporta el comando " << comando_recibido << '\n';
            // Pasamos a la siguiente vuelta del bucle y nos saltamos la respuesta al cliente
            continue; 
        }
        // Enviamos la respuesta
        if (sendto(sfd, buf, nread, 0,
                    (struct sockaddr *) &peer_addr,
                    peer_addr_len) != nread)
            fprintf(stderr, "Error sending response\n");
    }
}
