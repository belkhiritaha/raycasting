#include "gameplay.h"

Player_t player;
Ennemy_t * ennemy_head;

void gestMovement(){
    float x_increment = (Keys[0] - Keys[2]) * player.deltax + (Keys [3] - Keys[1]) * sin(player.angle);
    float y_increment = (Keys[0] - Keys[2]) * player.deltay + (Keys[1] - Keys[3]) * cos(player.angle);
    float newpos_x = (player.x + + x_increment)/BLOCK_SIZE;
    float newpos_y = (player.y + y_increment)/BLOCK_SIZE;
    if (newpos_x >= 0 && newpos_x < MAPSIZE && newpos_y >= 0 && newpos_y < MAPSIZE){
        if (map[(int)newpos_y][(int)newpos_x] != 1){
            player.x += x_increment * MV_SPEED;
            player.y += y_increment * MV_SPEED;
        }
    }
}

void initEnnemy(Ennemy_t * ennemy){
    ennemy->x = rand()% MAPSIZE * BLOCK_SIZE;
    ennemy->y = rand()% MAPSIZE * BLOCK_SIZE;
    ennemy->angle = 0;
    ennemy->deltax = 0;
    ennemy->deltay = 0;
    ennemy->hp = 5;
}

void SpawnEnnemies(int n){
    for (int i = 0; i < n; i++){
        Ennemy_t * new_ennemy = (Ennemy_t *) malloc(sizeof(Ennemy_t));
        initEnnemy(new_ennemy);
        new_ennemy->next = ennemy_head;
        ennemy_head = new_ennemy;
    }
}


void Shoot(){
    if (player.shoot_timer == 0){
        player.shoot_timer = SHOOT_DELAY;
        Bullet_t * bullet = malloc(sizeof(Bullet_t));
        bullet->x = player.x;
        bullet->y = player.y;
        bullet->angle = player.angle;
        bullet->deltay = sin(player.angle);
        bullet->deltax = cos(player.angle);
        bullet->speed = BULLET_SPEED;
        bullet->next = NULL;
        if (player.bullet_list == NULL){
            player.bullet_list = bullet;
        }
        else {
            Bullet_t * tmp = player.bullet_list;
            // add to head of list
            player.bullet_list = bullet;
            bullet->next = tmp;
        }
    }
}

void gestBullet(){
    Bullet_t * tmp = player.bullet_list;
    while (tmp != NULL){
        tmp->x += tmp->deltax * tmp->speed;
        tmp->y += tmp->deltay * tmp->speed;
        tmp = tmp->next;
    }
}


void DeleteBullet(Bullet_t * bullet){
    Bullet_t * curr = player.bullet_list;
    if (curr == bullet){
        player.bullet_list = curr->next;
        free(curr);
    }
    else {
        while (curr->next != bullet){
            curr = curr->next;
        }
        curr->next = bullet->next;
        free(bullet);
    }
}


void DeleteEnnemy(Ennemy_t * ennemy){
    Ennemy_t * curr = ennemy_head;
    if (curr == ennemy){
        ennemy_head = curr->next;
        free(curr);
    }
    else {
        while (curr->next != ennemy){
            curr = curr->next;
        }
        curr->next = ennemy->next;
        free(ennemy);
    }
}


void updatePlayerShootTimer(){
    if (player.shoot_timer > 0){
        player.shoot_timer--;
    }
}

void checkHitEnnemy(Ennemy_t * ennemy_head){
    Ennemy_t * tmp = ennemy_head;
    Ennemy_t * tmp2 = ennemy_head;

    Bullet_t * tmp_bullet2;
    while (tmp != NULL){
        Bullet_t * tmp_bullet = player.bullet_list;
        while (tmp_bullet != NULL){
            if (fabs(tmp->x - tmp_bullet->x) < BLOCK_SIZE * 2 && fabs(tmp->y - tmp_bullet->y) < BLOCK_SIZE/4){
                tmp->hp--;
                if (tmp->hp == 0){
                    tmp2 = tmp->next;
                    DeleteEnnemy(tmp);
                    tmp->next = tmp2;
                }
            }
            tmp_bullet = tmp_bullet->next;
        }

        tmp = tmp->next;
    }
}


void printBulletList(){
    Bullet_t * tmp = player.bullet_list;
    while (tmp != NULL){
        printf("%f %f\n", tmp->x, tmp->y);
        tmp = tmp->next;
    }
}

void gestAll(){
    gestMovement();
    gestBullet();
    updatePlayerShootTimer();
    checkHitEnnemy(ennemy_head);
}