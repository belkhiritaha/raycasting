#include "render.h"

SDL_Window * window = NULL;
SDL_Renderer * renderer = NULL;


int Window_Height;
int Window_Width;

int DrawHitEffect = 0;

TTF_Font *RobotoFont;

SDL_Color Color = {255, 255, 255};

SDL_Rect rect;
SDL_Rect ground;
SDL_Rect sky;

// ennemy texture and surface
SDL_Texture *EnnemyTexture = NULL;
SDL_Surface *EnnemySurface = NULL;
SDL_Surface *WallSurface = NULL;
SDL_Texture *WallTexture = NULL;



void CreateWindow(){
    SDL_DisplayMode ScreenDimension;

    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("Couldn't create window.");
        exit(EXIT_FAILURE);
    }

    SDL_GetCurrentDisplayMode(0, &ScreenDimension);

    window = SDL_CreateWindow("Explorateur 3000", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ScreenDimension.w, ScreenDimension.h, SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);

    if (window == NULL){
        printf("Couldn't create window");
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL){
        printf("Couldn't create renderer.");
        exit(EXIT_FAILURE);
    }


    if (TTF_Init() == -1)
    {
        exit(EXIT_FAILURE);
    }


    RobotoFont = TTF_OpenFont("Roboto-Black.ttf", 50);

    SDL_GetWindowSize(window, &screen_width, &screen_height);
}

