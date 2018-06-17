//
// Created by andresrq on 16/06/18.
//

#include "Commands.h"

/*
 * Crea un nuevo socket cliente.
 * Se le pasa el socket servidor y el array de clientes, con el número de
 * clientes ya conectados.
 */
void newClient(int servidor, int *clientes, int *nClientes) {

    /* Acepta la conexión con el cliente, guardándola en el array */
    clientes[*nClientes] = Acepta_Conexion_Cliente(servidor);
    (*nClientes)++;

    /* Si se ha superado el maximo de clientes, se cierra la conexión,
     * se deja tal como estaba y se vuelve. */
    if ((*nClientes) >= MAX_CLIENTS) {
        close(clientes[(*nClientes) - 1]);
        (*nClientes)--;
        return;
    }

    json_object *jp1 = json_object_new_object();

    json_object *p1_id = json_object_new_int(12);
    json_object *p1_x = json_object_new_int(56);
    json_object *p1_pos = json_object_new_int(34);
    json_object *p1_speed = json_object_new_int(78);
    json_object *p1_lifes = json_object_new_int(90);

    json_object_object_add(jp1, "id", p1_id);
    json_object_object_add(jp1, "x", p1_x);
    json_object_object_add(jp1, "position", p1_pos);
    json_object_object_add(jp1, "speed", p1_speed);
    json_object_object_add(jp1, "lives", p1_lifes);

    sendJson(clientes, *nClientes, jp1);
    //sendString(clientes, *nClientes,"Bienvenido");
    /* Escribe en pantalla que ha aceptado al cliente y vuelve */
    printf("Aceptado cliente %d\n", *nClientes);

}

/*
 * Función que devuelve el valor máximo en la tabla.
 * Supone que los valores válidos de la tabla son positivos y mayores que 0.
 * Devuelve 0 si n es 0 o la tabla es NULL */
int maxMember(const int *tabla, int n) {
    int i;
    int max;

    if ((tabla == NULL) || (n < 1))
        return 0;

    max = tabla[0];
    for (i = 0; i < n; i++)
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
void trimClients(int *tabla, int *n) {
    int i, j;

    if ((tabla == NULL) || ((*n) == 0))
        return;

    j = 0;
    for (i = 0; i < (*n); i++) {
        if (tabla[i] != -1) {
            tabla[j] = tabla[i];
            j++;
        }
    }

    *n = j;
}

void sendString(int *clientes, int cliente, const char *dato) {
    int str_len = (int) strlen(dato) + 1; //+1 para que meta el caracter final
    char string[str_len];
    strcpy(string, dato);
    // Se transforna int a formato de red
    int Aux = htonl((uint32_t) str_len);
    Escribe_Socket(clientes[(cliente) - 1], (char *) &Aux, sizeof(int)); //se envia longitud
    Escribe_Socket(clientes[(cliente) - 1], string, str_len);// se envia string
    printf("Paquete enviado:\nlongitud: %d\ndato: %s\n", str_len - 1, string);

}

char *receiveString(int *clientes, int cliente, const int str_len) {
    char *string = (char *) malloc(str_len * sizeof(char));
    Lee_Socket(clientes[(cliente) - 1], string, str_len);// se lee el string
    return string;
}

json_object *receiveJson(int *clientes, int cliente, const int str_len) {
    char *mensaje = receiveString(clientes, cliente, str_len);
    struct json_object *jsonObject = malloc(sizeof jsonObject);
    jsonObject = json_tokener_parse(mensaje);
    free(mensaje);
    return jsonObject;
}

void sendJson(int *clientes, int cliente, json_object *dato) {
    int str_len = (int) strlen(json_object_to_json_string(dato)) + 1; //+1 para que meta el caracter final
    char jsonString[str_len];
    strcpy(jsonString, json_object_to_json_string(dato));
    // Se transforna int a formato de red
    int Aux = htonl((uint32_t) str_len);
    Escribe_Socket(clientes[(cliente) - 1], (char *) &Aux, sizeof(int)); //se envia longitud
    Escribe_Socket(clientes[(cliente) - 1], jsonString, str_len);// se envia string
    printf("Paquete enviado:\nlongitud: %d\ndato: %s\n", str_len - 1, jsonString);

}
