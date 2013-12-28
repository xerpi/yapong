/* yapong by xerpi */

#ifndef _YAPONG_H_
#define _YAPONG_H_

#define DEG_TO_RAD(x) ((x) * 0.01745329251)
#define RAND_0_1() ((float)rand()/(float)RAND_MAX)

#define SCREEN_WIDTH  480
#define SCREEN_HEIGHT 272

#define BALL_DEFAULT_JERK   0.01f
#define BALL_DEFAULT_SIZE   8
#define BALL_RESPAWN_ACCEL  0.25f
#define BALL_RESPAWN_SPEED  0.0f
#define BALL_MINIMUM_ACCEL  0.002f

#define RACKET_DEFAULT_WIDTH  7
#define RACKET_DEFAULT_HEIGHT 55
#define RACKET_PRESS_ACCEL    1.0f
#define RACKET_FRICTION_ACCEL 1.25f

typedef struct {
    float x, y;
} vector_2f;

typedef struct {
    int x, y;
} vector_2i;

enum {
    PLAYER_1 = 1,
    PLAYER_2 = 2
};

enum {
    MOVING_UP,
    MOVING_DOWN,
    MOVING_REST
};

typedef struct {
    vector_2f pos;
    float speed, accel, press_accel;
    vector_2i size;
    int color, player, status, score;
} yaracket_t;

typedef struct {
    vector_2f pos, direction;
    float speed, accel, jerk;
    vector_2i size;
    int color;
} yaball_t;



void yaball_init(yaball_t *ball);
void yaball_move(yaball_t *ball);
void yaball_calc_dir(yaball_t *ball, float angle);
void yaball_draw(yaball_t *ball);

void yaracket_init(yaracket_t *racket, int player);
void yaracket_move_up(yaracket_t *racket);
void yaracket_move_down(yaracket_t *racket);
void yaracket_move(yaracket_t *racket);
void yaracket_draw(yaracket_t *racket);

int yaracket_collision_ball(yaracket_t *racket, yaball_t *ball);


#endif
