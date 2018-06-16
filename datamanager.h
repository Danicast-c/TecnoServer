//
// Created by Daniel Castro on 16/6/18.
//

#ifndef SERVIDOR_DATAMANAGER_H
#define SERVIDOR_DATAMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>

void data_toSend ();

//----------------------Player_# = [id,x,pos,speed,life]
extern char player_1[5];
extern char player_2[5];
extern char player_3[5];
extern char player_4[5];

#endif //SERVIDOR_DATAMANAGER_H
