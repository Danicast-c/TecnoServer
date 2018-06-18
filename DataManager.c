//
// Created by Daniel Castro on 16/6/18.
//
#include <stdlib.h>
#include <stdio.h>
#include <json-c/json.h>
#include "DataManager.h"

struct Player player_1;
struct Player player_2;
struct Player player_3;
struct Player player_4;

double bombas[50][4] = {{1,5, 500,  1},
                        {2,2, 2000,  1},
                        {3,6, 3000, 1}};
int bombas_index = 3;

double vidas[50][4] = {{1,7, 550,  1},
                       {2,2, 2050,  1},
                       {3,1, 3050, 1},
                       {4,5, 5700, 1}};
int vidas_index = 4;

double boost[50][4] = {{1,5, 600,  1},
                       {2,8, 2100,  1},
                       {3,9, 3050, 1},
                        {4,5, 5000, 1}};
int boost_index = 4;

/**
 * Funcion usada para que el servidor cree las bombas
 * Almacena las nuevas bombas en el array bombas
 * @param x    Es la posicion en x que tendra la bomba asi el cliente sabra donde pintarla, va del 1 al 10
 * @param pos   Es la posicion respecto al largo de la pista que tendra la bomba, va de 0 a 100
 */
void crearBomba(int x, int pos){

    if (0<= x <= 10 && 0<=pos){
        double x_tosend = x;
        double pos_tosend = pos;


        bombas[bombas_index][0] = bombas_index+1;
        bombas[bombas_index][1] = x_tosend;
        bombas[bombas_index][2] = pos_tosend;
        bombas[bombas_index][3] = 1;
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
        double x_tosend = x;
        double pos_tosend = pos;

        vidas[vidas_index][0] = vidas_index+1;
        vidas[vidas_index][1] = x_tosend;
        vidas[vidas_index][2] = pos_tosend;
        vidas[vidas_index][3] = 1;
        vidas_index= vidas_index+1;


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
        double x_tosend = x;
        double pos_tosend = pos;

        printf("antes x: %f \n",boost[boost_index][1]);

        boost[boost_index+1][0] = boost_index+1;
        boost[boost_index+1][1] = x_tosend;
        boost[boost_index+1][2] = pos_tosend;
        boost[boost_index+1][3] = 1;
        boost_index= boost_index + 1;

        printf("desp x: %f \n",boost[boost_index][1]);

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
void bomb_Remove(int id) {
    bombas[id-1][3]=0;
}
void vidas_Remove(int id) {
    vidas[id-1][3]=0;
}
void boost_Remove(int id) {
    boost[id-1][3]=0;
}


void json_Parser(json_object *jplayer) {

    pthread_mutex_lock(&lock);

    printf ("The json object created: %s \n",json_object_to_json_string(jplayer));

    int id = json_object_get_int(json_object_object_get(jplayer,"id"));
    double x = json_object_get_double(json_object_object_get(jplayer, "x"));
    int pos = json_object_get_int(json_object_object_get(jplayer,"position"));
    int speed = json_object_get_int(json_object_object_get(jplayer,"speed"));
    int life = json_object_get_int(json_object_object_get(jplayer,"life"));


    switch(id) {

        case 1:

            player_1.id = id;
            player_1.x = x;
            player_1.pos = pos;
            player_1.speed = speed;
            player_1.life = life;
            break; /* optional */

        case 2:
            player_2.id = id;
            player_2.x = x;
            player_2.pos = pos;
            player_2.speed = speed;
            player_2.life = life;
            break; /* optional */

        case 3:
            player_3.id = id;
            player_3.x = x;
            player_3.pos = pos;
            player_3.speed = speed;
            player_3.life = life;
            break; /* optional */

        case 4:
            player_4.id = id;
            player_4.x = x;
            player_4.pos = pos;
            player_4.speed = speed;
            player_4.life = life;
            break; /* optional */

        default:
            printf("Error in jsonParcer\n");

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

    json_object *p1_id = json_object_new_int(player_1.id);
    json_object *p1_pos = json_object_new_int(player_1.pos);
    json_object *p1_x = json_object_new_double(player_1.x);
    json_object *p1_speed = json_object_new_int(player_1.speed);
    json_object *p1_life = json_object_new_int(player_1.life);

    json_object_object_add(jp1,"id", p1_id);
    json_object_object_add(jp1,"x", p1_x);
    json_object_object_add(jp1,"position", p1_pos);
    json_object_object_add(jp1,"speed", p1_speed);
    json_object_object_add(jp1,"life", p1_life);

    //-------------------------------------------player 2
    json_object* jp2 = json_object_new_object();

    json_object *p2_id = json_object_new_int(player_2.id);
    json_object *p2_pos = json_object_new_int(player_2.pos);
    json_object *p2_x = json_object_new_double(player_2.x);
    json_object *p2_speed = json_object_new_int(player_2.speed);
    json_object *p2_life = json_object_new_int(player_2.life);

    json_object_object_add(jp2,"id", p2_id);
    json_object_object_add(jp2,"x", p2_x);
    json_object_object_add(jp2,"position", p2_pos);
    json_object_object_add(jp2,"speed", p2_speed);
    json_object_object_add(jp2,"life", p2_life);

    //-------------------------------------------player 3
    json_object* jp3 = json_object_new_object();

    json_object *p3_id = json_object_new_int(player_3.id);
    json_object *p3_pos = json_object_new_int(player_3.pos);
    json_object *p3_x = json_object_new_double(player_3.x);
    json_object *p3_speed = json_object_new_int(player_3.speed);
    json_object *p3_life = json_object_new_int(player_3.life);

    json_object_object_add(jp3,"id", p3_id);
    json_object_object_add(jp3,"x", p3_x);
    json_object_object_add(jp3,"position", p3_pos);
    json_object_object_add(jp3,"speed", p3_speed);
    json_object_object_add(jp3,"life", p3_life);

    //-------------------------------------------player 4
    json_object* jp4 = json_object_new_object();

    json_object *p4_id = json_object_new_int(player_4.id);
    json_object *p4_pos = json_object_new_int(player_4.pos);
    json_object *p4_x = json_object_new_double(player_4.x);
    json_object *p4_speed = json_object_new_int(player_4.speed);
    json_object *p4_life = json_object_new_int(player_4.life);

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


    for(int i=0;i<bombas_index;i++ ) {

        if (bombas[i][3] != 0.0) {
            json_object *eleh = json_object_new_object();

            json_object *id = json_object_new_double(bombas[i][0]);
            json_object *hx = json_object_new_double(bombas[i][1]);
            json_object *hpos = json_object_new_double(bombas[i][2]);
            json_object *htime = json_object_new_double(bombas[i][3]);

            json_object_object_add(eleh, "id", id);
            json_object_object_add(eleh, "x", hx);
            json_object_object_add(eleh, "position", hpos);
            json_object_object_add(eleh, "active", htime);

            json_object_array_add(obsArray, eleh);
        }

    }
    //----------------------------------------------Vidas

    json_object *vidasArray = json_object_new_array();

    int vimax = sizeof(vidas)/12;


    for(int i=0;i<vidas_index;i++ ) {

        if (vidas[i][3] != 0) {
            json_object *eleh = json_object_new_object();

            json_object *id = json_object_new_double(vidas[i][0]);
            json_object *hx = json_object_new_double(vidas[i][1]);
            json_object *hpos = json_object_new_double(vidas[i][2]);
            json_object *htime = json_object_new_double(vidas[i][3]);

            json_object_object_add(eleh, "id", id);
            json_object_object_add(eleh, "x", hx);
            json_object_object_add(eleh, "position", hpos);
            json_object_object_add(eleh, "active", htime);

            json_object_array_add(vidasArray, eleh);
        }

    }
    //----------------------------------------------Boost

    json_object *boostArray = json_object_new_array();

    int bimax = sizeof(vidas)/12;


    for(int i=0;i<boost_index+1;i++ ) {

        if (boost[i][3] != 0.0) {
            json_object *eleh = json_object_new_object();

            json_object *id = json_object_new_double(boost[i][0]);
            json_object *hx = json_object_new_double(boost[i][1]);
            json_object *hpos = json_object_new_double(boost[i][2]);
            json_object *htime = json_object_new_double(boost[i][3]);

            json_object_object_add(eleh, "id", id);
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

    json_object_object_add(masterJson,"boost", boostArray);


    printf("SENDED json object created: %s\n", json_object_to_json_string(masterJson));

    return masterJson ;
}

void cleanUpPlayer(const int *player) {

    json_object *cleanJson = json_object_new_object();

    json_object *p_id = json_object_new_int(*player);
    json_object *p_pos = json_object_new_int(0);
    json_object *p_x = json_object_new_double(0);
    json_object *p_speed = json_object_new_int(0);
    json_object *p_life = json_object_new_int(0);

    json_object_object_add(cleanJson, "id", p_id);
    json_object_object_add(cleanJson, "x", p_x);
    json_object_object_add(cleanJson, "position", p_pos);
    json_object_object_add(cleanJson, "speed", p_speed);
    json_object_object_add(cleanJson, "life", p_life);

    json_Parser(cleanJson);
}
