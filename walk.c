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

#include <exec/exec.h>
#include <intuition/intuition.h>
#include <graphics/gfx.h>
#include <dos/dos.h>

#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>
#include <clib/graphics_protos.h>

#include "map.h"
#include "tiles.h"

#define SWIDTH 320
#define SHEIGHT 200
#define SDEPTH 5

/********************************/
/*                              */
/*  Chr Output Buffer           */
/*                              */
/********************************/

USHORT avatarBuffer[80];
USHORT *avatarMask = NULL;

struct IntuitionBase *IntuitionBase;
struct GfxBase *GfxBase;
struct DosBase *dosBase;
struct Library *iffBase;

struct Screen *GameScreen;
struct Window *FirstWindow;
struct BitMap *GameBitMap;

GameMap *gameMap = NULL;
Tiles *tiles = NULL;

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
        
        if ((message = (struct IntuiMessage *) GetMsg(FirstWindow->UserPort)) == NULL) {
            Wait(1L << FirstWindow->UserPort->mp_SigBit);
            continue;
        }
        
        ReplyMsg((struct Message *)message);
        
        code = message->Code;
        MessageClass = message->Class;
        
        if(MessageClass & RAWKEY)
        {
            tx = x;
            ty = y;
        
            switch(code) {
                case 0x3e:
                    ty = y - 1;
                    break;
                case 0x1e:
                    ty = y + 1;
                    break;
                case 0x2d:
                    tx = x - 1;
                    break;
                case 0x2f:
                    tx = x + 1;
                    break;
                case 0x3d:
                    ty = y - 1;
                    tx = x - 1;
                    break;
                case 0x3f:
                    ty = y - 1;
                    tx = x + 1;
                    break;
                case 0x1d:
                    ty = y + 1;
                    tx = x - 1;
                    break;
                case 0x1f:
                    ty = y + 1;
                    tx = x + 1;
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

            // Handle wrapping around the map if needed
            if(tx < 0) {
                tx += gameMap->width;
            }
            if(tx >= gameMap->width) {
                tx -= gameMap->width;
            }
            if(ty < 0) {
                ty += gameMap->height;
            }
            if(ty >= gameMap->height) {
                ty -= gameMap->height;
            }

            // Check to see if something is in the way
            if(collisionDetection && (((ReadMap(gameMap, x, ty) == '*') && (ReadMap(gameMap, tx, y) == '*')) || (ReadMap(gameMap, tx, ty) == '*'))) {
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
            switch(ReadMap(gameMap, x - 4 + drawX, y - 4 + drawY)) {
                case '*':
                    outImage.ImageData = GetTile(tiles, 1);
                    // bitmap.Planes[0] = (UBYTE *)(&Wall[0]);
                    // bitmap.Planes[1] = (UBYTE *)(&Wall[16]);
                    // bitmap.Planes[2] = (UBYTE *)(&Wall[32]);
                    // bitmap.Planes[3] = (UBYTE *)(&Wall[48]);
                    // bitmap.Planes[4] = (UBYTE *)(&Wall[64]);
                    break;
                case '.':
                    outImage.ImageData = GetTile(tiles, 0);
                    // bitmap.Planes[0] = (UBYTE *)(&Grass[0]);
                    // bitmap.Planes[1] = (UBYTE *)(&Grass[16]);
                    // bitmap.Planes[2] = (UBYTE *)(&Grass[32]);
                    // bitmap.Planes[3] = (UBYTE *)(&Grass[48]);
                    // bitmap.Planes[4] = (UBYTE *)(&Grass[64]);
                    break;
            }

            if(drawX == 4 && drawY == 4) {
                DrawAvatarOverTile(GetTile(tiles, 2), outImage.ImageData, avatarMask, avatarBuffer);
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
    
    if(!(dosBase = (struct DosBase *) OpenLibrary("dos.library", 0L))) {
        Close_All(FALSE);
    }

    if(!(iffBase = OpenLibrary("iffparse.library", 0L))) {
        Close_All(FALSE);
    }

    // Allocate a bitmap for our game screen...

    if(!(GameBitMap = allocbitmap(SDEPTH, SWIDTH, SHEIGHT))) {
        Close_All(FALSE);
    }
    
    // Create a screen for the game...

    // InitBitMap(&ScrMap, SDEEP, SWIDE, SHIGH);
    // ScrMap.BytesPerRow *= SDEEP;
    // base = GameBitMap->Planes[0];
    // for(i = 0; i < SDEEP; i++) {
    //     ScrMap.Planes[i] = base;
    //     base += GameBitMap->BytesPerRow;
    // }
    
    NewGameScreen.CustomBitMap = GameBitMap;
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
    
    // Load the game map
    gameMap = LoadGameMap("resources/castle.map");

    // Load the game tiles
    tiles = LoadTiles("resources/tiles.iff");

    // Build the avatar alpha mask
    avatarMask = buildAvatarMask(GetTile(tiles, 2), 5);

    // Set the screen color palette...
    LoadRGB4(ViewPortAddress(FirstWindow), tiles->palette, 32);
}

/****************/
/*              */
/* Close All... */
/*              */
/****************/

void Close_All(int tf) {
    if(tiles) {
        ReleaseTiles(tiles);
    }
    if(gameMap) {
        ReleaseGameMap(gameMap);
    }
    if(avatarMask) {
        freeAvatarMask(avatarMask);
    }
    if(FirstWindow) {
        CloseWindow(FirstWindow);
    }
    if(GameScreen) {
        CloseScreen(GameScreen);
    }
    if(GameBitMap) {
        freebitmap(GameBitMap);
    }
    if(iffBase) {
        CloseLibrary(iffBase);
    }
    if(dosBase) {
        CloseLibrary((struct Library *)dosBase);
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
