// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#define pi 3.14159265358979323846
#define pi2 pi/2
#define pi3 3*pi/2
#define DR 0.0174533
#define MAPSIZE 10
#define SENSI 0.1

// include SDL2
#include <SDL2/SDL.h>

// create Player struct
typedef struct Player {
    float x;
    float y;
    float angle;
    float deltax;
    float deltay;
} Player_t;

// define ennemy struct
typedef struct Ennemy {
    float x;
    float y;
    float angle;
    float deltax;
    float deltay;
} Ennemy_t;

// declare 10x10 matrix with only 1s and 0s
int map[10][10] = {
    {1,0,0,0,0,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,1,1},
    {1,0,0,0,0,0,0,0,1,1},
    {1,0,0,0,0,1,1,0,1,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,1,0,0,1,1},
    {1,0,0,0,0,1,0,0,1,1},
    {1,0,0,1,0,1,1,1,1,1},
    {1,1,0,0,0,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1}
};

int screen_height = 1920;
int screen_width = 1080;

int drawincenter = 240;



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
    return dist(x1, y1, x2, y2) / (8 * 64);
}


void drawRay(Player_t * player, int map[MAPSIZE][MAPSIZE], SDL_Renderer *renderer){
    int r, mx, my, mp, dof;
    float rx, ry, ra, xo, yo, distT;
    ra = player->angle - DR * 30;
    if (ra < 0) ra += 2*pi;
    if (ra > 2*pi) ra -= 2*pi;
    for (r = 0; r<960; r++){
        // check horizontal line
        
        dof = 0;
        float disH=100000, hx = player->x, hy = player->y;
        float aTan = -1/tan(ra);
        if (ra>pi){ // looking up
            ry = (((int)player->y>>6)<<6) - 0.0001;
            rx = (player->y - ry) * aTan + player->x;
            yo = -64;
            xo = -yo*aTan;
        }
        if (ra<pi){ // looking down
            ry = (((int)player->y>>6)<<6) + 64;
            rx = (player->y - ry) * aTan + player->x;
            yo = 64;
            xo = -yo*aTan;
        }
        if (ra==pi){ // looking horizontal
            ry = player->y;
            rx = player->x;
            dof = 8;
        }
        while (dof < 8){
            mx = (int)rx>>6;
            my = (int)ry>>6;
            if (mx>=0 && mx<MAPSIZE && my>=0 && my<MAPSIZE && map[my][mx]==1){
                hx = rx;
                hy = ry;
                disH = dist(player->x, player->y, hx, hy);
                dof = 8;
            }
            else {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }
        

        // check vertical line
        dof = 0;
        float disV=100000, vx = player->x, vy = player->y;
        float nTan = -tan(ra);
        if (ra>pi2 && ra < pi3){ // looking left
            rx = (((int)player->x>>6)<<6) - 0.0001;
            ry = (player->x - rx) * nTan + player->y;
            xo = -64;
            yo = -xo*nTan;
        }
        if (ra<pi2 || ra>pi3){ // looking right
            rx = (((int)player->x>>6)<<6) + 64;
            ry = (player->x - rx) * nTan + player->y;
            xo = 64;
            yo = -xo*nTan;
        }
        if (ra==pi || ra == 0){ // looking horizontal
            ry = player->y;
            rx = player->x;
            dof = 8;
        }
        while (dof < 8){
            mx = (int)rx>>6;
            my = (int)ry>>6;
            if (mx>=0 && mx<MAPSIZE && my>=0 && my<MAPSIZE && map[my][mx]==1){
                vx = rx;
                vy = ry;
                disV = dist(player->x, player->y, vx, vy);
                dof = 8;
            }
            else {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }
        if (disH<disV){
            rx = hx;
            ry = hy;
            distT = disH;
        }
        else {
            rx = vx;
            ry = vy;
            distT = disV;
        }
        // draw line
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, 1066 + player->x, player->y,1066+ rx, ry);
        ra+=DR/16;
        if (ra < 0) ra += 2*pi;
        if (ra > 2*pi) ra -= 2*pi;

        // draw collumn
        float ca = player->angle - ra;
        if (ca < 0) ca += 2*pi;
        if (ca > 2*pi) ca -= 2*pi;
        distT = distT * cos(ca);
        float lineH = (MAPSIZE * 480) / distT;
        if (lineH > screen_width) lineH = screen_width;
        float factor = getScalingFactor(player->x, player->y, rx, ry);
        // sdl draw rect with width 1 and height distT
        int width = screen_height/(960);
        // draw ground
        SDL_SetRenderDrawColor(renderer, 50, 55, 0, 255);
        SDL_Rect ground = {r * width, drawincenter - (int)lineH, width, screen_height - drawincenter + (int)lineH};
        SDL_RenderFillRect(renderer, &ground);
        SDL_Rect rect = {r * width, drawincenter - (int)lineH, width, (int)(screen_width * lineH/200)};
        // draw rect
        if (disH < disV){
            SDL_SetRenderDrawColor(renderer, 255 * (1 - factor), 255 * (1 - factor), 255 * (1 - factor) , 255);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 150 * (1 - factor), 150 * (1 - factor), 150 * (1 - factor), 255);
        }
        SDL_RenderFillRect(renderer, &rect);
        // draw sky
        SDL_SetRenderDrawColor(renderer, 50, 55, 255, 255);
        SDL_Rect sky = {r * width, 0, width, drawincenter - (int)lineH};
        SDL_RenderFillRect(renderer, &sky);

    }
}

