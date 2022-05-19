// includes
#include "main.h"


Player_t player;
Ennemy_t * ennemy_head;


// declare 50x50 matrix with only 1s and 0s
int map[50][50];

// write map to map.txt
void writeMap(int map[][MAPSIZE])
{
    FILE *fp;
    fp = fopen("map.txt", "w");
    for(int i = 0; i < MAPSIZE; i++)
    {
        for(int j = 0; j < MAPSIZE; j++)
        {
            fprintf(fp, "%d ", map[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

// read map from map.txt
void readMap(int map[][MAPSIZE])
{
    FILE *fp;
    fp = fopen("map.txt", "r");
    for(int i = 0; i < MAPSIZE; i++)
    {
        for(int j = 0; j < MAPSIZE; j++)
        {
            fscanf(fp, "%d", &map[i][j]);
        }
    }
    fclose(fp);
}

int screen_height = 1920;
int screen_width = 1080;

int drawincenter = 240;

int QUALITY = ULTRA;



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
    int pos_x = player.x/BLOCK_SIZE;
    int pos_y = player.y/BLOCK_SIZE;
    for(int i = 0; i < MAPSIZE; i++){
        for(int j = 0; j < MAPSIZE; j++){
            if (j == pos_x && i == pos_y){
                printf("P ");
            }
            else{
                printf("%d ", map[i][j]);
            }
        }
        printf("\n");
    }
}

float dist(float x1, float y1, float x2, float y2){
    return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
}

float getScalingFactor(float x1, float y1, float x2, float y2){
    return dist(x1, y1, x2, y2) / (MAPSIZE * sqrt(2)  * BLOCK_SIZE);
}

int running;
int GameOption;

void initEnnemy(Ennemy_t * ennemy){
    ennemy->x = 500;
    ennemy->y = 500;
    ennemy->angle = 0;
    ennemy->deltax = 0;
    ennemy->deltay = 0;
}

int main(){
    srand(time(NULL));

    running = 1;
    GameOption = MENU;


    player.x = 700;
    player.y = 500;
    player.deltax = 0;
    player.deltay = 0;
    player.angle = pi/2;

    // declare ennemy
    ennemy_head = (Ennemy_t *) malloc(sizeof(Ennemy_t));
    initEnnemy(ennemy_head);

    //writeMap(map);

    // read map
    readMap(map);

    printMap(map);

    BouclePrincipale();
}

