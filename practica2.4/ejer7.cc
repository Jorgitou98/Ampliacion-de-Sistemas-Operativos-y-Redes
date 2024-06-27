#include <iostream>
#include <unistd.h>
#include <sys/types.h>
using namespace std;

void parent_func(int p_h[2], int h_p[2]){
    // Cerramos los extremos innecesarios de las tuberia
    // (lectura de p_h y escritua de h_p)
    close(p_h[0]);
    close(h_p[1]);
    char response;
    do{
        // Leemos un mensaje y se lo enviamos al hijo
        cout << "Introduce un mensaje:\n";
        string msg;
        getline(cin, msg);
        if (write(p_h[1], msg.c_str(), msg.size()) == -1){
            perror("write parent");
            exit(1);
        }
        // Nos bloquemos esperando la respuesta del hijo
        if (read(h_p[0], &response, 1) == -1){
            perror("read parent");
            exit(1);
        }
    } while(response == 'l'); // Si la respuesta es 'l', seguimos enviando
    // Si ya acabó la conversación, cerramos los descriptores de fichero restantes
    close(p_h[1]);
    close(h_p[0]);
    cout << "Fin del padre\n";
}

void child_func(int p_h[2], int h_p[2]){
    // Cerramos los extremos innecesarios de las tuberia
    // (escritura de p_h y lectura de h_p)
    close(p_h[1]);
    close(h_p[0]);
    char msg[100];
    // Trataremos 10 mensajes con el padre
    for (int i = 1; i <= 10; ++i){
        ssize_t read_len;
        // Nos bloqueamos hasta que el padre nos mande algún mensaje
        if ((read_len = read(p_h[0], msg, 100)) == -1){
            perror("read child");
            exit(1);
        }
        //Colocamos el '/0' al final de la cadena leída
        msg[read_len] ='\0';
        cout << "Recibi la cadena: " << msg << '\n';
        cout << "Es el mensaje numero " << i << '\n';

        char response = 'l';
        // En el último mensaje, le respondemos 'q' en lugar de 'l'
        if (i == 10) response = 'q';
        // Respondemos al padre
        if (write(h_p[1], &response, 1) == -1){
            perror("write child");
            exit(1);
        }
    }
    // Si ya acabó la conversación, cerramos los descriptores de fichero restantes
    close(p_h[0]);
    close(h_p[1]);
    cout << "Fin del hijo\n";
}


int main(){
    int p_h[2], h_p[2];
    if(pipe(p_h)== -1 || pipe(h_p)== -1){
        perror("pipe");
        return 1;
    }
    pid_t pid = fork();
    if(pid == -1){
        perror("fork");
        return 1;
    }
    // En el hijo
    else if(pid == 0){
        child_func(p_h, h_p);
    }
    // En el padre
    else{
        parent_func(p_h, h_p);
    }


    return 0;
}