//
// Created by andresrq on 16/06/18.
//

#ifndef TECNSERVER_COMMANDS_H
#define TECNSERVER_COMMANDS_H

#include <json-c/json_tokener.h>
#include <json-c/json_object.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <Socket_Servidor.h>
#include <Socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>

#define MAX_CLIENTS 10

void newClient(int servidor, int *clientes, int *nClientes);

int maxMember(const int *tabla, int n);

void trimClients(int *tabla, int *n);

void sendString(int *clientes, int cliente, const char *dato);

char *receiveString(int *clientes, int cliente, int str_len);

void sendJson(int *clientes, int cliente, json_object *dato);

json_object *receiveJson(int *clientes, int cliente, int str_len);

#endif //TECNSERVER_COMMANDS_H
