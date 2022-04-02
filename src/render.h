#ifndef _RENDER_HEADER_
#define _RENDER_HEADER_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <pthread.h>
#include <unistd.h>
#include "main.h"
#include "gest_event.h"

#define NB_RAYS (screen_width/4)
#define FOV_ANGLE 60

extern int Window_Height;
extern int Window_Width;

extern int DrawHitEffect;

int BouclePrincipale();

void AffichageMap();

#endif /*_RENDER_HEADER_*/