/************************/
/*                      */
/*  Walking Test        */
/*                      */
/*  Version 2.1         */
/*                      */
/*  02/07/2021          */
/*                      */
/************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <exec/types.h>
#include <exec/memory.h>
#include <intuition/intuition.h>
#include <graphics/gfx.h>

#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>
#include <clib/graphics_protos.h>

#define MAPSIZE 32
#define MAPMASK (MAPSIZE-1)
#define SWIDTH 320
#define SHEIGHT 200
#define SDEPTH 5

char map[MAPSIZE][MAPSIZE+1] = {
    "*******..................*******",
    "*.....*..................*.....*",
    "*.....********************.....*",
    "*..............................*",
    "*.....********.***********.....*",
    "*.....*......*.....*.....*.....*",
    "***.***............*.....***.***",
    "..*.*.*......*.....*.....*.*.*..",
    "..*.*........*.............*.*..",
    "..*.**********.....*.....*.*.*..",
    "..*.*.....*..*.....*.....*.*.*..",
    "..*.*..............*.....*.*.*..",
    "..*.*.....****************.*.*..",
    "..*.*.....*...........*..*.*.*..",
    "..*.*.....*...........*....*.*..",
    "..*.*******...........*..*.*.*..",
    "..*.*.....*...........****.*.*..",
    "..*.*......................*.*..",
    "..*.*.**.**...........****.*.*..",
    "..*.*.*...*...........*....*.*..",
    "..*.*.*...*...........*....*.*..",
    "..*.*.*...*...........*......*..",
    "..*.*.********.....****....*.*..",
    "..*.*.*......*.....*.......*.*..",
    "..*.*........*.....*.......*.*..",
    "***.***...*****...*****..***.***",
    "*.....*****...*...*...****.....*",
    "*.............*...*............*",
    "*.....*****...*...*...****.....*",
    "*.....*...*****...*****..*.....*",
    "*.....*..................*.....*",
    "*******..................*******",
};

/************************/
/*                      */
/*  Color Map           */
/*                      */
/************************/

USHORT Colors[] = {
    0x000, 0xF0F, 0x333, 0x555,
    0x888, 0xAAA, 0xDDD, 0xFFF,
    0x89F, 0x67F, 0x35F, 0x13F,
    0x0F0, 0x0D0, 0x0A0, 0x080,
    
    0xFEA, 0xEC8, 0xDB7, 0xC96,
    0xB85, 0xA64, 0x953, 0x842,
    0x942, 0xB32, 0xC31, 0xE21,
    0xF00, 0xF50, 0xFA0, 0xFF0
};

/************************/
/*                      */
/*  Wall Gfx Data       */
/*                      */
/************************/

// USHORT Wall[] = {
//     0xffff, 0xffff, 0xffff, 0x0000, 0xffff,
//     0xbf77, 0x9375, 0x6c8e, 0x0000, 0xffff,
//     0xfbff, 0xdb83, 0x6c7c, 0x0000, 0xffff,
//     0xffff, 0x01b6, 0xfe49, 0x0000, 0xffff,
    
//     0x6fda, 0xfd27, 0x02dc, 0x0000, 0xffff,
//     0xdf7f, 0x9341, 0x6cbe, 0x0000, 0xffff,
//     0xfd83, 0xb6cb, 0x4934, 0x0000, 0xffff,
//     0xffff, 0xffff, 0xffff, 0x0000, 0xffff,
    
//     0xd97f, 0x0056, 0xffad, 0x0000, 0xffff,
//     0x9933, 0x9131, 0x6ece, 0x0000, 0xffff,
//     0xffef, 0xda27, 0x25d8, 0x0000, 0xffff,
//     0xefff, 0xb4f7, 0x4b08, 0x0000, 0xffff,
    
//     0xefdf, 0xd9cf, 0x2630, 0x0000, 0xffff,
//     0x96ee, 0xd935, 0x0000, 0x0000, 0xffff,
//     0xffff, 0xffff, 0xffff, 0x0000, 0xffff,
//     0x0000, 0x0000, 0x0000, 0x0000, 0xffff
// };
USHORT Wall[] = {
    0xffff, 0xbf77, 0xfbbf, 0xffff,
    0x6fda, 0xdf7f, 0xfd83, 0xffff,
    0xd97f, 0x9933, 0xffef, 0xefff,
    0xefdf, 0x96ee, 0xffff, 0x0000,
    
    0xffff, 0x9375, 0xdb83, 0x01b6,
    0xfd27, 0x9341, 0xb6cb, 0xffff,
    0x0056, 0x9131, 0xda27, 0xb4f7,
    0xd9cf, 0xd935, 0xffff, 0x0000,
    
    0xffff, 0x6c8e, 0x6c7c, 0xfe49,
    0x02dc, 0x6cbe, 0x4934, 0xffff,
    0xffad, 0x6ece, 0x25d8, 0x4b08,
    0x2630, 0x0000, 0xffff, 0x0000,
    
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
        
    0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff
};

