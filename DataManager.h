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

int json_Parser(json_object *jplayer);

json_object *dataToSend();

void bombAdder(int x, int pos);

void speedAdder(int x, int pos);

void lifeAdder(int x, int pos);

void bombRemover(int id);

void lifeRemover(int id);

void speedRemover(int id);

void cleanUpPlayer(const int *player);

//----------------------Player_# = [id,x,pos,speed,life]

double bombs[50][4];
int bombs_index;

double lives[50][4];
int life_index;

double speed[50][4];
int speed_index;


extern struct Player player_1;
extern struct Player player_2;
extern struct Player player_3;
extern struct Player player_4;

//----------------------Mutex
pthread_mutex_t lock;

#endif //SERVIDOR_DATAMANAGER_H
