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

struct Player {
    int id;
    double x;
    int pos;
    int speed;
    int life;
};

void json_Parser(json_object * jplayer);
json_object* data_toSend ();
void crearBomba(int x, int pos);
void crearBoost(int x, int pos);
void crearVida(int x, int pos);

void cleanUpPlayer(const int *player);

//----------------------Player_# = [id,x,pos,speed,life]

double bombas[50][3];
int bombas_index;

double vidas[50][3];
int vidas_index;

double boost[50][3];
int boost_index;


extern struct Player player_1;
extern struct Player player_2;
extern struct Player player_3;
extern struct Player player_4;

//----------------------Mutex
pthread_mutex_t lock;

#endif //SERVIDOR_DATAMANAGER_H