/************************/
/*                      */
/*  Floor Gfx Data      */
/*                      */
/************************/

// USHORT  Grass[] = {
//     0x5C00, 0x33FF, 0x0000, 0xFFFE, 0x0000,
//     0xa67c, 0x19ff, 0x0000, 0xfffe, 0x0000,
//     0x513c, 0x8eff, 0x0000, 0xfffe, 0x0000,
//     0x286c, 0xc77f, 0x0000, 0xfffe, 0x0000,
    
//     0x241c, 0xcbff, 0x0000, 0xfffe, 0x0000,
//     0x99bc, 0x67ff, 0x0000, 0xfffe, 0x0000,
//     0x46fc, 0xbdff, 0x0000, 0xfffe, 0x0000,
//     0x227c, 0xddff, 0x0000, 0xfffe, 0x0000,
    
//     0x10bc, 0xef7f, 0x0000, 0xfffe, 0x0000,
//     0x447c, 0xfcff, 0x0000, 0xfffe, 0x0000,
//     0x44bc, 0xfbff, 0x0000, 0xfffe, 0x0000,
//     0x52dc, 0xfdff, 0x0000, 0xfffe, 0x0000,
    
//     0x78bc, 0xffff, 0x0000, 0xfffe, 0x0000,
//     0x7d7c, 0xffff, 0x0000, 0xfffe, 0x0000,
//     0x0002, 0xfffd, 0x0000, 0xfffe, 0x0000,
//     0x0000, 0xfffe, 0x0000, 0x0001, 0x0000
// };
USHORT Grass[] = {
    0x5C00, 0xA67C, 0x513C, 0x286C,
    0x241C, 0x99BC, 0x46FC, 0x227C,
    0x10BC, 0x447C, 0x44BC, 0x52DC,
    0x78BC, 0x7D7C, 0x0002, 0x0000,
    
    0x33FF, 0x19FF, 0x8EFF, 0xC77F,
    0xCBFF, 0x67FF, 0xBDFF, 0xDDFF,
    0xEF7F, 0xFCFF, 0xFBFF, 0xFDFF,
    0xFFFF, 0xFFFF, 0xFFFD, 0xFFFE,
    
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
    
    0xFFFE, 0xFFFE, 0xFFFE, 0xFFFE,
    0xFFFE, 0xFFFE, 0xFFFE, 0xFFFE,
    0xFFFE, 0xFFFE, 0xFFFE, 0xFFFE,
    0xFFFE, 0xFFFE, 0xFFFE, 0x0001,
    
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000
};

/************************/
/*                      */
/*  Player Gfx Data     */
/*                      */
/************************/

// USHORT Avatar[] = {
//     0x0000, 0x0800, 0x1000, 0x28A0, 0x1FF0,
//     0x3718, 0x1260, 0x0980, 0x0030, 0x0A90, 
//     0x0158, 0x015C, 0x006C, 0x006C, 0x0000, 
//     0x0000, 0x7C00, 0x7E00, 0xFFE0, 0xF430,
    
//     0x6218, 0x5EC4, 0x47E4, 0x6F84, 0x707C, 
//     0x37FE, 0x1FEE, 0x0FA6, 0x01B6, 0x00FE, 
//     0x006C, 0x0000, 0x3000, 0x3C00, 0x7800, 
//     0x7380, 0x2380, 0x2FC0, 0x1FE0, 0x0BE0,
    
//     0x03E0, 0x01B0, 0x00D8, 0x005C, 0x006C, 
//     0x006C, 0x0000, 0x0000, 0x3000, 0x3C00, 
//     0x7800, 0x73E0, 0x23F0, 0x2FF8, 0x3FF8, 
//     0x1B98, 0x0C60, 0x09B0, 0x00C0, 0x0000,
    
