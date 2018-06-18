//
// Created by andresrq on 16/06/18.
//

#include "Communication.h"
#include "../DataManager.h"

/*
 * Crea un nuevo socket cliente.
 * Se le pasa el socket servidor y el array de clientes, con el número de
 * clientes ya conectados.
 */
void newClient(int server, int *clients, int *numberClients, int *positionInArray) {

    for (; *positionInArray < MAX_CLIENTS; (*positionInArray)++) {
        if (clients[*positionInArray] == -1) {
            clients[*positionInArray] = Acepta_Conexion_Cliente(
                    server);    /* Acepta la conexión con el cliente, guardándola en el array */
            (*numberClients)++;
            break;
        }
    }

    if (*positionInArray == MAX_CLIENTS) {
        *positionInArray = 0;
        return;
    }

    json_object *jsonID = json_object_new_object();
    json_object *id = json_object_new_int(*positionInArray + 1);
    json_object_object_add(jsonID, "id", id);

    //sendString(clients, *numberClients, "Bienvenido");
    sendJson(&clients[*positionInArray], jsonID);
    /* Escribe en pantalla que ha aceptado al cliente y vuelve */
    printf("Aceptado cliente número %d y jugador %d\n", *numberClients, *positionInArray + 1);

}

/*
 * Función que devuelve el valor máximo en la tabla.
 * Supone que los valores válidos de la tabla son positivos y mayores que 0.
 * Devuelve 0 si n es 0 o la tabla es NULL */
int maxMember(const int *table, int n) {
    int i;
    int max;

    if ((table == NULL) || (n < 1))
        return 0;

    max = table[0];
    for (i = 0; i < n; i++)
        if (table[i] > max)
            max = table[i];

    return max;
}

/*
 * Busca en array todas las posiciones con -1 y las elimina, copiando encima
 * las posiciones siguientes.
 * Ejemplo, si la entrada es (3, -1, 2, -1, 4) con *n=5
 * a la salida tendremos (3, 2, 4) con *n=3
 */
void trimClients(int *table, int *n) {
    int i, j;

    if ((table == NULL) || ((*n) == 0))
        return;

    j = 0;
    for (i = 0; i < (*n); i++) {
        if (table[i] != -1) {
            table[j] = table[i];
            j++;
        }
    }

    //*n = j;
}

void sendString(int *clients, int client, const char *data) {
    int str_len = (int) strlen(data) + 1; //+1 para que meta el caracter final
    char string[str_len];
    strcpy(string, data);
    // Se transforna int a formato de red
    int Aux = htonl((uint32_t) str_len);
    Escribe_Socket(clients[(client) - 1], (char *) &Aux, sizeof(int)); //se envia longitud
    Escribe_Socket(clients[(client) - 1], string, str_len);// se envia string
    printf("Paquete enviado:\nlongitud: %d\ndata: %s\n", str_len - 1, string);

}

void sendJson(const int *client, json_object *data) {
    int str_len = (int) strlen(json_object_to_json_string(data)) + 1; //+1 para que meta el caracter final
    char jsonString[str_len];
    strcpy(jsonString, json_object_to_json_string(data));
    // Se transforna int a formato de red
    int Aux = htonl((uint32_t) str_len);
    Escribe_Socket(*client, (char *) &Aux, sizeof(int)); //se envia longitud
    Escribe_Socket(*client, jsonString, str_len);// se envia string
}

char *receiveString(const int *client, const int str_len) {
    char *string = (char *) malloc(str_len * sizeof(char));
    Lee_Socket(*client, string, str_len);// se lee el string
    return string;
}

json_object *receiveJson(const int *client, const int str_len) {
    char *message = receiveString(client, str_len);
    json_object *jsonObject = malloc(sizeof jsonObject);
    if (*message == 'B') {
        int id = textToInt(message);
        bombRemover(id);
        jsonObject = json_tokener_parse("");
    } else {
        jsonObject = json_tokener_parse(message);
    }
    free(message);
    return jsonObject;
}

int textToInt(char *text) {
    int length = (int) strlen(text) - 1; // Removes the \n

    int number = 0;
    int i = 0;

    for (; i < length; i++) {
        number += (text[i] - '0') * pow(10, (length - i - 1));
    }

    return number;
}
