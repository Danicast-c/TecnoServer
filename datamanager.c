//
// Created by Daniel Castro on 16/6/18.
//
#include <stdlib.h>
#include <stdio.h>
#include <json-c/json.h>
#include "datamanager.h"

char player_1[5]={0,0,0,0,0};
char player_2[5]={0,0,0,0,0};
char player_3[5]={0,0,0,0,0};
char player_4[5]={0,0,0,0,0};

int bombas [50][3] = {{124,4,500},{174,7,500},{217,10,500}};
int bombas_index = 3;

int vidas [50][3] = {{124,4,500},{174,7,500},{217,10,500}};
int vidas_index = 3;

int boost [50][3] = {{124,4,500},{174,7,500},{217,10,500}};
int boost_index = 3;

/**
 * Funcion usada para que el servidor cree las bombas
 * Almacena las nuevas bombas en el array bombas
 * @param x    Es la posicion en x que tendra la bomba asi el cliente sabra donde pintarla, va del 1 al 10
 * @param pos   Es la posicion respecto al largo de la pista que tendra la bomba, va de 0 a 100
 */
void crearBomba(int x, int pos){

    if (0<= x <= 10 && 0<=pos){
        int x_tosend = (x-5)/5;
        int pos_tosend = pos;

        bombas[bombas_index][0] = x_tosend;
        bombas[bombas_index][1] = pos_tosend;
        bombas[bombas_index][2] = 1;
        bombas_index+=1;


    } else {
        printf("El valor de X debe estar entre 0 y 10, el valor de Pos entre 0 y 100");
    }
};

/**
 * Funcion usada para que el servidor cree las vidas
 * Almacena las nuevas vidas en el array vidas
 * @param x     Es la posicion en x que tendra la vida asi el cliente sabra donde pintarla, va del 1 al 10
 * @param pos   Es la posicion respecto al largo de la pista que tendra la vida, va de 0 a 100
 */
void crearVida(int x, int pos){

    if (0<= x <= 10 && 0<=pos){
        int x_tosend = (x-5)/5;
        int pos_tosend = pos;

        vidas[vidas_index][0] = x_tosend;
        vidas[vidas_index][1] = pos_tosend;
        vidas[vidas_index][2] = 1;
        vidas_index+=1;


    } else {
        printf("El valor de X debe estar entre 0 y 10, el valor de Pos entre 0 y 100");
    }
};

/**
 * Funcion usada para que el servidor cree los boost
 * Almacena los nuevos boost en el array boost
 * @param x     Es la posicion en x que tendra el boost asi el cliente sabra donde pintarla, va del 1 al 10
 * @param pos   Es la posicion respecto al largo de la pista que tendra el boost, va de 0 a 100
 */
void crearBoost(int x, int pos){

    if (0<= x <= 10 && 0<=pos){
        int x_tosend = (x-5)/5;
        int pos_tosend = pos;

        boost[boost_index][0] = x_tosend;
        boost[boost_index][1] = pos_tosend;
        boost[boost_index][2] = 1;
        boost_index+=1;


    } else {
        printf("El valor de X debe estar entre 0 y 10, el valor de Pos entre 0 y 100");
    }
};

/**
 *Esta funcion se encarga de parsear el Json enviado por los diferentes clientes
 * Almacena la informacion de los jugadores
 * @param jplayer   recibe el json_object que fue recibido mediante el socket
 * @param player_Id un valor numerico que indica el numero de jugador que envio el socket
 */