//     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
//     0x0000, 0x0000, 0x0860, 0x1E70, 0x0E38, 
//     0x2618, 0x1278, 0x0F80, 0x0800, 0x0018, 
//     0x005C, 0x006C, 0x006C, 0x0000, 0x0000
// };
USHORT  Avatar[] =
{
    0x0000, 0x0800, 0x1000, 0x28A0,
    0x1FF0, 0x3718, 0x1260, 0x0980,
    0x0030, 0x0A90, 0x0158, 0x015C,
    0x006C, 0x006C, 0x0000, 0x0000,
    
    0x7C00, 0x7E00, 0xFFE0, 0xF430,
    0x6218, 0x5EC4, 0x47E4, 0x6F84,
    0x707C, 0x37FE, 0x1FEE, 0x0FA6,
    0x01B6, 0x00FE, 0x006C, 0x0000,
    
    0x3000, 0x3C00, 0x7800, 0x7380,
    0x2380, 0x2FC0, 0x1FE0, 0x0BE0,
    0x03E0, 0x01B0, 0x00D8, 0x005C,
    0x006C, 0x006C, 0x0000, 0x0000,
    
    0x3000, 0x3C00, 0x7800, 0x73E0,
    0x23F0, 0x2FF8, 0x3FF8, 0x1B98,
    0x0C60, 0x09B0, 0x00C0, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
    
    0x0000, 0x0000, 0x0000, 0x0860,
    0x1E70, 0x0E38, 0x2618, 0x1278,
    0x0F80, 0x0800, 0x0018, 0x005C,
    0x006C, 0x006C, 0x0000, 0x0000
};

/********************************/
/*                              */
/*  Chr Output Buffer           */
/*                              */
/********************************/

USHORT avatarBuffer[80];
USHORT *avatarMask = NULL;

struct IntuitionBase *IntuitionBase;
struct GfxBase *GfxBase;

struct Screen *GameScreen;
struct Window *FirstWindow;
struct BitMap *GameMap;

void draw(int, int);
void Open_All();
void Close_All(int);
void DrawAvatarOverTile(USHORT *, USHORT *, USHORT *, USHORT *);
struct BitMap *allocbitmap(LONG, LONG, LONG);
void freebitmap(struct BitMap *);
USHORT *buildAvatarMask(USHORT *, int);
void freeAvatarMask(USHORT *);

/************************/
/*                      */
/* Main Program Loop    */
/*                      */
/************************/

int main(int argc, char *argv[]) {
    int x = 16;
    int y = 16;
    int tx, ty;
    char code;
    struct IntuiMessage *message;
    ULONG MessageClass;
    int collisionDetection = TRUE;

    Open_All();
    
    FOREVER {
        draw(x, y);
        
        tx = x;
        ty = y;
        
        if ((message = (struct IntuiMessage *) GetMsg(FirstWindow->UserPort)) == NULL) {
            Wait(1L << FirstWindow->UserPort->mp_SigBit);
            continue;
        }
        
        ReplyMsg((struct Message *)message);
        
        code = message->Code;
        MessageClass = message->Class;
        
        if(MessageClass & RAWKEY)
        {
            switch(code) {
                case 0x3e:
                    ty = y - 1;
                    if(ty < 0) {
                        ty = ty + MAPSIZE;
                    }
                    break;
                case 0x1e:
                    ty = y + 1;
                    if(ty > MAPSIZE-1) {
                        ty = ty - MAPSIZE;
                    }
                    break;
                case 0x2d:
                    tx = x - 1;
                    if(tx < 0) {
                        tx = tx + MAPSIZE;
                    }
                    break;
                case 0x2f:
                    tx = x + 1;
                    if(tx > MAPSIZE-1) {
                        tx = tx - MAPSIZE;
                    }
                    break;
                case 0x3d:
                    ty = y - 1;
                    if(ty < 0) {
                        ty = ty + MAPSIZE;
                    }
                    tx = x - 1;
                    if(tx < 0) {
                        tx = tx + MAPSIZE;
                    }
                    break;
                case 0x3f:
                    ty = y - 1;
                    if(ty < 0) {
                        ty = ty + MAPSIZE;
                    }
                    tx = x + 1;
                    if(tx > MAPSIZE-1) {
                        tx = tx - MAPSIZE;
                    }
                    break;
                case 0x1d:
                    ty = y + 1;
                    if(ty > MAPSIZE-1) {
                        ty = ty - MAPSIZE;
                    }
                    tx = x - 1;
                    if(tx < 0) {
                        tx = tx + MAPSIZE;
                    }
                    break;
                case 0x1f:
                    ty = y + 1;
                    if(ty > MAPSIZE-1) {
                        ty = ty - MAPSIZE;
                    }
                    tx = x + 1;
                    if(tx > MAPSIZE-1) {
                        tx = tx - MAPSIZE;
                    }
                    break;
                case 0x45:
                    Close_All(TRUE);
                    break;
                case 0x50:                      // F1
                    collisionDetection = collisionDetection ? FALSE : TRUE;
                    break;
                default:
                    if(code >= 0) {
                        printf("Unrecognized key: 0x%x\n", code);
                    }
            }

            if(collisionDetection && (((map [ty] [x] == '*') && (map [y] [tx] == '*')) || (map [ty] [tx] == '*'))) {
                DisplayBeep(NULL);
            } else {
                y = ty;
                x = tx; 
            }                   
        }
        
        if(MessageClass & CLOSEWINDOW) Close_All(TRUE);
    }
}

