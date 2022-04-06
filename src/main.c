// includes
#include "main.h"


Player_t player;

Ennemy_t ennemy;

// declare 10x10 matrix with only 1s and 0s
int map[10][10] = {
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,1,1},
    {1,0,0,0,0,0,0,0,1,1},
    {1,0,0,0,0,0,1,0,1,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,1,0,0,1,1},
    {1,0,0,0,0,1,0,0,1,1},
    {1,0,0,1,0,1,0,0,1,1},
    {1,1,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1}
};

int screen_height = 1920;
int screen_width = 1080;

int drawincenter = 240;

int QUALITY = LOW;



// function to generate map
void generateMap(int map[MAPSIZE][MAPSIZE]) {
    // generate map
    for(int i = 0; i < MAPSIZE; i++){
        for(int j = 0; j < MAPSIZE; j++){
            map[i][j] = rand() % 2;
        }
    }
    map[0][0] = 0;
}

// function to print map
void printMap(int map[MAPSIZE][MAPSIZE]){
    // print map
    for(int i = 0; i < MAPSIZE; i++){
        for(int j = 0; j < MAPSIZE; j++){
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }
}

float dist(float x1, float y1, float x2, float y2){
    return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
}

float getScalingFactor(float x1, float y1, float x2, float y2){
    return dist(x1, y1, x2, y2) / ((MAPSIZE+2)  * 64);
}

int running;
int GameOption;

int main(){
    srand(time(NULL));

    running = 1;
    GameOption = MENU;

    player.x = 100;
    player.y = 100;
    player.deltax = 0;
    player.deltay = 0;
    player.angle = pi/2;

    // declare ennemy
    ennemy.x = 300;
    ennemy.y = 200;
    ennemy.deltax = 0;
    ennemy.deltay = 0;
    ennemy.angle = pi/2;

    BouclePrincipale();
}