// draw ennemy 
void drawEnnemyonMap(Ennemy_t * ennemy, Player_t * player, SDL_Renderer *renderer){
    // define vector of ennemy relative to player
    float ennemy_angle = atan2(ennemy->y - player->y, ennemy->x - player->x);
    float ennemy_dist = dist(ennemy->x, ennemy->y, player->x, player->y);
    if (ennemy_angle < 0) ennemy_angle += 2*pi;
    if (ennemy_angle > 2*pi) ennemy_angle -= 2*pi;
    if (ennemy_angle >= player->angle - 30 * DR && ennemy_angle <= player->angle + 30 * DR){
        // draw ennemy
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_Rect rect = {ennemy->x - ennemy_dist * cos(ennemy_angle), ennemy->y - ennemy_dist * sin(ennemy_angle), 10, 10};
        SDL_RenderFillRect(renderer, &rect);
    }
}

void drawEnnemy(Ennemy_t * ennemy, Player_t * player, SDL_Renderer *renderer){
    int sens = 1;
    float ennemy_angle = atan2(ennemy->y - player->y, ennemy->x - player->x);
    if (ennemy_angle < 0) ennemy_angle += 2*pi;
    if (ennemy_angle > 2*pi) ennemy_angle -= 2*pi;
    float ennemy_dist = dist(ennemy->x, ennemy->y, player->x, player->y);
    float ennemy_dist_x;
    float ennemy_dist_y;
    float tot;
    int ennemy_width = 10;
    

    ennemy_dist_x = ennemy_dist * cos(ennemy_angle - player->angle);

    if (ennemy_angle >= player->angle){
        ennemy_dist_y = ennemy_dist * sin(ennemy_angle - player->angle);
    }

    else {
        ennemy_dist_y = - ennemy_dist * sin(ennemy_angle - player->angle);
        sens = -1;
    }

    tot = sqrt(3) * ennemy_dist_x;
    
    float base_triangle = 2 * ennemy_dist_x / sqrt(3);

    if (ennemy_angle >= player->angle - 30 * DR && ennemy_angle <= player->angle + 30 * DR){
        float distance = dist(player->x, player->y, ennemy->x, ennemy->y);
        float ennemy_length = (MAPSIZE * 480) / distance;

        if (ennemy_length > screen_height/5) ennemy_length = screen_height/5;

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect rect = {1080 + 2 * sens * (screen_width * ennemy_dist_y)/base_triangle, drawincenter + ennemy->y - ennemy_dist * sin(ennemy_angle), ennemy_width * screen_width / tot, ennemy_length};
        SDL_RenderFillRect(renderer, &rect);
    }
}