/****************************************/
/*                                      */
/* Draw Screen V. 2.0                   */
/*                                      */
/* draw(x,y) : x = XPos, y = YPos       */
/*                                      */
/****************************************/

void draw(int x, int y) {
    int drawX, drawY, outputX, outputY;
    // struct BitMap bitmap;
    // memset(&bitmap, 0, sizeof(bitmap));

    // bitmap.BytesPerRow = 2;
    // bitmap.Rows = 16;
    // bitmap.Depth = 5;
    
    struct Image outImage;
    memset(&outImage, 0, sizeof(outImage));

    outImage.Width = 16;
    outImage.Height = 16;
    outImage.Depth = 5;
    outImage.PlanePick = 0x1f;

    outputY = 8;

    for(drawY = 0; drawY < 9; drawY++) {
        outputX = 8;

        for(drawX = 0; drawX < 9; drawX++) {
            switch(map[(y - 4 + drawY) & MAPMASK][(x - 4 + drawX) & MAPMASK]) {
                case '*':
                    outImage.ImageData = Wall;
                    // bitmap.Planes[0] = (UBYTE *)(&Wall[0]);
                    // bitmap.Planes[1] = (UBYTE *)(&Wall[16]);
                    // bitmap.Planes[2] = (UBYTE *)(&Wall[32]);
                    // bitmap.Planes[3] = (UBYTE *)(&Wall[48]);
                    // bitmap.Planes[4] = (UBYTE *)(&Wall[64]);
                    break;
                case '.':
                    outImage.ImageData = Grass;
                    // bitmap.Planes[0] = (UBYTE *)(&Grass[0]);
                    // bitmap.Planes[1] = (UBYTE *)(&Grass[16]);
                    // bitmap.Planes[2] = (UBYTE *)(&Grass[32]);
                    // bitmap.Planes[3] = (UBYTE *)(&Grass[48]);
                    // bitmap.Planes[4] = (UBYTE *)(&Grass[64]);
                    break;
            }

            if(drawX == 4 && drawY == 4) {
                DrawAvatarOverTile(Avatar, outImage.ImageData, avatarMask, avatarBuffer);
                outImage.ImageData = avatarBuffer;
            }

            // BltBitMap(bitmap, 0, 0, FirstWindow->RPort->BitMap, outputX, outputY, 16, 16, 0xc0, 0xff, NULL);
            DrawImage(FirstWindow->RPort, &outImage, outputX, outputY);

            outputX += 16;
        }

        outputY += 16;
    }
}

/****************/
/*              */
/* Open up...   */
/*              */
/****************/

