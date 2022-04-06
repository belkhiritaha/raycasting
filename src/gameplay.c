#include "gameplay.h"

void gestMovement(){
    float x_increment = (Keys[0] - Keys[2]) * player.deltax + (Keys [3] - Keys[1]) * sin(player.angle);
    float y_increment = (Keys[0] - Keys[2]) * player.deltay + (Keys[1] - Keys[3]) * cos(player.angle);
    float newpos_x = (player.x + x_increment)/64;
    float newpos_y = (player.y + y_increment)/64;
    // if new position in in map
    //if (newpos_x >= 0 && newpos_x < MAPSIZE && newpos_y >= 0 && newpos_y < MAPSIZE){
        // if new positin doesnt hit wall
      //  if (map[(int)newpos_x][(int)newpos_y] == 0){
            player.x += x_increment * MV_SPEED;
            player.y += y_increment * MV_SPEED;
        //}
    //}
}