// function to draw the map
void drawMap(int map[MAPSIZE][MAPSIZE], SDL_Renderer *renderer){
    // draw map
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for(int i = 0; i < MAPSIZE; i++){
        for(int j = 0; j < MAPSIZE; j++){
            if (map[i][j] == 1){
                SDL_Rect rect = {1066 + j*64, i*64, 64, 64};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
} 


int main(){
    // random seed
    srand(time(NULL));

    // create window fullscreen
    SDL_Window *window = SDL_CreateWindow("Raycasting", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_height, screen_width, SDL_WINDOW_FULLSCREEN);
    // create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    // create int 2d array map
    //int map[5][5];
    //generateMap(map);
    printMap(map);
    //SDL_SetRelativeMouseMode(SDL_TRUE);
    //hide cursor
    SDL_ShowCursor(SDL_DISABLE);
    //declare player
    Player_t player;
    player.x = 100;
    player.y = 100;
    player.deltax = 0;
    player.deltay = 0;
    player.angle = pi/2;

    // declare ennemy
    Ennemy_t ennemy;
    ennemy.x = 200;
    ennemy.y = 200;
    ennemy.deltax = 0;
    ennemy.deltay = 0;
    ennemy.angle = pi/2;



    // get input
    // main loop
    int start = 0;
    int time;
    while(1){
        start = SDL_GetTicks();
        // get input
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    exit(0);
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        case SDLK_z:
                            player.x += player.deltax;
                            player.y += player.deltay;
                            continue;
                        case SDLK_d:
                            // move right
                            player.x -= sin(player.angle);
                            player.y += cos(player.angle);
                            

                            continue;
                        case SDLK_s:
                            player.x -= player.deltax;
                            player.y -= player.deltay;
                            continue;
                        case SDLK_q:
                            // move left
                            player.x += sin(player.angle);
                            player.y -= cos(player.angle);
                            continue;
                        default:
                            break;
                    }
                    break;
                // mouse movement
                case SDL_MOUSEMOTION:
                    // if mouse moved to right
                    if (event.motion.xrel > 0){
                        player.angle += 0.1 * SENSI;
                        if (player.angle > 2*pi) player.angle -= 2*pi;
                        player.deltax = cos(player.angle);
                        player.deltay = sin(player.angle);
                    }
                    // if mouse move to left
                    if (event.motion.xrel < 0){
                        player.angle -= 0.1 * SENSI;
                        if (player.angle < 0) {player.angle += 2*pi;}
                        player.deltax = cos(player.angle);
                        player.deltay = sin(player.angle);
                    }

                    // if mouse moved to up
                    if (event.motion.yrel > 0){
                        drawincenter -= 5;
                    }
                    // if mouse moved to down
                    if (event.motion.yrel < 0){
                        drawincenter += 5;
                    }

                    // cancel mouse movement
                    SDL_WarpMouseInWindow(window, screen_height/2, screen_width/2);


                    break;

                case SDL_KEYUP:
                    switch(event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            exit(0);
                            break;
                        default:
                            break;
                    }
                    break;
            }
        }
        // move player
        //movePlayer(map, &player, direction);
        // clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        // draw map all screen
        
        // draw ray
        drawRay(&player, map, renderer);
        drawMap(map, renderer);
        drawEnnemy(&ennemy, &player, renderer);

                // draw player
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect rect = {1066 + player.x, player.y, 10, 10};
        SDL_RenderFillRect(renderer, &rect);

        // draw ennemy
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect rect2 = {1066 + ennemy.x, ennemy.y, 10, 10};
        SDL_RenderFillRect(renderer, &rect2);
        
        // update screen
        SDL_RenderPresent(renderer);
        time = SDL_GetTicks() - start;
        if (time < 0) continue;
        int sleepTime = (1000/60) - time;
        if (sleepTime > 0) SDL_Delay(sleepTime);
    }
}

