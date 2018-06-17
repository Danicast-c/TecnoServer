#include <sys/time.h>
#include <sys/types.h>
#include <Socket_Servidor.h>
#include <Socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <Commands.h>
#include <json-c/json.h>

#include "datamanager.h"

int socketCliente[MAX_CLIENTS] = {-1, -1, -1, -1};     /* Descriptores de sockets con clientes */
pthread_t threadArray[MAX_CLIENTS];                 /* Array de threads*/

void server_client_communication(void *ClientSocket);

int main()
{
    int masterSocket;                    /* Descriptor del socket servidor */
    int numeroClientes = 0;                /* Número clientes conectados */
    fd_set readFs;                        /* Descriptores de interes para select() */
    int buffer_size;                    /* Dice de que tamanho es el string que va a entrar */
    int maximo;							/* Número de descriptor más grande */
    int i;								/* Para los for */

    //Lo que el servidor recibe del cliente
    //json_object* jp1 = json_object_new_object();

    //json_object *p1_id = json_object_new_int(12);
    //json_object *p1_x = json_object_new_int(56);
    //json_object *p1_pos = json_object_new_int(34);
    //json_object *p1_speed = json_object_new_int(78);
    //json_object *p1_life = json_object_new_int(90);

    //json_object_object_add(jp1,"id", p1_id );
    //json_object_object_add(jp1,"x", p1_x);
    //json_object_object_add(jp1,"position", p1_pos);
    //json_object_object_add(jp1,"speed", p1_speed);
    //json_object_object_add(jp1,"life", p1_life);

    //Para guardar los datos recibidos
    //json_Parser(jp1,1);

    //Lo que el servidor debe enviar al cliente
    //json_object* prueba = data_toSend();

    //mensaje que se recibe del jugador

    /* Se abre el socket servidor, avisando por pantalla y saliendo si hay
     * algún problema */
    masterSocket = Abre_Socket_Inet ("dlsrap");
    if (masterSocket == -1)
    {
        perror ("Error al abrir servidor, puerto ya utilizado");
        exit (-1);
    }

    while (1)
    {

        //Borra sockets inactivos al inicio de cada iteración
        trimClients(socketCliente, &numeroClientes);
        //Se tiene que limpiar, inicializar y rellenar los file descriptors de los sockets cada vez, para que select sepa cual utilizar
        FD_ZERO (&readFs);
        FD_SET (masterSocket, &readFs);
        /* Se añaden para select() los sockets con los clientes ya conectados */
        for (i=0; i<numeroClientes; i++)
            FD_SET (socketCliente[i], &readFs);

        /* Se el valor del descriptor más grande. Si no hay ningún cliente,
         * devolverá 0 */
        maximo = maxMember(socketCliente, numeroClientes);

        if (maximo < masterSocket)
            maximo = masterSocket;


        select (maximo + 1, &readFs, NULL, NULL, NULL);//Aqui queda esperando el servidor a que haya actividad en algun fd(file descriptor) activo

        /* Se comprueba si algún cliente ya conectado ha enviado algo */
//        for (i=0; i<numeroClientes; i++)
//        {
//            if (FD_ISSET (socketCliente[i], &readFs))
//            {
//                /* Se lee lo enviado por el cliente y se escribe en pantalla */
//                if ((Lee_Socket (socketCliente[i], (char *)&buffer_size, sizeof(int)) > 0)) { //si es mayor a 0 quiere decir que está entrando un dato
//                    int str_len = ntohl(
//                            (uint32_t) buffer_size);//Buffer size tiene el tamaño del paquete en int pero en formato de red, hay que pasarlo a int de C.
//                    char* mensaje= receiveString(socketCliente, i + 1, str_len);
//                    printf("Paquete de %d bytes recibido: %s\n",(int)strlen(mensaje),mensaje);//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<Mae a este punto ya está listo el mensaje para mandarlo al otro cliente, se usa
//                    free(mensaje);//aqui ya muere el mensaje, está en heap, ¡HAY QUE HACER EL FREE MUY IMPORTANTE!
//                }else
//                {
//                    printf ("Cliente %d ha cerrado la conexión\n", i+1);
//                    socketCliente[i] = -1;//se "desactiva" el socket, mas adelante la funcion trimclients lo borra
//                }
//            }
//        }


        if (FD_ISSET (masterSocket, &readFs)) {//verifica si hay clientes nuevos y los registra
            newClient(masterSocket, socketCliente, &numeroClientes);
            int err = pthread_create(&(threadArray[i]), NULL, &server_client_communication,
                                     (void *) &socketCliente[numeroClientes - 1]);

            if (err != 0)
                printf("\ncan't create thread :[%s]", strerror(err));
            else {
                pthread_join((threadArray[i]), NULL);
            }
        }
    }
}

void server_client_communication(void *socketCliente) {

    int cancelability;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &cancelability);

    int player;

    if (socketCliente == &socketCliente[0]) {
        player = 1;
    } else if (socketCliente == &socketCliente[1]) {
        player = 2;
    } else if (socketCliente == &socketCliente[2]) {
        player = 3;
    } else if (socketCliente == &socketCliente[3]) {
        player = 4;
    } else {
        return;
    }

    int buffer_size;

    while (1) {
        /* Se lee lo enviado por el cliente y se escribe en pantalla */
        if ((Lee_Socket(*(int *) socketCliente, (char *) &buffer_size, sizeof(int)) >
             0)) { //si es mayor a 0 quiere decir que está entrando un dato
            int str_len = ntohl(
                    (uint32_t) buffer_size);//Buffer size tiene el tamaño del paquete en int pero en formato de red, hay que pasarlo a int de C.
            json_object *mensaje = receiveJson((int *) socketCliente, str_len);
            json_Parser(mensaje, player);
            json_object *respuesta = data_toSend();
            sendJson((int *) socketCliente, respuesta);
        } else {
            printf("Cliente %d ha cerrado la conexión\n", player);
            *(int *) socketCliente = -1;
            pthread_cancel(threadArray[player - 1]);
        }
    }
}
