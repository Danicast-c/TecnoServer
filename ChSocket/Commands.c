//
// Created by andresrq on 16/06/18.
//

#include "Commands.h"

/*
 * Crea un nuevo socket cliente.
 * Se le pasa el socket servidor y el array de clientes, con el número de
 * clientes ya conectados.
 */
void newClient(int servidor, int *clientes, int *nClientes, int *posicionEnArray) {

    for (; *posicionEnArray < MAX_CLIENTS; (*posicionEnArray)++) {
        if (clientes[*posicionEnArray] == -1) {
            clientes[*posicionEnArray] = Acepta_Conexion_Cliente(
                    servidor);    /* Acepta la conexión con el cliente, guardándola en el array */
            (*nClientes)++;
            break;
        }
    }

    if (*posicionEnArray == MAX_CLIENTS) {
        *posicionEnArray = 0;
        return;
    }

    json_object *jsonID = json_object_new_object();
    json_object *id = json_object_new_int(*posicionEnArray + 1);
    json_object_object_add(jsonID, "id", id);

    //sendString(clientes, *nClientes, "Bienvenido");
    sendJson(&clientes[*posicionEnArray], jsonID);
    /* Escribe en pantalla que ha aceptado al cliente y vuelve */
    printf("Aceptado cliente número %d y jugador %d\n", *nClientes, *posicionEnArray + 1);

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

void sendJson(const int *cliente, json_object *dato) {
    int str_len = (int) strlen(json_object_to_json_string(dato)) + 1; //+1 para que meta el caracter final
    char jsonString[str_len];
    strcpy(jsonString, json_object_to_json_string(dato));
    // Se transforna int a formato de red
    int Aux = htonl((uint32_t) str_len);
    Escribe_Socket(*cliente, (char *) &Aux, sizeof(int)); //se envia longitud
    Escribe_Socket(*cliente, jsonString, str_len);// se envia string
}

char *receiveString(const int *cliente, const int str_len) {
    char *string = (char *) malloc(str_len * sizeof(char));
    Lee_Socket(*cliente, string, str_len);// se lee el string
    return string;
}

json_object *receiveJson(const int *cliente, const int str_len) {
    char *mensaje = receiveString(cliente, str_len);
    json_object *jsonObject = malloc(sizeof jsonObject);
    jsonObject = json_tokener_parse(mensaje);
    free(mensaje);
    return jsonObject;
}
