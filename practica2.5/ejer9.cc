// Partir del código del ejercicio 8 y hacer los cambios 11, 12 y 13

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
// Cambio 0: Incluir iostream y espacio de nombres std para trabajar con c++
#include <iostream>
// Cambio 11: Incluimos signal para manejar la señal SIGCHLD y wait para recoger su estado
#include <signal.h>
#include <sys/wait.h>
using namespace std;

#define BUF_SIZE 500

// Declaramos la función que ejecutan los hijos para atender a los clientes
void atenderCliente(int afd);

// Función para manejar la señal de finalización de un hijo
void handler_sigchld(int signal);

int main(int argc, char *argv[]){
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;

    // Cambio 1: Pedimos 3 argumentos (programa, host y puerto)
    if (argc != 3) {
        fprintf(stderr, "Usage: %s host port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    // Cambio 2: Usar SOCK_STREAM (TCP) en lugar de SOCK_DGRAM (UDP) 
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    // Cambio 3: Pasamos el host (argv[1]) como primer argumento y el puerto (argv[2]) como segundo
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

    // Cambio 4: Poner el servidor TCP en escucha (listen).
    // Permitimos hasta 5 conexiones pendientes
    listen(sfd, 5);

    // Cambio 12: Instalamos un manejador para la señal SIGCHLD que manda un hijo al terminar
    struct sigaction act;
    act.sa_handler = handler_sigchld;
    act.sa_flags = SA_RESTART;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGCHLD);
    sigaction(SIGCHLD, &act, NULL);

    /* Read datagrams and echo them back to sender */

    for (;;) {
        // Cambio 5: Hacemos un accept para recibir la conexión
        // Nos devuelve el decriptor de fichero que habrá que utilizar en adelante para comunicarse
        // Recibimos la dirección del otro extremo (del cliente) y la mostramos
        int afd = accept(sfd, (struct sockaddr *) &peer_addr, &peer_addr_len);       

        // Mostramos la dirección del cliente conectado
        char host[NI_MAXHOST], service[NI_MAXSERV];
        // Colocar NI_NUMERICHOST para que no haga DNS
        int s = getnameinfo((struct sockaddr *) &peer_addr,
                        peer_addr_len, host, NI_MAXHOST,
                        service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
        if (s == 0) cout << "Conexión desde " << host << ' ' << service << '\n';
        else {
            fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
            exit(1);
        }

        // Cambio 8: Mediante fork() creamos un hijo que atienda la conexión
        int pid = fork();
        // Si somos el hijo, ejecutamos la función atenderCliente();
        if(pid == 0) {
            atenderCliente(afd);
            exit(0);
        }
        else if (pid == -1){
            perror("fork");
            return 1;
        }
        // Cerramos el socket devuelto por accept en el padre
        close(afd);
    }
}

// Cambio 9: LLevamos la funcionalidad de eco a la función que ejecutan los hijos
void atenderCliente(int afd){
    ssize_t nread;
    char buf[BUF_SIZE];
    // Cambio 10: Mostramos el pid del proceso que atiende
    cout << "Atendido por el proceso " << getpid() << '\n';
    // Cambio 6: Hacemos eco repetidamente a los mensajes del cliente
    // (saldremos cuando el cliente haga ctrl+c pues recibimos 0 bytes)
    while(1){
        nread = recv(afd, buf, BUF_SIZE, 0);
        if (nread == -1) continue;               /* Ignore failed request */
        // Salimos cuando recibamos 0 bytes (ctrl+c del cliente)
        else if (nread == 0){
            cout << "Conexion terminada con el proceso " << getpid() << '\n';
            break; 
        }
        if (send(afd, buf, nread, 0) != nread) fprintf(stderr, "Error sending response\n");
    }
    // Cambio 7: Cerramos el descriptor de fichero del accept
    close(afd);
}

// Cambio 13: Si un hijo a terminado, informamos por pantalla y recogemos su estado con wait (para que no quede zombie)
void handler_sigchld(int signal){
    pid_t pid = wait(NULL);
    cout << "Mi hijo " << pid << " ha terminado\n";
}