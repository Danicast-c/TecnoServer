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
#include <math.h>

#define MAX_CLIENTS 4

void newClient(int server, int *clients, int *numberClients, int *positionInArray);

int maxMember(const int *table, int n);

void trimClients(int *table, int *n);

void sendString(int *clients, int client, const char *data);

char *receiveString(const int *client, int str_len);

json_object *receiveJson(const int *client, int str_len);

void sendJson(const int *client, json_object *data);

int textToInt(char *text);

#endif //TECNSERVER_COMMANDS_H
