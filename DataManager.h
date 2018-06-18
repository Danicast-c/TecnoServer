//
// Created by Daniel Castro on 16/6/18.
//

#ifndef SERVIDOR_DATAMANAGER_H
#define SERVIDOR_DATAMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <json-c/json.h>

void json_Parser(json_object * jplayer);
json_object* data_toSend ();
void crearBomba(int x, int pos);
void crearBoost(int x, int pos);
void crearVida(int x, int pos);

//----------------------Player_# = [id,x,pos,speed,life]

int bombas [50][3];
int bombas_index;

int vidas [50][3];
int vidas_index;

int boost [50][3];
int boost_index;





extern char player_1[5];
extern char player_2[5];
extern char player_3[5];
extern char player_4[5];

//----------------------Mutex
pthread_mutex_t lock;

#endif //SERVIDOR_DATAMANAGER_H
