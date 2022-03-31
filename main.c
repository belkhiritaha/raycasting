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
        //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        //SDL_RenderDrawLine(renderer, 1066 + player->x, player->y,1066+ rx, ry);
        ra+=DR/16;
        if (ra < 0) ra += 2*pi;
        if (ra > 2*pi) ra -= 2*pi;

        // draw collumn
        float ca = player->angle - ra;
        if (ca < 0) ca += 2*pi;
        if (ca > 2*pi) ca -= 2*pi;
        distT = distT * cos(ca);
        float lineH = (MAPSIZE * 480) / distT;
        if (lineH > 300) lineH = 300;
        float factor = getScalingFactor(player->x, player->y, rx, ry);
        // sdl draw rect with width 1 and height distT
        int width = 1920/(960);
        // draw ground
        SDL_SetRenderDrawColor(renderer, 50, 55, 0, 255);
        SDL_Rect ground = {r * width, 240, width,1080 - (int)lineH};
        SDL_RenderFillRect(renderer, &ground);
        SDL_Rect rect = {r * width, 240 - (int)lineH, width, (int)(1080 * lineH/300)};
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
        SDL_Rect sky = {r * width, 0, width,240 - (int)lineH};
        SDL_RenderFillRect(renderer, &sky);
        


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
    SDL_Window *window = SDL_CreateWindow("Raycasting", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_FULLSCREEN);
    // create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    // create int 2d array map
    //int map[5][5];
    //generateMap(map);
    printMap(map);
    SDL_ShowCursor(SDL_DISABLE);
    //set relative mouse mode
    //SDL_SetRelativeMouseMode(SDL_TRUE);
    //declare player
    Player_t player;
    player.x = 100;
    player.y = 100;
    player.deltax = 0;
    player.deltay = 0;
    player.angle = pi/2;
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
                            player.x -= -sin(player.angle);
                            player.y += -cos(player.angle);
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
                        //player.x += player.deltax;
                        //player.y += player.deltay;
                    }

                    // keep mouse centered
                    SDL_WarpMouseInWindow(window, 960, 540);

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
        //drawMap(map, renderer);

        // draw player
        //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        //SDL_Rect rect = {1066 + player.x, player.y, 10, 10};
        //SDL_RenderFillRect(renderer, &rect);
        
        // draw ray
        drawRay(&player, map, renderer);

        // update screen
        SDL_RenderPresent(renderer);
        time = SDL_GetTicks() - start;
        if (time < 0) continue;
        int sleepTime = (1000/60) - time;
        if (sleepTime > 0) SDL_Delay(sleepTime);
    }
}

