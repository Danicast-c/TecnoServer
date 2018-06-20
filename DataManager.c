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

double bombs[50][4] = {{1, 5, 500,   1},
                       {2, 2, 2000,  1},
                       {3, 3, 12000, 1},
                       {4, 7, 24000, 1},
                       {5, 1, 700,   1},
                       {6, 8, 2700,  1},
                       {7, 6, 4278,  1}};
int bombs_index = 7;

double lives[50][4] = {{1, 7, 550,  1},
                       {2,2, 2957,  1},
                       {3,5, 6973, 1},
                       {4,8, 13570, 1},
                       {5,7, 15000, 1},
                       {6,2, 20000, 1},
                       {7,1, 23000, 1},
                       {8,5, 27000, 1}};
int life_index = 8;

double speed[50][4] = {{ 1, 5, 600,  1},
                       {2,8, 2100,  1},
                       {3,9, 838, 1},
                       {4,9, 4000, 1},
                       {5,9, 6000, 1},
                       {6,9, 10000, 1},
                       {7,9, 15000, 1},
                        {8,5, 20000, 1}};

int speed_index = 8;

/**
 * Funcion usada para que el servidor cree las bombas
 * Almacena las nuevas bombas en el array bombas
 * @param x    Es la posicion en x que tendra la bomba asi el cliente sabra donde pintarla, va del 1 al 10
 * @param pos   Es la posicion respecto al largo de la pista que tendra la bomba, va de 0 a 100
 */
void bombAdder(int x, int pos) {

    if (0<= x <= 10 && 0<=pos){
        double xToSend = x;
        double posToSend = pos;


        bombs[bombs_index][0] = bombs_index + 1;
        bombs[bombs_index][1] = xToSend;
        bombs[bombs_index][2] = posToSend;
        bombs[bombs_index][3] = 1;
        bombs_index += 1;



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
void lifeAdder(int x, int pos) {

    if (0<= x <= 10 && 0<=pos){
        double xToSend = x;
        double posToSend = pos;

        lives[life_index][0] = life_index + 1;
        lives[life_index][1] = xToSend;
        lives[life_index][2] = posToSend;
        lives[life_index][3] = 1;
        life_index = life_index + 1;


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
void speedAdder(int x, int pos) {

    if (0<= x <= 10 && 0<=pos){
        double xToSend = x;
        double posToSend = pos;

        printf("antes x: %f \n", speed[speed_index][1]);

        speed[speed_index + 1][0] = speed_index + 1;
        speed[speed_index + 1][1] = xToSend;
        speed[speed_index + 1][2] = posToSend;
        speed[speed_index + 1][3] = 1;
        speed_index = speed_index + 1;

        printf("desp x: %f \n", speed[speed_index][1]);

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
void bombRemover(int id) {
    bombs[id - 1][3] = 0;
}

void lifeRemover(int id) {
    lives[id - 1][3] = 0;
}

void speedRemover(int id) {
    speed[id - 1][3] = 0;
}


int json_Parser(json_object *jplayer) {

    pthread_mutex_lock(&lock);

    int resultado = 0;

   // printf ("The json object created: %s \n",json_object_to_json_string(jplayer));

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
            printf("Error in jsonParser\n");
            resultado = -1;

    }

    free(jplayer);

    pthread_mutex_unlock(&lock);

    return resultado;
}



/**
 *Funcion que copila toda la informacion actual de los diferentes jugadores, bombas, vidas, boost
 * y se encarga de almacenarlos en un solo json.
 * @return retorna el msterJson con toda la informacion necesaria
 */
json_object *dataToSend() {



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

    //----------------------------------------------Player Array
    json_object *playersArray = json_object_new_array();

    json_object_array_add(playersArray,jp1);
    json_object_array_add(playersArray,jp2);
    json_object_array_add(playersArray,jp3);
    json_object_array_add(playersArray,jp4);

    //----------------------------------------------Bombs

    json_object *bombsArray = json_object_new_array();

    int imax = sizeof(bombs) / 12;


    for (int i = 0; i < bombs_index; i++) {

        if (bombs[i][3] != 0.0) {
            json_object *eleh = json_object_new_object();

            json_object *id = json_object_new_double(bombs[i][0]);
            json_object *hx = json_object_new_double(bombs[i][1]);
            json_object *hpos = json_object_new_double(bombs[i][2]);
            json_object *htime = json_object_new_double(bombs[i][3]);

            json_object_object_add(eleh, "id", id);
            json_object_object_add(eleh, "x", hx);
            json_object_object_add(eleh, "position", hpos);
            json_object_object_add(eleh, "active", htime);

            json_object_array_add(bombsArray, eleh);
        }

    }
    //----------------------------------------------Lives

    json_object *livesArray = json_object_new_array();

    int vimax = sizeof(lives) / 12;


    for (int i = 0; i < life_index; i++) {

        if (lives[i][3] != 0) {
            json_object *eleh = json_object_new_object();

            json_object *id = json_object_new_double(lives[i][0]);
            json_object *hx = json_object_new_double(lives[i][1]);
            json_object *hpos = json_object_new_double(lives[i][2]);
            json_object *htime = json_object_new_double(lives[i][3]);

            json_object_object_add(eleh, "id", id);
            json_object_object_add(eleh, "x", hx);
            json_object_object_add(eleh, "position", hpos);
            json_object_object_add(eleh, "active", htime);

            json_object_array_add(livesArray, eleh);
        }

    }
    //----------------------------------------------Speed

    json_object *speedArray = json_object_new_array();

    int bimax = sizeof(lives) / 12;


    for (int i = 0; i < speed_index + 1; i++) {

        if (speed[i][3] != 0.0) {
            json_object *eleh = json_object_new_object();

            json_object *id = json_object_new_double(speed[i][0]);
            json_object *hx = json_object_new_double(speed[i][1]);
            json_object *hpos = json_object_new_double(speed[i][2]);
            json_object *htime = json_object_new_double(speed[i][3]);

            json_object_object_add(eleh, "id", id);
            json_object_object_add(eleh, "x", hx);
            json_object_object_add(eleh, "position", hpos);
            json_object_object_add(eleh, "active", htime);

            json_object_array_add(speedArray, eleh);
        }

    }


    //----------------------------------------------Array de jugadores
    json_object *obstaclesArray = json_object_new_array();

    json_object_array_add(obstaclesArray, jp1);
    json_object_array_add(obstaclesArray, jp2);
    json_object_array_add(obstaclesArray, jp3);
    json_object_array_add(obstaclesArray, jp4);


    //-----------------------------------------------add a master

    json_object_object_add(masterJson,"players", playersArray);


    json_object_object_add(masterJson, "bombs", bombsArray);

    json_object_object_add(masterJson, "lives", livesArray);

    json_object_object_add(masterJson, "speed", speedArray);


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
