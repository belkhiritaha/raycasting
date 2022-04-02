#include "gest_event.h"

int Keys[NB_KEYS];

void * gestInputOnTerrain(SDL_Renderer * renderer){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    exit(0);
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        case SDLK_z:
                            Keys[0] = 1;
                            player.x += player.deltax;
                            player.y += player.deltay;
                            continue;
                        case SDLK_d:
                            Keys[1] = 1;
                            player.x -= sin(player.angle);
                            player.y += cos(player.angle);
                            continue;
                        case SDLK_s:
                            Keys[2] = 1;
                            player.x -= player.deltax;
                            player.y -= player.deltay;
                            continue;
                        case SDLK_q:
                            Keys[3] = 1;
                            // move left
                            player.x += sin(player.angle);
                            player.y -= cos(player.angle);
                            continue;
                        default:
                            continue;
                    }
                    continue;
                // mouse movement
                case SDL_MOUSEMOTION:
                    // if mouse moved to right
                    if (event.motion.xrel > 0){
                        player.angle += 0.2 * SENSI;
                        if (player.angle > 2*pi) player.angle -= 2*pi;
                        player.deltax = cos(player.angle);
                        player.deltay = sin(player.angle);
                    }
                    // if mouse move to left
                    if (event.motion.xrel < 0){
                        player.angle -= 0.2 * SENSI;
                        if (player.angle < 0) {player.angle += 2*pi;}
                        player.deltax = cos(player.angle);
                        player.deltay = sin(player.angle);
                    }

                    // if mouse moved to up
                    if (event.motion.yrel > 0){
                        drawincenter -= 250 * SENSI;
                    }
                    // if mouse moved to down
                    if (event.motion.yrel < 0){
                        drawincenter += 250 * SENSI;
                    }

                    SDL_WarpMouseGlobal(screen_width/2, screen_height/2);

                    continue;

                case SDL_KEYUP:
                    switch(event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            exit(0);
                            break;
                        case SDLK_z:
                            Keys[0] = 0;
                            continue;
                        case SDLK_d:
                            Keys[1] = 0;
                            continue;
                        case SDLK_s:
                            Keys[2] = 0;
                            continue;
                        case SDLK_q:
                            Keys[3] = 0;
                            continue;
                        default:
                            continue;
                    }

                    continue;
            }
        }
    SDL_Delay(1);
    return NULL;
}

void gestMenu(){
    SDL_Event event;
    while (SDL_PollEvent(&event)){
            switch(event.type)
            {   
                case SDL_MOUSEBUTTONUP:
                    GameOption = GAMERUNNING;
                    continue;

                default:
                    break;


                case SDL_WINDOWEVENT:
                    if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
                        screen_height = event.window.data2;
                        screen_width = event.window.data1;
                    }
                    break;

                case SDL_QUIT:
                    printf("end game");
                    running = 0;
                    break;
                
                case SDL_KEYUP:
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_x:
                        printf("end game");
                        running = 0;
                        continue;
                    
                    default:
                        continue;
                    }
            }
    }
    SDL_Delay(1);
}