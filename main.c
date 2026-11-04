#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <stdlib.h>

PSP_MODULE_INFO("GodOfStrength", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USERMODE | THREAD_ATTR_VFPU);

int exit_callback(int arg1, int arg2, void *common) {
    sceKernelExitGame();
    return 0;
}
int CallbackThread(SceSize args, void *argp) {
    int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}
int SetupCallbacks(void) {
    int thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
    if (thid >= 0) sceKernelStartThread(thid, 0, 0);
    return thid;
}

void drawText(int x, int y, const char *text) {
    pspDebugScreenSetXY(x, y);
    pspDebugScreenKprintf(text);
}

int main() {
    SetupCallbacks();
    pspDebugScreenInit();
    SceCtrlData pad;

    int gameState = 0; // 0=logo, 1=menu, 2=playing
    int logoTimer = 0;
    int playerX = 10;
    int playerY = 15;
    int health = 100;
    int temple = 1;

    while(1) {
        sceCtrlReadBufferPositive(&pad, 1);
        pspDebugScreenSetOffset((int)pspDebugScreenGetOffset(0));

        if(gameState == 0) {
            // 10 SEC LOGO
            pspDebugScreenClear();
            drawText(10, 10, "LIL STAR ENTERTAINMENT");
            drawText(8, 12, "PRESENTS");
            drawText(5, 15, "GOD OF STRENGTH - ISMAIL SAGA");
            logoTimer++;
            if(logoTimer > 200) gameState = 1; // ~10 sec
            sceDisplayWaitVblankStart();
            sceKernelDelayThread(50000);
        }
        else if(gameState == 1) {
            // MENU
            pspDebugScreenClear();
            drawText(5, 5, "A Lil Star Entertainment Presents");
            drawText(3, 7, "GOD OF STRENGTH - Starring ISMAIL");
            drawText(3, 9, "One Warrior. Five Temples. One Final Battle.");
            drawText(3, 11, "");
            drawText(3, 12, "Press X to Start - Easy/Hard/Very Hard");
            drawText(3, 14, "Use D-PAD to move, [] to attack");
            drawText(3, 16, "Temple: 1 - ENUGU VILLAGE");
            if(pad.Buttons & PSP_CTRL_CROSS) {
                gameState = 2;
                sceKernelDelayThread(300000);
            }
            sceDisplayWaitVblankStart();
        }
        else if(gameState == 2) {
            // GAMEPLAY
            pspDebugScreenClear();
            char buf[100];
            sprintf(buf, "TEMPLE %d/5 - HEALTH: %d%% - ISMAIL", temple, health);
            drawText(1, 1, buf);
            drawText(1, 3, "----------------------------------------");
            // Draw map simple
            for(int y=5; y<20; y++) {
                for(int x=0; x<40; x++) {
                    if(x==playerX && y==playerY) {
                        drawText(x, y, "I"); // Ismail
                    } else if(x==30 && y==15) {
                        drawText(x, y, "B"); // Babbaka boss
                    }
                }
            }
            drawText(2, 22, "Ismail (I) vs BABBAKA (B) - Use ARROWS, SQUARE=Attack, START=Menu");
            if(pad.Buttons & PSP_CTRL_LEFT) { if(playerX>0) playerX--; }
            if(pad.Buttons & PSP_CTRL_RIGHT) { if(playerX<39) playerX++; }
            if(pad.Buttons & PSP_CTRL_UP) { if(playerY>5) playerY--; }
            if(pad.Buttons & PSP_CTRL_DOWN) { if(playerY<20) playerY++; }
            if(pad.Buttons & PSP_CTRL_SQUARE) {
                if(playerX > 25) {
                    drawText(10, 12, "ISMAIL SMASH! BABBAKA HIT!!");
                    health -= 5;
                    if(health <=0) {
                        pspDebugScreenClear();
                        drawText(10, 12, "YOU DEFEAT BABBAKA! YOU ARE GOD OF STRENGTH!");
                        drawText(10, 14, "Press START to return to Enugu Village");
                        sceDisplayWaitVblankStart();
                        sceKernelDelayThread(2000000);
                        temple++;
                        if(temple>5) temple=1;
                        health=100;
                    }
                }
            }
            if(pad.Buttons & PSP_CTRL_START) { gameState = 1; }
            sceKernelDelayThread(100000);
            sceDisplayWaitVblankStart();
        }
    }
    return 0;
}
