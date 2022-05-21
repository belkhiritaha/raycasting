#ifndef _GAMEPLAY_HEADER_
#define _GAMEPLAY_HEADER_

#include "gest_event.h"

#define MV_SPEED (1)
#define BULLET_SPEED (100)
#define SHOOT_DELAY (10)

void gestMovement();
void Shoot();
void gestAll();
void printBulletList();
#endif