#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <pspctrl.h>

PSP_MODULE_INFO("God Of Strength", 0, 1, 1);

int main() {
    pspDebugScreenInit();
    pspDebugScreenSetXY(10,10);
    pspDebugScreenPrintf("A Lil Star Entertainment Presents");
    pspDebugScreenSetXY(10,12);
    pspDebugScreenPrintf("GOD OF STRENGTH - Starring ISMAIL");
    pspDebugScreenSetXY(10,15);
    pspDebugScreenPrintf("One Warrior. Five Temples. One Final Battle.");
    pspDebugScreenSetXY(10,18);
    pspDebugScreenPrintf("Press X to Start - Easy/Hard/Very Hard");

    SceCtrlData pad;
    while(1) {
        sceCtrlReadBufferPositive(&pad, 1);
        if(pad.Buttons & PSP_CTRL_CROSS) break;
        sceDisplayWaitVblankStart();
    }
    sceKernelExitGame();
    return 0;
}
