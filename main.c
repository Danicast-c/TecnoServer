#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <Socket_Servidor.h>
#include <Socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <json-c/json.h>

#define MAX_CLIENTS 10


void newClient(int servidor, int *clientes, int *nClientes);
int maxMember(int *tabla, int n);
void trimClients(int *tabla, int *n);
void sendString(int *clientes, int cliente, const char* dato);
char* receiveString(int *clientes,int cliente, int size);
int main()
{
    json_object* jsonObject = json_tokener_parse("\"booleanProperty\": true");

    int masterSocket;				/* Descriptor del socket servidor */
    int socketCliente[MAX_CLIENTS];/* Descriptores de sockets con clientes */
    int numeroClientes = 0;			/* Número clientes conectados */
    fd_set readFs;	/* Descriptores de interes para select() */
    int buffer_size;							/* Dice de que tamanho es el string que va a entrar */
    int maximo;							/* Número de descriptor más grande */
    int i;								/* Para los for */
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
        for (i=0; i<numeroClientes; i++)
        {
            if (FD_ISSET (socketCliente[i], &readFs))
            {
                /* Se lee lo enviado por el cliente y se escribe en pantalla */
                if ((Lee_Socket (socketCliente[i], (char *)&buffer_size, sizeof(int)) > 0)) { //si es mayor a 0 quiere decir que está entrando un dato
                    int str_len = ntohl(buffer_size);//Buffer size tiene el tamaño del paquete en int pero en formato de red, hay que pasarlo a int de C.
                    char* mensaje= receiveString(socketCliente, i + 1, str_len);
                    printf("Paquete de %d bytes recibido: %s\n",(int)strlen(mensaje),mensaje);//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<Mae a este punto ya está listo el mensaje para mandarlo al otro cliente, se usa
                    free(mensaje);//aqui ya muere el mensaje, está en heap, ¡HAY QUE HACER EL FREE MUY IMPORTANTE!
                }else
                {
                    printf ("Cliente %d ha cerrado la conexión\n", i+1);
                    socketCliente[i] = -1;//se "desactiva" el socket, mas adelante la funcion trimclients lo borra
                }
            }
        }


        if (FD_ISSET (masterSocket, &readFs))//verifica si hay clientes nuevos y los registra
            newClient(masterSocket, socketCliente, &numeroClientes);
    }
}

/*
 * Crea un nuevo socket cliente.
 * Se le pasa el socket servidor y el array de clientes, con el número de
 * clientes ya conectados.
 */
void newClient(int servidor, int *clientes, int *nClientes)
{

    /* Acepta la conexión con el cliente, guardándola en el array */
    clientes[*nClientes] = Acepta_Conexion_Cliente (servidor);
    (*nClientes)++;

    /* Si se ha superado el maximo de clientes, se cierra la conexión,
     * se deja tal como estaba y se vuelve. */
    if ((*nClientes) >= MAX_CLIENTS)
    {
        close (clientes[(*nClientes) -1]);
        (*nClientes)--;
        return;
    }

    sendString(clientes, *nClientes,"Bienvenido");
    /* Escribe en pantalla que ha aceptado al cliente y vuelve */
    printf ("Aceptado cliente %d\n", *nClientes);

}

/*
 * Función que devuelve el valor máximo en la tabla.
 * Supone que los valores válidos de la tabla son positivos y mayores que 0.
 * Devuelve 0 si n es 0 o la tabla es NULL */
int maxMember(int *tabla, int n)
{
    int i;
    int max;

    if ((tabla == NULL) || (n<1))
        return 0;

    max = tabla[0];
    for (i=0; i<n; i++)
        if (tabla[i] > max)
            max = tabla[i];

    return max;
}

/*
 * Busca en array todas las posiciones con -1 y las elimina, copiando encima
 * las posiciones siguientes.
 * Ejemplo, si la entrada es (3, -1, 2, -1, 4) con *n=5
 * a la salida tendremos (3, 2, 4) con *n=3
 */
void trimClients(int *tabla, int *n)
{
    int i,j;

    if ((tabla == NULL) || ((*n) == 0))
        return;

    j=0;
    for (i=0; i<(*n); i++)
    {
        if (tabla[i] != -1)
        {
            tabla[j] = tabla[i];
            j++;
        }
    }

    *n = j;
}

void sendString(int *clientes, int cliente, const char *dato){
    int str_len = (int)strlen(dato)+1; //+1 para que meta el caracter final
    char string[str_len];
    strcpy (string, dato);
    // Se transforna int a formato de red
    int Aux = htonl (str_len);
    Escribe_Socket (clientes[(cliente)-1], (char *)&Aux, sizeof(int)); //se envia longitud
    Escribe_Socket (clientes[(cliente)-1], string, str_len);// se envia string
    printf ("Paquete enviado:\nlongitud: %d\ndato: %s\n", str_len-1,string);

}

 char* receiveString(int *clientes, int cliente, const int str_len){
    char* string = (char*)malloc(str_len* sizeof(char));
    Lee_Socket (clientes[(cliente)-1], string, str_len);// se lee el string
    return string;
}
