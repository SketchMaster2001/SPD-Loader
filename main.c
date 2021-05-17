#include <gccore.h>
#include <stdio.h>
#include <unistd.h>

#define TITLE_ID(x,y)		(((u64)(x) << 32) | (y))

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

void Goto_SPD() {
    WII_LaunchTitle(TITLE_ID(0x00010008,0x4843434a));
}

int main(void) {
    VIDEO_Init();

    rmode = VIDEO_GetPreferredMode(NULL);
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    console_init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight,
                 rmode->fbWidth * VI_DISPLAY_PIX_SZ);
    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(xfb);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync();

    ISFS_Initialize();
    CONF_Init();

    s32 fd = ISFS_Open("/title/00010008/4843434a/content/00000001.app", ISFS_OPEN_WRITE);
    if (fd < 0) {
        printf("You do not have the Set Personal Data(SPD) Channel installed.\nPlease download it from the WiiLink24 Patcher.");
        sleep(5);
        WII_ReturnToMenu();
    } else {
        Goto_SPD();
    }

    return 0;
}
