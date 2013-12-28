/* yapong by xerpi */

#include <math.h>
#include <stdlib.h>
#include <ya2d.h>
#include "yapong.h"


void yaball_init(yaball_t *ball)
{
    ball->size.x = ball->size.y = BALL_DEFAULT_SIZE;
    ball->pos.x = SCREEN_WIDTH/2  - ball->size.x/2;
    ball->pos.y = SCREEN_HEIGHT/2 - ball->size.y/2;
    
    //Angle between -30 and 30, or between -150 and 150
    float angle = (float)DEG_TO_RAD( (-30 + RAND_0_1()*60) + (RAND_0_1() < 0.5f ? 180 : 0));
    yaball_calc_dir(ball, angle);
    
    ball->speed = BALL_RESPAWN_SPEED;
    ball->accel = BALL_RESPAWN_ACCEL;
    ball->jerk  = BALL_DEFAULT_JERK;
    ball->color = 0xFF00FFFF;
}


void yaball_move(yaball_t *ball)
{
    if (ball->accel > BALL_MINIMUM_ACCEL) {
        ball->accel -= ball->jerk;
        if (ball->accel < BALL_MINIMUM_ACCEL) {
            ball->accel = BALL_MINIMUM_ACCEL;
        }
    }
    
    ball->speed += ball->accel;
    ball->pos.x += ball->direction.x * ball->speed;
    ball->pos.y += ball->direction.y * ball->speed;
    
    //Vertical collision
    if (ball->pos.y < 0) {
        ball->pos.y = 0;
        ball->direction.y *= -1;
    } else if ((ball->pos.y + ball->size.y) > SCREEN_HEIGHT) {
        ball->pos.y = SCREEN_HEIGHT - ball->size.y;
        ball->direction.y *= -1;
    }
    
    //Horizontal collision (it's a pong!!)
    /*if (ball->pos.x < 0) {
        ball->pos.x = 0;
        ball->direction.x *= -1;
    } else if ((ball->pos.x + ball->size.x) > SCREEN_WIDTH) {
        ball->pos.x = SCREEN_WIDTH - ball->size.x;
        ball->direction.x *= -1;
    }*/
}

void yaball_calc_dir(yaball_t *ball, float angle)
{
    ball->direction.x = cosf(angle);
    ball->direction.y = sinf(angle);
}


void yaball_draw(yaball_t *ball)
{
    ya2d_draw_fillrect(ball->pos.x, ball->pos.y, ball->size.x, ball->size.y, ball->color);  
}




void yaracket_init(yaracket_t *racket, int player)
{
    racket->size.x = RACKET_DEFAULT_WIDTH;
    racket->size.y = RACKET_DEFAULT_HEIGHT;
    racket->player = player;
    
    if (player == PLAYER_1) {
        racket->pos.x = 0.0f;
    } else {
        racket->pos.x = SCREEN_WIDTH - racket->size.x;
    }
    racket->pos.y = SCREEN_HEIGHT/2 - racket->size.y/2;

    racket->speed = 0.0f;
    racket->accel = 0.0f;
    racket->press_accel = RACKET_PRESS_ACCEL;   
    racket->status = MOVING_REST;
    racket->score = 0;
    racket->color = player == PLAYER_1 ? 0xFF0000FF : 0xFFFF0000;
}

void yaracket_move_up(yaracket_t *racket)
{
    racket->status = MOVING_UP;
    racket->accel  = -racket->press_accel;
}

void yaracket_move_down(yaracket_t *racket)
{
    racket->status = MOVING_DOWN;
    racket->accel  = racket->press_accel;
}

void yaracket_move(yaracket_t *racket)
{
    
    racket->speed += racket->accel;
    racket->pos.y += racket->speed;

    if (racket->status == MOVING_UP) {
        if (racket->speed < 0.0f) {
            racket->accel = RACKET_FRICTION_ACCEL;
        } else {
            racket->speed = 0.0f;
            racket->accel = 0.0f;
        }            
    } else if (racket->status == MOVING_DOWN) {
        if (racket->speed > 0.0f) {
            racket->accel = -RACKET_FRICTION_ACCEL;
        } else {
            racket->speed = 0.0f;
            racket->accel = 0.0f;
        }  
    }
    
    //Vertical collision
    if (racket->pos.y < 0) {
        racket->pos.y = 0.0f;
        racket->speed = 0.0f;
    } else if ((racket->pos.y + racket->size.y) > SCREEN_HEIGHT) {
        racket->pos.y = SCREEN_HEIGHT - racket->size.y;
        racket->speed = 0.0f;
    }
}

void yaracket_draw(yaracket_t *racket)
{
    ya2d_draw_fillrect(racket->pos.x, racket->pos.y, racket->size.x, racket->size.y, racket->color);  
}


int yaracket_collision_ball(yaracket_t *racket, yaball_t *ball)
{
    if ( ((ball->pos.x+ball->size.x) >= racket->pos.x) && (ball->pos.x <= (racket->pos.x+racket->size.x)) &&
         ((ball->pos.y+ball->size.y) >= racket->pos.y) && (ball->pos.y <= (racket->pos.y+racket->size.y)) ) {
            return 1;
    }
    return 0;
}

