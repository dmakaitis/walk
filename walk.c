/************************/
/*                      */
/*  Walking Test        */
/*                      */
/*  Version 2.0         */
/*                      */
/*  02/12/92            */
/*                      */
/************************/

#include    <exec/types.h>
#include    <exec/memory.h>
#include    <intuition/intuition.h>
#include    <graphics/gfx.h>
#define     MAPSIZE 32
#define     SWIDE   320
#define     SHIGH   320
#define     SDEEP   5

char    map[33] [33] = {
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

USHORT  Colors[] =
{
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

USHORT  Wall[] =
{
    0xffff, 0xffff, 0xffff, 0x0000, 0xffff,
    0xbf77, 0x9375, 0x6c8e, 0x0000, 0xffff,
    0xfbff, 0xdb83, 0x6c7c, 0x0000, 0xffff,
    0xffff, 0x01b6, 0xfe49, 0x0000, 0xffff,
    
    0x6fda, 0xfd27, 0x02dc, 0x0000, 0xffff,
    0xdf7f, 0x9341, 0x6cbe, 0x0000, 0xffff,
    0xfd83, 0xb6cb, 0x4934, 0x0000, 0xffff,
    0xffff, 0xffff, 0xffff, 0x0000, 0xffff,
    
    0xd97f, 0x0056, 0xffad, 0x0000, 0xffff,
    0x9933, 0x9131, 0x6ece, 0x0000, 0xffff,
    0xffef, 0xda27, 0x25d8, 0x0000, 0xffff,
    0xefff, 0xb4f7, 0x4b08, 0x0000, 0xffff,
    
    0xefdf, 0xd9cf, 0x2630, 0x0000, 0xffff,
    0x96ee, 0xd935, 0x0000, 0x0000, 0xffff,
    0xffff, 0xffff, 0xffff, 0x0000, 0xffff,
    0x0000, 0x0000, 0x0000, 0x0000, 0xffff
};

/************************/
/*                      */
/*  Floor Gfx Data      */
/*                      */
/************************/

USHORT  Grass[] =
{
    0x5C00, 0x33FF, 0x0000, 0xFFFE, 0x0000,
    0xa67c, 0x19ff, 0x0000, 0xfffe, 0x0000,
    0x513c, 0x8eff, 0x0000, 0xfffe, 0x0000,
    0x286c, 0xc77f, 0x0000, 0xfffe, 0x0000,
    
    0x241c, 0xcbff, 0x0000, 0xfffe, 0x0000,
    0x99bc, 0x67ff, 0x0000, 0xfffe, 0x0000,
    0x46fc, 0xbdff, 0x0000, 0xfffe, 0x0000,
    0x227c, 0xddff, 0x0000, 0xfffe, 0x0000,
    
    0x10bc, 0xef7f, 0x0000, 0xfffe, 0x0000,
    0x447c, 0xfcff, 0x0000, 0xfffe, 0x0000,
    0x44bc, 0xfbff, 0x0000, 0xfffe, 0x0000,
    0x52dc, 0xfdff, 0x0000, 0xfffe, 0x0000,
    
    0x78bc, 0xffff, 0x0000, 0xfffe, 0x0000,
    0x7d7c, 0xffff, 0x0000, 0xfffe, 0x0000,
    0x0002, 0xfffd, 0x0000, 0xfffe, 0x0000,
    0x0000, 0xfffe, 0x0000, 0x0001, 0x0000
};

/************************/
/*                      */
/*  Player Gfx Data     */
/*                      */
/************************/

USHORT  Avatar[] =
{
    0x0000, 0x0800, 0x1000, 0x28A0, 0x1FF0,
    0x3718, 0x1260, 0x0980, 0x0030, 0x0A90, 
    0x0158, 0x015C, 0x006C, 0x006C, 0x0000, 
    0x0000, 0x7C00, 0x7E00, 0xFFE0, 0xF430,
    
    0x6218, 0x5EC4, 0x47E4, 0x6F84, 0x707C, 
    0x37FE, 0x1FEE, 0x0FA6, 0x01B6, 0x00FE, 
    0x006C, 0x0000, 0x3000, 0x3C00, 0x7800, 
    0x7380, 0x2380, 0x2FC0, 0x1FE0, 0x0BE0,
    
    0x03E0, 0x01B0, 0x00D8, 0x005C, 0x006C, 
    0x006C, 0x0000, 0x0000, 0x3000, 0x3C00, 
    0x7800, 0x73E0, 0x23F0, 0x2FF8, 0x3FF8, 
    0x1B98, 0x0C60, 0x09B0, 0x00C0, 0x0000,
    
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0860, 0x1E70, 0x0E38, 
    0x2618, 0x1278, 0x0F80, 0x0800, 0x0018, 
    0x005C, 0x006C, 0x006C, 0x0000, 0x0000
};
    
/********************************/
/*                              */
/*  Chr Output Buffer           */
/*                              */
/********************************/

USHORT  Chr[] =
{
    0xffff, 0xffff, 0xffff, 0xffff,
    0xc003, 0xc003, 0xc003, 0xc003,
    0xc003, 0xc003, 0xc003, 0xc003,
    0xffff, 0xffff, 0xffff, 0xffff,
    
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
    
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
    
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
    
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000
};

/************************/
/*                      */
/*  Chr Sil Biffer      */
/*                      */
/************************/

USHORT  Sil[] =
{
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000
};

ULONG   Output;

struct  BitMap      *allocbitmap(LONG, LONG, LONG);

/* struct   BitMap      Sil, Chr, Avatar, Grass, Wall; */
struct  IntuitionBase   *IntuitionBase;
struct  GfxBase     *GfxBase;
struct  Window      *FirstWindow;
struct  Screen      *GameScreen;
struct  BitMap      *GameMap;

struct  NewScreen   NewGameScreen = 
{
    0, 0, 
    SWIDE, SHIGH, 
    SDEEP,
    7, 4,
    NULL,
    CUSTOMSCREEN | CUSTOMBITMAP,
    NULL,
    (UBYTE *) "Ultima Walking Test, V. 2.0",
    NULL,
    NULL
};

struct  NewWindow   FirstNewWindow =
{
    0, 0,
    10, 10,
    8, 10,
    RAWKEY | CLOSEWINDOW,
    SMART_REFRESH | ACTIVATE | WINDOWDEPTH | WINDOWDRAG | WINDOWCLOSE,
    NULL,
    NULL,
    (UBYTE *) "Walk 2.0",
    NULL,
    NULL,
    10, 10,
    320, 200,
    CUSTOMSCREEN,
};

struct  IntuiMessage    *message;

/************************/
/*                      */
/* Main Program Loop    */
/*                      */
/************************/

void main()

{
    int x = 16;
    int y = 16;
    int tx, ty;
    char    code;
    ULONG   MessageClass;
    
    Open_All();
    
    FOREVER
    {
        draw(x, y);
        
        tx = x;
        ty = y;
        
        if ((message = (struct IntuiMessage *) GetMsg(FirstWindow->UserPort)) == NULL)
        {
            Wait(1L << FirstWindow->UserPort->mp_SigBit);
            continue;
        }
        
        ReplyMsg(message);
        
        code = message->Code;
        MessageClass = message->Class;
        
        if(MessageClass & RAWKEY)
        {
            if(code == 0x3e)
            {
                ty = y - 1;
                if(ty < 0) ty = ty + MAPSIZE;
            }
            
            if(code == 0x1e)
            {
                ty = y + 1;
                if(ty > MAPSIZE-1) ty = ty - MAPSIZE;
            }
            
            if(code == 0x2d)
            {
                tx = x - 1;
                if(tx < 0) tx = tx + MAPSIZE;
            }
            
            if(code == 0x2f)
            {
                tx = x + 1;
                if(tx > MAPSIZE-1) tx = tx - MAPSIZE;
            }
            
            if(code == 0x3d)
            {
                ty = y - 1;
                if(ty < 0) ty = ty + MAPSIZE;
                tx = x - 1;
                if(tx < 0) tx = tx + MAPSIZE;
            }
            
            if(code == 0x3f)
            {
                ty = y - 1;
                if(ty < 0) ty = ty + MAPSIZE;
                tx = x + 1;
                if(tx > MAPSIZE-1) tx = tx - MAPSIZE;
            }
            
            if(code == 0x1d)
            {
                ty = y + 1;
                if(ty > MAPSIZE-1) ty = ty - MAPSIZE;
                tx = x - 1;
                if(tx < 0) tx = tx + MAPSIZE;
            }
            
            if(code == 0x1f)
            {
                ty = y + 1;
                if(ty > MAPSIZE-1) ty = ty - MAPSIZE;
                tx = x + 1;
                if(tx > MAPSIZE-1) tx = tx - MAPSIZE;
            }
            
            if(code == 0x45) Close_All(TRUE);
                
            if(((map [ty] [x] == '*') && (map [y] [tx] == '*')) || (map [ty] [tx] == '*'))
                DisplayBeep(NULL);
            else
            {
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

draw(x,y)

register int    x, y;

{
    int xx, yy, cx, cy, dx, dy, Ox, Oy, Oh;
    
    for(dy = y - 4, cy = 0; dy <= y + 4; dy++, cy++)
    {
        yy = dy;
        
        if(yy < 0) yy = yy + MAPSIZE;
        if(yy > MAPSIZE-1) yy = yy - MAPSIZE;
        
        for(dx = x - 4, cx = 0; dx <= x + 4; dx++, cx++)
        {
            xx = dx;
            
            if(xx < 0) xx = xx + MAPSIZE;
            if(xx > MAPSIZE-1) xx = xx - MAPSIZE;
            
            if(map[yy] [xx] == '*') Output = &Wall[0];
            if(map[yy] [xx] == '.') Output = &Grass[0];
            
    /* Chr Display here.........*/
    
            Ox = (8 + 16 * cx);
            Oy = 5 * (8 + 16 * cy);
            BltBitMap (Output, 0, 0, GameMap, Ox, Oy, 16, 16 * Oh, 0xc0, -1, NULL);
        }
    }
}

/****************/
/*              */
/* Open up...   */
/*              */
/****************/

Open_All()

{
    void        *OpenLibrary();
    struct      Window      *OpenWindow();
    struct      BitMap      ScrMap;
    register    PLANEPTR    base;
    register    short       i;
    
    if(!(IntuitionBase = (struct IntuitionBase *) OpenLibrary("intuition.library", 0L)))
        Close_All(FALSE);
        
    if(!(GfxBase = (struct GfxBase *) OpenLibrary("graphics.library", 0L)))
        Close_All(FALSE);
    
    if(!(GameMap = allocbitmap(1, SWIDE, SHIGH * SDEEP)))
        Close_All(FALSE);
    
    InitBitMap(&ScrMap, SDEEP, SWIDE, SHIGH);
    ScrMap.BytesPerRow *= SDEEP;
    base = GameMap->Planes[0];
    for(i = 0; i < SDEEP; i++)
    {
        ScrMap.Planes[i] = base;
        base += GameMap->BytesPerRow;
    }
    
    NewGameScreen.CustomBitMap = &ScrMap;
    if(!(GameScreen = (struct Screen *) OpenScreen(&NewGameScreen)))
        Close_All(FALSE);
    
    FirstNewWindow.Screen = GameScreen;
    
    if(!(FirstWindow = (struct Window *) OpenWindow(&FirstNewWindow)))
        Close_All(FALSE);
    
    LoadRGB4((struct ViewPort *) ViewPortAddress(FirstWindow), &Colors[0], 32);
}

/****************/
/*              */
/* Close All... */
/*              */
/****************/

Close_All(tf)

register    int tf;

{
    if(FirstWindow)     CloseWindow(FirstWindow);
    if(GameScreen)      CloseScreen(GameScreen);
    if(GameMap)     freebitmap(GameMap);
    if(GfxBase)     CloseLibrary(GfxBase);
    if(IntuitionBase)   CloseLibrary(IntuitionBase);
    
    exit(tf);
}

/****************/
/*              */
/* allocbitmap  */
/*              */
/****************/

struct  BitMap  *

allocbitmap(deep, wide, high)

register    LONG    deep, wide, high;

{
    register    struct  BitMap  *bm;
    register    LONG        size;
    
    if (bm = AllocMem (sizeof (*bm), MEMF_CLEAR))
    {
        InitBitMap(bm, deep, wide, high);
        size = (LONG) bm->BytesPerRow * bm->Rows;
        
        while (deep--)
            if(!(bm->Planes[deep] = AllocMem(size, MEMF_CHIP)))
            {
                freebitmap(bm);
                return(NULL);
            }
        
        BltBitMap(bm, 0, 0, bm, 0, 0, wide, high, 0, -1, NULL);
    }
    return(bm);
}

/****************/
/*              */
/* freebitmap   */
/*              */
/****************/

freebitmap(bm)

register    struct  BitMap  *bm;

{
    register    PLANEPTR    *p;
    register    LONG        size;
    register    short       deep;
    
    size = (LONG) bm->BytesPerRow * bm->Rows;
    p = bm->Planes;
    deep = bm->Depth;
    
    while(deep--)
    {
        if (*p)
        {
            FreeMem(*p, size);
            *p = NULL;
        }
        p++;
    }
    bm->Depth = 0;
    FreeMem(bm, sizeof(*bm));
}


