/* yapong by xerpi */

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pspkernel.h>
#include <pspctrl.h>
#include <pspgu.h>
#include <tinyfont.h>
#include <ya2d.h>
#include "yapong.h"

PSP_MODULE_INFO("yapong", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

int run = 1;
int exit_callback(int arg1, int arg2, void *common);
int CallbackThread(SceSize args, void *argp);
int SetupCallbacks();


int main(int argc, char* argv[])
{
    srand(time(NULL));

    SetupCallbacks();

    ya2d_init();	
    ya2d_set_clear_color(0);
    ya2d_set_vsync(1);

    SceCtrlData pad, old_pad; old_pad.Buttons = 0;

    yaball_t ball;
    yaball_init(&ball);
    yaracket_t racket1, racket2;
    yaracket_init(&racket1, PLAYER_1);
    yaracket_init(&racket2, PLAYER_2);

    while (run) {
        sceCtrlPeekBufferPositive(&pad, 1);
        ya2d_start();

        ya2d_draw_line(SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT, 0xFFFFFFFF);

        tinyfont_draw_stringf(112, 3,  GU_RGBA(255,255,255,255), "%i", racket1.score);
        tinyfont_draw_stringf(352, 3,  GU_RGBA(255,255,255,255), "%i", racket2.score);

        //Move rackets
        if (pad.Buttons & PSP_CTRL_UP)
            yaracket_move_up(&racket1);
        else if (pad.Buttons & PSP_CTRL_DOWN)
            yaracket_move_down(&racket1);

        if (pad.Buttons & PSP_CTRL_TRIANGLE)
            yaracket_move_up(&racket2);
        else if (pad.Buttons & PSP_CTRL_CROSS)
            yaracket_move_down(&racket2);


        if (pad.Buttons & PSP_CTRL_SELECT & ~old_pad.Buttons) {
            yaball_init(&ball);
            yaracket_init(&racket1, PLAYER_1);
            yaracket_init(&racket2, PLAYER_2);
        }

        yaracket_move(&racket1);
        yaracket_move(&racket2);
        yaball_move(&ball);

        //Check ball-racket collisions
        if (yaracket_collision_ball(&racket1, &ball)) {
            ball.pos.x = racket1.pos.x+racket1.size.x + 1;
            yaball_calc_dir(&ball, atan2f(ball.direction.y + racket1.speed/12.5f, -ball.direction.x));
        }
        if (yaracket_collision_ball(&racket2, &ball)) {
            ball.pos.x = racket2.pos.x-ball.size.x - 1;
            yaball_calc_dir(&ball, atan2f(ball.direction.y + racket2.speed/12.5f, -ball.direction.x));
        }

        //Check goal
        if (ball.pos.x+ball.size.x < 0) {
            ++racket2.score;
            yaball_init(&ball);
        } else if (ball.pos.x > SCREEN_WIDTH) {
            ++racket1.score;
            yaball_init(&ball);
        }

        yaball_draw(&ball);
        yaracket_draw(&racket1);
        yaracket_draw(&racket2);

        ya2d_finish();
        ya2d_swapbuffers();
        old_pad = pad;
    }

    ya2d_shutdown();
    sceKernelExitGame();
    return 0;
}


/* Exit callback */
int exit_callback(int arg1, int arg2, void *common)
{
    run = 0;
    return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp)
{
    int cbid;
    cbid = sceKernelCreateCallback("Exit Callback", exit_callback, argp);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks()
{
    int thid = 0;
    thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
    if(thid >= 0) {
        sceKernelStartThread(thid, 0, NULL);
    }
    return thid;
}


