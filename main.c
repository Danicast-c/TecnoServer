#include <sys/time.h>
#include <sys/types.h>
#include <Socket_Servidor.h>
#include <Socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <Communication.h>
#include <json-c/json.h>

#include "DataManager.h"

int numberClients = 0;                 /* Número clientes conectados */
int socketArray_Clients[MAX_CLIENTS] = {-1, -1, -1, -1};     /* Descriptores de sockets con clientes */
pthread_t threadArray[MAX_CLIENTS];                 /* Array de threads*/

void server_ClientCommunication(void *socketClient);

void listenForClients(void);

char *getLine(void);

void cleanUp(int *player, int *socketClient);

/**
 * Funcion main que controla el servidor
 * @return
 */
int main()
{
    pthread_t threadListener;               /* Thread que escucha por clientes*/
    int err = pthread_create(&(threadListener), NULL, &listenForClients, NULL);

    if (err != 0)
        printf("\ncan't create master thread :[%s]", strerror(err));
    else {
        pthread_detach(threadListener);
    }




    while (1) {
        printf("Write element to create (Bomb, Life o Speed): ");
        char *command = getLine();
        printf("Write the horizontal position (From 0 to 10): ");
        char *x_text = getLine();
        printf("Write the vertical position: ");
        char *pos_text = getLine();

        int x_value = textToInt(x_text);
        int pos_value = textToInt(pos_text);

        if (*command == 'B') {
            bombAdder(x_value, pos_value);
        } else if (*command == 'L') {
            lifeAdder(x_value, pos_value);
        } else if (*command == 'S') {
            speedAdder(x_value, pos_value);
        }

        printf("Element created\n");
    }
}

/**
 * Funcion que se encarga de la comunicacion entre el servidor y cliente,, crea el socket y abre el puerto
 * hace el manejo de clientes
 */
void listenForClients(void) {
    int socketServer;                     /* Descriptor del socket servidor */
    fd_set readFs;                          /* Descriptores de interes para select() */
    int buffer_size;                        /* Dice de que tamanho es el string que va a entrar */
    int maxNumber;                                /* Número de descriptor más grande */
    int i;                                    /* Para los for */

    /* Se abre el socket servidor, avisando por pantalla y saliendo si hay
     * algún problema */
    socketServer = Abre_Socket_Inet("dlsrap");
    if (socketServer == -1)
    {
        perror ("Error al abrir servidor, puerto ya utilizado");
        exit (-1);
    }


    json_object *jsp = dataToSend();
    while (1) {

        //Se tiene que limpiar, inicializar y rellenar los file descriptors de los sockets cada vez, para que select sepa cual utilizar
        FD_ZERO (&readFs);
        FD_SET (socketServer, &readFs);
        /* Se añaden para select() los sockets con los clientes ya conectados */
        for (i = 0; i < numberClients; i++)
            FD_SET (socketArray_Clients[i], &readFs);

        /* Se el valor del descriptor más grande. Si no hay ningún cliente,
         * devolverá 0 */
        maxNumber = maxMember(socketArray_Clients, numberClients);

        if (maxNumber < socketServer)
            maxNumber = socketServer;

        select(maxNumber + 1, &readFs, NULL, NULL,
               NULL);//Aqui queda esperando el servidor a que haya actividad en algun fd(file descriptor) activo

        if (FD_ISSET (socketServer, &readFs)) {//verifica si hay clientes nuevos y los registra
            int posicionEnArray = 0;
            newClient(socketServer, socketArray_Clients, &numberClients, &posicionEnArray);
            int err = pthread_create(&(threadArray[posicionEnArray]), NULL, &server_ClientCommunication,
                                     (void *) &socketArray_Clients[posicionEnArray]);

            if (err != 0)
                printf("\ncan't create thread :[%s]", strerror(err));
            else {
                pthread_detach((threadArray[posicionEnArray]));
            }
        }
    }
}

/**
 * Funcion que recibe el socket y se encarga de la comunicacion con el cliente conectado
 * @param socketClient es la direccion, de socket creado
 */
void server_ClientCommunication(void *socketClient) {

    int cancelability;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &cancelability);

    int player;

    if (socketClient == &socketArray_Clients[0]) {
        player = 1;
    } else if (socketClient == &socketArray_Clients[1]) {
        player = 2;
    } else if (socketClient == &socketArray_Clients[2]) {
        player = 3;
    } else if (socketClient == &socketArray_Clients[3]) {
        player = 4;
    } else {
        return;
    }

    int buffer_size;

    while (1) {
        /* Se lee lo enviado por el cliente y se escribe en pantalla */
        if ((Lee_Socket(*(int *) socketClient, (char *) &buffer_size, sizeof(int)) >
             0)) {                                                                          //si es mayor a 0 quiere decir que está entrando un dato
            int str_len = ntohl(
                    (uint32_t) buffer_size);                                    //Buffer size tiene el tamaño del paquete en int pero en formato de red, hay que pasarlo a int de C.
            json_object *mensaje = receiveJson((int *) socketClient, str_len);

            json_Parser(mensaje);
            json_object *respuesta = dataToSend();
            sendJson((int *) socketClient, respuesta);

        } else {
            printf("Jugador %d ha cerrado la conexión del socket %d\n", player, *(int *) socketClient);
            cleanUp(&player, (int *) socketClient);
            pthread_exit(NULL);
        }
    }
}

/**
 * Se encarga de leer la proxima linea
 * @return un array de caracteres con lo que se escribio
 */
char *getLine(void) {
    char *line = malloc(100), *linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if (line == NULL)
        return NULL;

    for (;;) {
        c = fgetc(stdin);
        if (c == EOF)
            break;

        if (--len == 0) {
            len = lenmax;
            char *linen = realloc(linep, lenmax *= 2);

            if (linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if ((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    return linep;
}

void cleanUp(int *player, int *socketClient) {
    close(*socketClient);
    *socketClient = -1;
    threadArray[*player - 1] = NULL;
    cleanUpPlayer(player);
    *player = 0;
    numberClients--;
}