void json_Parser(json_object *jplayer) {

    pthread_mutex_lock(&lock);

    printf ("The json object created: %s \n",json_object_to_json_string(jplayer));

    int id = json_object_get_int(json_object_object_get(jplayer,"id"));
    int x = json_object_get_int(json_object_object_get(jplayer,"x"));
    int pos = json_object_get_int(json_object_object_get(jplayer,"position"));
    int speed = json_object_get_int(json_object_object_get(jplayer,"speed"));
    int life = json_object_get_int(json_object_object_get(jplayer,"life"));


    switch(id) {

        case 1:

            player_1[0]=(char) id;
            player_1[1]=(char) x;
            player_1[2]=(char) pos;
            player_1[3]=(char) speed;
            player_1[4]=(char) life;
            break; /* optional */

        case 2:
            player_2[0]=(char)id;
            player_2[1]=(char)x;
            player_2[2]=(char)pos;
            player_2[3]=(char)speed;
            player_2[4]=(char)life;
            break; /* optional */

        case 3:
            player_3[0]=(char)id;
            player_3[1]=(char)x;
            player_3[2]=(char)pos;
            player_3[3]=(char)speed;
            player_3[4]=(char)life;
            break; /* optional */

        case 4:
            player_4[0]=(char)id;
            player_4[1]=(char)x;
            player_4[2]=(char)pos;
            player_4[3]=(char)speed;
            player_4[4]=(char)life;
            break; /* optional */

        default: printf("Error in jsonParcer");

    }


   // printf("valor es: %d",val);
    free(jplayer);

//    json_object_object_foreach(jplayer, key, val) {
//        if (key == "x") {
//            printf("valor de x: %d", json_object_get_int(val));
//        }
//
//        printf("value: %d", json_object_get_int(val));
//        }
//

    pthread_mutex_unlock(&lock);
}



/**
 *Funcion que copila toda la informacion actual de los diferentes jugadores, bombas, vidas, boost
 * y se encarga de almacenarlos en un solo json.
 * @return retorna el msterJson con toda la informacion necesaria
 */
