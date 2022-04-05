#ifndef _RENDER_HEADER_
#define _RENDER_HEADER_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <pthread.h>
#include <unistd.h>
#include "main.h"
#include "gest_event.h"

#define FPS_TO_GET 1000/60.0

#define NB_RAYS (screen_width/5)
#define FOV_ANGLE 60

#define ANGLE_INC ((DR * FOV_ANGLE)/(NB_RAYS))

extern int Window_Height;
extern int Window_Width;

extern int DrawHitEffect;


int BouclePrincipale();

void AffichageMap();

#endif /*_RENDER_HEADER_*/