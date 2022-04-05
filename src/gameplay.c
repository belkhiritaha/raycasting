#include "gameplay.h"

void gestMovement(){
    player.x += (Keys[0] - Keys[2]) * player.deltax + (Keys [3] - Keys[1]) * sin(player.angle);
    player.y += (Keys[0] - Keys[2]) * player.deltay + (Keys[1] - Keys[3]) * cos(player.angle);
}