json_object* data_toSend (){



    json_object* masterJson = json_object_new_object(); // crea el json master

//
//    json_object *jpos = json_object_new_string("position");
//    json_object *jx = json_object_new_string("x");
//    json_object *jspeed = json_object_new_string("speed");
//    json_object *jlifes = json_object_new_string("lives");

    //-------------------------------------------player 1
    json_object* jp1 = json_object_new_object();

    json_object *p1_id = json_object_new_int(player_1[0]);
    json_object *p1_pos = json_object_new_int(player_1[1]);
    json_object *p1_x = json_object_new_int(player_1[2]);
    json_object *p1_speed = json_object_new_int(player_1[3]);
    json_object *p1_life = json_object_new_int(player_1[4]);

    json_object_object_add(jp1,"id", p1_id);
    json_object_object_add(jp1,"x", p1_x);
    json_object_object_add(jp1,"position", p1_pos);
    json_object_object_add(jp1,"speed", p1_speed);
    json_object_object_add(jp1,"life", p1_life);

    //-------------------------------------------player 2
    json_object* jp2 = json_object_new_object();

    json_object *p2_id = json_object_new_int(player_2[0]);
    json_object *p2_pos = json_object_new_int(player_2[1]);
    json_object *p2_x = json_object_new_int(player_2[2]);
    json_object *p2_speed = json_object_new_int(player_2[3]);
    json_object *p2_life = json_object_new_int(player_2[4]);

    json_object_object_add(jp2,"id", p2_id);
    json_object_object_add(jp2,"x", p2_x);
    json_object_object_add(jp2,"position", p2_pos);
    json_object_object_add(jp2,"speed", p2_speed);
    json_object_object_add(jp2,"life", p2_life);

    //-------------------------------------------player 3
    json_object* jp3 = json_object_new_object();

    json_object *p3_id = json_object_new_int(player_3[0]);
    json_object *p3_pos = json_object_new_int(player_3[1]);
    json_object *p3_x = json_object_new_int(player_3[2]);
    json_object *p3_speed = json_object_new_int(player_3[3]);
    json_object *p3_life = json_object_new_int(player_3[4]);

    json_object_object_add(jp3,"id", p3_id);
    json_object_object_add(jp3,"x", p3_x);
    json_object_object_add(jp3,"position", p3_pos);
    json_object_object_add(jp3,"speed", p3_speed);
    json_object_object_add(jp3,"life", p3_life);

    //-------------------------------------------player 4
    json_object* jp4 = json_object_new_object();

    json_object *p4_id = json_object_new_int(player_4[0]);
    json_object *p4_pos = json_object_new_int(player_4[1]);
    json_object *p4_x = json_object_new_int(player_4[2]);
    json_object *p4_speed = json_object_new_int(player_4[3]);
    json_object *p4_life = json_object_new_int(player_4[4]);

    json_object_object_add(jp4,"id", p4_id);
    json_object_object_add(jp4,"x", p4_x);
    json_object_object_add(jp4,"position", p4_pos);
    json_object_object_add(jp4,"speed", p4_speed);
    json_object_object_add(jp4,"life", p4_life);

    //----------------------------------------------Array de jugadores
    json_object *playersArray = json_object_new_array();

    json_object_array_add(playersArray,jp1);
    json_object_array_add(playersArray,jp2);
    json_object_array_add(playersArray,jp3);
    json_object_array_add(playersArray,jp4);

    //----------------------------------------------BOMBAS

    json_object *obsArray = json_object_new_array();

    int imax = sizeof(bombas)/12;


    for(int i=0;i<imax;i++ ) {

        if (bombas[i][2] != 0) {
            json_object *eleh = json_object_new_object();

            json_object *hpos = json_object_new_int(bombas[i][0]);
            json_object *hx = json_object_new_int(bombas[i][1]);
            json_object *htime = json_object_new_int(bombas[i][2]);

            json_object_object_add(eleh, "x", hx);
            json_object_object_add(eleh, "position", hpos);
            json_object_object_add(eleh, "active", htime);

            json_object_array_add(obsArray, eleh);
        }

    }
    //----------------------------------------------Vidas

    json_object *vidasArray = json_object_new_array();

    int vimax = sizeof(vidas)/12;


    for(int i=0;i<vimax;i++ ) {

        if (vidas[i][2] != 0) {
            json_object *eleh = json_object_new_object();

            json_object *hpos = json_object_new_int(vidas[i][0]);
            json_object *hx = json_object_new_int(vidas[i][1]);
            json_object *htime = json_object_new_int(vidas[i][2]);

            json_object_object_add(eleh, "x", hx);
            json_object_object_add(eleh, "position", hpos);
            json_object_object_add(eleh, "active", htime);

            json_object_array_add(vidasArray, eleh);
        }

    }
    //----------------------------------------------Boost

    json_object *boostArray = json_object_new_array();

    int bimax = sizeof(vidas)/12;


    for(int i=0;i<bimax;i++ ) {

        if (vidas[i][2] != 0) {
            json_object *eleh = json_object_new_object();

            json_object *hpos = json_object_new_int(boost[i][0]);
            json_object *hx = json_object_new_int(boost[i][1]);
            json_object *htime = json_object_new_int(boost[i][2]);

            json_object_object_add(eleh, "x", hx);
            json_object_object_add(eleh, "position", hpos);
            json_object_object_add(eleh, "active", htime);

            json_object_array_add(boostArray, eleh);
        }

    }


    //----------------------------------------------Array de jugadores
    json_object *osbstaclesArray = json_object_new_array();

    json_object_array_add(osbstaclesArray,jp1);
    json_object_array_add(osbstaclesArray,jp2);
    json_object_array_add(osbstaclesArray,jp3);
    json_object_array_add(osbstaclesArray,jp4);


    //-----------------------------------------------add a master

    json_object_object_add(masterJson,"players", playersArray);


    json_object_object_add(masterJson,"bombas", obsArray);

    json_object_object_add(masterJson,"vidas", vidasArray);

    json_object_object_add(masterJson,"boost", vidasArray);


    printf("The json object created: %s\n", json_object_to_json_string(masterJson));

    return masterJson ;
}
