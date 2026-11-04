#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <stdio.h>
#include <stdlib.h>

PSP_MODULE_INFO("GodOfStrength", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USERMODE | THREAD_ATTR_VFPU);

int exit_callback(int arg1, int arg2, void *common){sceKernelExitGame();return 0;}
int CallbackThread(SceSize args, void *argp){int cbid=sceKernelCreateCallback("Exit Callback",exit_callback,NULL);sceKernelRegisterExitCallback(cbid);sceKernelSleepThreadCB();return 0;}
int SetupCallbacks(void){int thid=sceKernelCreateThread("update_thread",CallbackThread,0x11,0xFA0,0,0);if(thid>=0)sceKernelStartThread(thid,0,0);return thid;}

int main(){
    SetupCallbacks();
    pspDebugScreenInit();
    SceCtrlData pad;
    int gameState=0, logoTimer=0, playerX=10, playerY=15, health=100, temple=1;
    while(1){
        sceCtrlReadBufferPositive(&pad,1);
        if(gameState==0){
            pspDebugScreenClear();
            pspDebugScreenSetXY(10,10); pspDebugScreenKprintf("LIL STAR ENTERTAINMENT");
            pspDebugScreenSetXY(8,12); pspDebugScreenKprintf("PRESENTS");
            pspDebugScreenSetXY(5,15); pspDebugScreenKprintf("GOD OF STRENGTH - ISMAIL SAGA");
            logoTimer++; if(logoTimer>200) gameState=1;
            sceDisplayWaitVblankStart(); sceKernelDelayThread(50000);
        } else if(gameState==1){
            pspDebugScreenClear();
            pspDebugScreenSetXY(5,5); pspDebugScreenKprintf("A Lil Star Entertainment Presents");
            pspDebugScreenSetXY(3,7); pspDebugScreenKprintf("GOD OF STRENGTH - Starring ISMAIL");
            pspDebugScreenSetXY(3,9); pspDebugScreenKprintf("One Warrior. Five Temples. One Final Battle.");
            pspDebugScreenSetXY(3,12); pspDebugScreenKprintf("Press X to Start - Easy/Hard/Very Hard");
            pspDebugScreenSetXY(3,14); pspDebugScreenKprintf("D-PAD move, SQUARE attack");
            if(pad.Buttons & PSP_CTRL_CROSS){gameState=2; sceKernelDelayThread(300000);}
            sceDisplayWaitVblankStart();
        } else {
            pspDebugScreenClear();
            char buf[100];
            sprintf(buf,"TEMPLE %d/5 - HEALTH: %d%% - ISMAIL",temple,health);
            pspDebugScreenSetXY(1,1); pspDebugScreenKprintf(buf);
            pspDebugScreenSetXY(1,3); pspDebugScreenKprintf("----------------------------------------");
            pspDebugScreenSetXY(playerX,playerY); pspDebugScreenKprintf("I");
            pspDebugScreenSetXY(30,15); pspDebugScreenKprintf("B");
            pspDebugScreenSetXY(2,22); pspDebugScreenKprintf("Ismail(I) vs BABBAKA(B) - ARROWS+SQUARE, START=Menu");
            if(pad.Buttons & PSP_CTRL_LEFT && playerX>0) playerX--;
            if(pad.Buttons & PSP_CTRL_RIGHT && playerX<39) playerX++;
            if(pad.Buttons & PSP_CTRL_UP && playerY>5) playerY--;
            if(pad.Buttons & PSP_CTRL_DOWN && playerY<20) playerY++;
            if(pad.Buttons & PSP_CTRL_SQUARE && playerX>25){
                pspDebugScreenSetXY(10,12); pspDebugScreenKprintf("ISMAIL SMASH! BABBAKA HIT!!");
                health-=5;
                if(health<=0){
                    pspDebugScreenClear();
                    pspDebugScreenSetXY(10,12); pspDebugScreenKprintf("YOU DEFEAT BABBAKA! GOD OF STRENGTH!");
                    sceDisplayWaitVblankStart(); sceKernelDelayThread(2000000);
                    temple++; if(temple>5) temple=1; health=100;
                }
            }
            if(pad.Buttons & PSP_CTRL_START) gameState=1;
            sceKernelDelayThread(100000); sceDisplayWaitVblankStart();
        }
    } return 0;
}
