#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <stdio.h>

PSP_MODULE_INFO("GodOfStrength", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

int main_thread(SceSize args, void *argp) {
    pspDebugScreenInit();
    SceCtrlData pad;
    int state = 0;
    int timer = 0;
    int px = 10;
    int py = 15;
    int hp = 100;
    int temple = 1;
    
    while(1) {
        sceCtrlReadBufferPositive(&pad, 1);
        pspDebugScreenClear();
        
        if(state==0) {
            pspDebugScreenSetXY(10,10);
            pspDebugScreenKprintf("LIL STAR ENTERTAINMENT PRESENTS");
            pspDebugScreenSetXY(5,13);
            pspDebugScreenKprintf("GOD OF STRENGTH - ISMAIL SAGA");
            pspDebugScreenSetXY(8,15);
            pspDebugScreenKprintf("LOADING... %d", timer);
            timer++;
            if(timer>150) state=1;
        }
        else if(state==1) {
            pspDebugScreenSetXY(5,5);
            pspDebugScreenKprintf("GOD OF STRENGTH - Starring ISMAIL");
            pspDebugScreenSetXY(3,7);
            pspDebugScreenKprintf("One Warrior. Five Temples.");
            pspDebugScreenSetXY(3,9);
            pspDebugScreenKprintf("Press X to Start");
            pspDebugScreenSetXY(3,11);
            pspDebugScreenKprintf("Easy / Hard / Very Hard");
            if(pad.Buttons & PSP_CTRL_CROSS) {
                state=2;
                sceKernelDelayThread(300*1000);
            }
        }
        else {
            char buf[80];
            sprintf(buf,"TEMPLE %d/5  HP:%d  ISMAIL vs BABBAKA",temple,hp);
            pspDebugScreenSetXY(0,0);
            pspDebugScreenKprintf(buf);
            pspDebugScreenSetXY(px,py);
            pspDebugScreenKprintf("I");
            pspDebugScreenSetXY(30,15);
            pspDebugScreenKprintf("B");
            pspDebugScreenSetXY(1,21);
            pspDebugScreenKprintf("D-PAD MOVE  SQUARE=ATTACK  START=MENU");
            if(pad.Buttons & PSP_CTRL_LEFT) px--;
            if(pad.Buttons & PSP_CTRL_RIGHT) px++;
            if(pad.Buttons & PSP_CTRL_UP) py--;
            if(pad.Buttons & PSP_CTRL_DOWN) py++;
            if(px<0) px=0; if(px>60) px=60;
            if(py<2) py=2; if(py>20) py=20;
            if((pad.Buttons & PSP_CTRL_SQUARE) && px>25) {
                pspDebugScreenSetXY(15,12);
                pspDebugScreenKprintf("SMASH! HIT BABBAKA!");
                hp--;
                if(hp<=0) {
                    pspDebugScreenSetXY(10,10);
                    pspDebugScreenKprintf("YOU WIN! GOD OF STRENGTH!");
                    sceKernelDelayThread(2000*1000);
                    temple++; if(temple>5) temple=1;
                    hp=100;
                }
            }
            if(pad.Buttons & PSP_CTRL_START) state=1;
        }
        sceDisplayWaitVblankStart();
        sceKernelDelayThread(80*1000);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    SceUID thid = sceKernelCreateThread("main_thread", main_thread, 20, 0x10000, 0, NULL);
    if(thid >= 0) sceKernelStartThread(thid, 0, NULL);
    sceKernelExitDeleteThread(0);
    return 0;
}
