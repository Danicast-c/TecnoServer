//
// Created by Daniel Castro on 16/6/18.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include "datamanager.h"

char player_1[5]={0,0,0,0,0};
char player_2[5]={0,0,0,0,0};
char player_3[5]={0,0,0,0,0};
char player_4[5]={0,0,0,0,0};

void data_toSend (){

    int huecos [3][3] = {{124,4,500},{174,7,500},{217,10,500}};


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
    json_object *p1_lifes = json_object_new_int(player_1[4]);

    json_object_object_add(jp1,"id", p1_id);
    json_object_object_add(jp1,"x", p1_x);
    json_object_object_add(jp1,"position", p1_pos);
    json_object_object_add(jp1,"speed", p1_speed);
    json_object_object_add(jp1,"lives", p1_lifes);

    //-------------------------------------------player 2
    json_object* jp2 = json_object_new_object();

    json_object *p2_id = json_object_new_int(player_2[0]);
    json_object *p2_pos = json_object_new_int(player_2[1]);
    json_object *p2_x = json_object_new_int(player_2[2]);
    json_object *p2_speed = json_object_new_int(player_2[3]);
    json_object *p2_lifes = json_object_new_int(player_2[4]);

    json_object_object_add(jp2,"id", p2_id);
    json_object_object_add(jp2,"x", p2_x);
    json_object_object_add(jp2,"position", p2_pos);
    json_object_object_add(jp2,"speed", p2_speed);
    json_object_object_add(jp2,"lives", p2_lifes);

    //-------------------------------------------player 3
    json_object* jp3 = json_object_new_object();

    json_object *p3_id = json_object_new_int(player_3[0]);
    json_object *p3_pos = json_object_new_int(player_3[1]);
    json_object *p3_x = json_object_new_int(player_3[2]);
    json_object *p3_speed = json_object_new_int(player_3[3]);
    json_object *p3_lifes = json_object_new_int(player_3[4]);

    json_object_object_add(jp3,"id", p3_id);
    json_object_object_add(jp3,"x", p3_x);
    json_object_object_add(jp3,"position", p3_pos);
    json_object_object_add(jp3,"speed", p3_speed);
    json_object_object_add(jp3,"lives", p3_lifes);

    //-------------------------------------------player 4
    json_object* jp4 = json_object_new_object();

    json_object *p4_id = json_object_new_int(player_4[0]);
    json_object *p4_pos = json_object_new_int(player_4[1]);
    json_object *p4_x = json_object_new_int(player_4[2]);
    json_object *p4_speed = json_object_new_int(player_4[3]);
    json_object *p4_lifes = json_object_new_int(player_4[4]);

    json_object_object_add(jp4,"id", p4_id);
    json_object_object_add(jp4,"x", p4_x);
    json_object_object_add(jp4,"position", p4_pos);
    json_object_object_add(jp4,"speed", p4_speed);
    json_object_object_add(jp4,"lives", p4_lifes);

    //----------------------------------------------Array de jugadores
    json_object *playersArray = json_object_new_array();

    json_object_array_add(playersArray,jp1);
    json_object_array_add(playersArray,jp2);
    json_object_array_add(playersArray,jp3);
    json_object_array_add(playersArray,jp4);

    //----------------------------------------------Obstaculos

    json_object *obsArray = json_object_new_array();

    int imax = sizeof(huecos)/12;

    for(int i=0;i<imax;i++ ){
        json_object* eleh = json_object_new_object();

        json_object *hpos = json_object_new_int(huecos[i][0]);
        json_object *hx = json_object_new_int(huecos[i][1]);
        json_object *htime = json_object_new_int(huecos[i][2]);

        json_object_object_add(eleh,"Pos", hpos);
        json_object_object_add(eleh,"x", hx);
        json_object_object_add(eleh,"time", htime);

        json_object_array_add(obsArray,eleh);

    }


    //----------------------------------------------Array de jugadores
    json_object *osbstaclesArray = json_object_new_array();

    json_object_array_add(osbstaclesArray,jp1);
    json_object_array_add(osbstaclesArray,jp2);
    json_object_array_add(osbstaclesArray,jp3);
    json_object_array_add(osbstaclesArray,jp4);


    //-----------------------------------------------add a master

    json_object_object_add(masterJson,"players", playersArray);


    json_object_object_add(masterJson,"huecos", obsArray);



    printf ("The json object created: %s",json_object_to_json_string(masterJson));
}