void Open_All() {
    // struct BitMap ScrMap;
    // PLANEPTR base;
    // short i;
    
    struct NewScreen NewGameScreen = {
        0, 0, 
        SWIDTH, SHEIGHT, 
        SDEPTH,
        7, 4,
        0x0000,
        CUSTOMSCREEN | CUSTOMBITMAP,
        NULL,
        (UBYTE *) "Ultima Walking Test, V. 2.1",
        NULL,
        NULL
    };

    struct NewWindow FirstNewWindow;

    // Open libraries...

    if(!(IntuitionBase = (struct IntuitionBase *) OpenLibrary("intuition.library", 0L))) {
        Close_All(FALSE);
    }
        
    if(!(GfxBase = (struct GfxBase *) OpenLibrary("graphics.library", 0L))) {
        Close_All(FALSE);
    }
    
    // Allocate a bitmap for our game screen...

    if(!(GameMap = allocbitmap(SDEPTH, SWIDTH, SHEIGHT))) {
        Close_All(FALSE);
    }
    
    // Create a screen for the game...

    // InitBitMap(&ScrMap, SDEEP, SWIDE, SHIGH);
    // ScrMap.BytesPerRow *= SDEEP;
    // base = GameMap->Planes[0];
    // for(i = 0; i < SDEEP; i++) {
    //     ScrMap.Planes[i] = base;
    //     base += GameMap->BytesPerRow;
    // }
    
    NewGameScreen.CustomBitMap = GameMap;
    if(!(GameScreen = (struct Screen *) OpenScreen(&NewGameScreen))) {
        Close_All(FALSE);
    }
    
    // Create a window for receiving keyboard events...

    memset(&FirstNewWindow, 0, sizeof(FirstNewWindow));

    FirstNewWindow.Width = SWIDTH;
    FirstNewWindow.Height = SHEIGHT;
    FirstNewWindow.DetailPen = -1;
    FirstNewWindow.BlockPen = -1;
    FirstNewWindow.IDCMPFlags = RAWKEY;
    FirstNewWindow.Flags = NOCAREREFRESH | BACKDROP | BORDERLESS | ACTIVATE;
    FirstNewWindow.Title = "Walk 2.1";
    FirstNewWindow.Screen = GameScreen;
    FirstNewWindow.MinWidth = SWIDTH;
    FirstNewWindow.MinHeight = SHEIGHT;
    FirstNewWindow.MaxWidth = SWIDTH;
    FirstNewWindow.MaxHeight = SHEIGHT;
    FirstNewWindow.Type = CUSTOMSCREEN;

    if(!(FirstWindow = (struct Window *) OpenWindow(&FirstNewWindow))) {
        Close_All(FALSE);
    }
    
    // Set the screen color palette...

    LoadRGB4(ViewPortAddress(FirstWindow), &Colors[0], 32);

    // Build the avatar alpha mask
    avatarMask = buildAvatarMask(Avatar, 5);
}

/****************/
/*              */
/* Close All... */
/*              */
/****************/

void Close_All(int tf) {
    if(avatarMask) {
        freeAvatarMask(avatarMask);
    }
    if(FirstWindow) {
        CloseWindow(FirstWindow);
    }
    if(GameScreen) {
        CloseScreen(GameScreen);
    }
    if(GameMap) {
        freebitmap(GameMap);
    }
    if(GfxBase) {
        CloseLibrary((struct Library *)GfxBase);
    }
    if(IntuitionBase) {
        CloseLibrary((struct Library *)IntuitionBase);
    }
    
    exit(tf);
}

/****************************/
/*                          */
/*  DrawAvatarOverTile      */
/*                          */
/****************************/

void DrawAvatarOverTile(USHORT *avatar, USHORT *tile, USHORT *mask, USHORT *output) {
    for(int i = 0; i < 80; i++) {
        output[i] = (avatar[i] & mask[i % 16]) | (tile[i] & ~mask[i % 16]);
    }
}

/****************/
/*              */
/* allocbitmap  */
/*              */
/****************/

struct BitMap * allocbitmap(LONG deep, LONG wide, LONG high) {
    struct BitMap *bm;
    LONG size;
    
    if (bm = (struct BitMap *) AllocMem (sizeof (*bm), MEMF_CLEAR)) {
        InitBitMap(bm, deep, wide, high);
        size = (LONG) bm->BytesPerRow * bm->Rows;
        
        while (deep--)
            if(!(bm->Planes[deep] = (PLANEPTR) AllocMem(size, MEMF_CHIP))) {
                freebitmap(bm);
                return(NULL);
            }
        
        BltBitMap(bm, 0, 0, bm, 0, 0, wide, high, 0, -1, NULL);
    }

    return bm;
}

/****************/
/*              */
/* freebitmap   */
/*              */
/****************/

void freebitmap(struct BitMap *bm) {
    PLANEPTR *p;
    LONG size;
    short deep;
    
    size = (LONG) bm->BytesPerRow * bm->Rows;
    p = bm->Planes;
    deep = bm->Depth;
    
    while(deep--) {
        if (*p) {
            FreeMem(*p, size);
            *p = NULL;
        }
        p++;
    }

    bm->Depth = 0;
    FreeMem(bm, sizeof(*bm));
}

/********************/
/*                  */
/* buildAvatarMask  */
/*                  */
/********************/

USHORT *buildAvatarMask(USHORT *avatarTile, int bitPlaneCount) {
    USHORT *mask = (USHORT *)AllocMem(16 * sizeof(USHORT), MEMF_CHIP);

    for(int i = 0; i < 16; i++) {
        mask[i] = 0x0000;
        for(int p = 0; p < bitPlaneCount; p++) {
            mask[i] |= avatarTile[i + 16*p];
        }
    }

    return mask;
}

/********************/
/*                  */
/* freeAvatarMask   */
/*                  */
/********************/

void freeAvatarMask(USHORT *mask) {
    FreeMem(mask, 16 * sizeof(USHORT));
}