void drawRay(Player_t * player, int map[MAPSIZE][MAPSIZE], SDL_Renderer *renderer){
    float htexture;
    int r, mx, my, dof;
    float rx, ry, xo, yo, distT;
    double ra = player->angle - DR * FOV_ANGLE/2;
    if (ra < 0) ra += 2*pi;
    if (ra > 2*pi) ra -= 2*pi;
    for (r = 0; r<NB_RAYS; r++){
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
            dof = MAPSIZE;
        }
        while (dof < MAPSIZE){
            mx = (int)rx>>6;
            my = (int)ry>>6;
            if (mx>=0 && mx<MAPSIZE && my>=0 && my<MAPSIZE && map[my][mx]==1){ 
                hx = rx;
                hy = ry;
                disH = dist(player->x, player->y, hx, hy);
                dof = MAPSIZE;
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
            dof = MAPSIZE;
        }
        while (dof < MAPSIZE){
            mx = (int)rx>>6;
            my = (int)ry>>6;
            if (mx>=0 && mx<MAPSIZE && my>=0 && my<MAPSIZE && map[my][mx]==1){
                vx = rx;
                vy = ry;
                disV = dist(player->x, player->y, vx, vy);
                dof = MAPSIZE;
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
        ra = ra + ANGLE_INC;
        if (ra < 0) ra += 2*pi;
        if (ra > 2*pi) ra -= 2*pi;
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderDrawLine(renderer, 1066 + player->x, player->y,1066+ rx, ry);

        // draw collumn
        float ca = player->angle - ra;
        if (ca < 0) ca += 2*pi;
        if (ca > 2*pi) ca -= 2*pi;
        distT = distT * cos(ca);
        float lineH = (MAPSIZE * NB_RAYS) / distT;
        if (lineH > screen_width) lineH = screen_width;
        float factor = getScalingFactor(player->x, player->y, rx, ry);
        // sdl draw rect with width 1 and height distT
        int width = screen_width/(NB_RAYS);

        rect.x = r * width;
        rect.y = drawincenter - (int)lineH;
        rect.w = width;
        rect.h = (int)(screen_height * lineH/200) * screen_width/NB_RAYS;
        // draw rect
        if (disH < disV){
            SDL_SetTextureColorMod(WallTexture, 255 * (1 - factor), 255 * (1 - factor), 255 * (1 - factor));
            htexture = (int)(rx/2.0)%32;
            //SDL_SetRenderDrawColor(renderer, 255 * (1 - factor), 255 * (1 - factor), 255 * (1 - factor) , 255);
        }
        else {
            //SDL_SetRenderDrawColor(renderer, 155 * (1 - factor), 155 * (1 - factor), 155 * (1 - factor) , 255);
            htexture = (int)(ry/2.0)%32;
            //SDL_SetTextureColorMod(WallTexture, 150 * (1 - factor), 150 * (1 - factor), 150 * (1 - factor));
        }
        SDL_Rect dstrect = {htexture,0,1,32};
        SDL_RenderCopy(renderer, WallTexture, &dstrect, &rect);


        //SDL_RenderFillRect(renderer, &rect);


    }
}

// draw crosshair
void drawCrosshair(Player_t * player, SDL_Renderer *renderer){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, screen_width/2, screen_height/2-10, screen_width/2, screen_height/2 + 10);
    SDL_RenderDrawLine(renderer, screen_width/2-10, screen_height/2, screen_width/2 + 10, screen_height/2);
}


// draw ennemy 
void drawEnnemyonMap(Ennemy_t * ennemy, Player_t * player, SDL_Renderer *renderer){
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
        float ennemy_length = 2 * (MAPSIZE * 480) / distance;
        float draw_width = ennemy_width * screen_width / tot;
        float draw_y =  drawincenter + ennemy->y - ennemy_dist * sin(ennemy_angle) - ennemy_length/2; if (draw_y > screen_width) draw_y = screen_width;

        //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect rect = {screen_width/2 + sens * (screen_width * ennemy_dist_y)/base_triangle, draw_y, draw_width, 2 *ennemy_length};
        //SDL_RenderFillRect(renderer, &rect);
        SDL_RenderCopy(renderer, EnnemyTexture, NULL, &rect);
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
                    // draw player
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect = {1066 + player.x, player.y, 10, 10};
    SDL_RenderFillRect(renderer, &rect);

        // draw ennemy
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect2 = {1066 + ennemy.x, ennemy.y, 10, 10};
    SDL_RenderFillRect(renderer, &rect2);

} 

void DrawFPS(float fps){
    char ScoreString[10];
    sprintf(ScoreString,"%d",(int)fps);
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(RobotoFont, ScoreString, Color); 
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Rect Message_rect;
    Message_rect.w = 100;
    Message_rect.h = 100;
    Message_rect.x = 0; 
    Message_rect.y = 0;  
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
}

void AffichageMenu(){
    rect.w = Window_Width;
    rect.h = Window_Height/2;
    rect.x = 0; 
    rect.y = Window_Height/4;  
    //blur_texture = SDL_CreateTextureFromSurface(renderer, GameName);
    //SDL_RenderCopy(renderer, blur_texture , NULL, &rect);
    SDL_RenderPresent(renderer);
    //SDL_DestroyTexture(blur_texture);
}

void drawSkyGround(){
            // draw ground
        SDL_SetRenderDrawColor(renderer, 50, 55, 0, 255);
        ground.x = 0;
        ground.y = 0;
        ground.w = screen_width;
        ground.h = screen_height;
        SDL_RenderFillRect(renderer, &ground);
                // draw sky
        SDL_SetRenderDrawColor(renderer, 50, 55, 255, 255);
        sky.x = 0;
        sky.y = 0;
        sky.w = screen_width;
        sky.h = drawincenter;
        SDL_RenderFillRect(renderer, &sky);
}



void AffichageNormal(float fps){
    SDL_RenderClear(renderer);
    drawSkyGround();
    drawRay(&player, map, renderer);
    drawEnnemy(&ennemy, &player, renderer);
    drawCrosshair(&player, renderer);
    DrawFPS(fps);
    drawMap(map, renderer);
    SDL_RenderPresent(renderer);
}

void *BoucleGestInput(void *arg){
    while(GameOption){
      switch(GameOption){
          case GAMERUNNING : gestInputOnTerrain(renderer);break;
          case MENU : gestMenu();break;
          case END_SCREEN : gestMenu();break;
          default:printf("game option fault");break;
      }
    }
    return NULL;
}

int BouclePrincipale(){
    CreateWindow();

    EnnemySurface = IMG_Load("character.png");
    WallSurface = IMG_Load("../Res/texture1.png");

    EnnemyTexture = SDL_CreateTextureFromSurface(renderer, EnnemySurface);
    WallTexture = SDL_CreateTextureFromSurface(renderer, WallSurface);
    
    unsigned int a = SDL_GetTicks();
    unsigned int b = SDL_GetTicks();
    double delta = 0;


    pthread_t threadGest;
    int RetourDuThreadGest = pthread_create(&threadGest, NULL, BoucleGestInput, NULL);
    if (RetourDuThreadGest){
        return 1;
    }

    //GameOption = ON_MAP;


    while (running)
    {
        a = SDL_GetTicks();
        delta = a - b;
        if (delta > FPS_TO_GET){
            //printf("fps : %f\n", 1000/delta);
            b = a;
            switch (GameOption)
            {
                case GAMERUNNING:
                    AffichageNormal(1000/delta);
                    break;

                case MENU:
                    //AffichageMenu();
                    break;

                case END_SCREEN:
                    //AffichageEnd();
                    break;

                default:
                    //printf("gameoption fault");
                    exit(EXIT_FAILURE);
            }
        }
        else{

            // sleep the thread
            usleep(10000 * (FPS_TO_GET - delta));
        } 

    }
    SDL_DestroyTexture(WallTexture);
    SDL_DestroyTexture(EnnemyTexture);
    return 0;
}