/****************/
/*              */
/* Walking Test */
/*              */
/* Version 1.1  */
/*              */
/* 09/29/91     */
/*              */
/****************/

#include    <exec/types.h>
#include    <intuition/intuition.h>
#include    <graphics/gfx.h>
#define     MAPSIZE 16

char    map[17] [17] = {
            ".***************",
            ".***************",
            "....*****.**..**",
            "***...***....***",
            "***.*.*...**..**",
            "**..*.*.*****.**",
            "**.**.*..*....**",
            "**....**...**.**",
            "***.*****.**..**",
            "***.*...*..*.***",
            "**..*.*...*...**",
            "**.**...***.****",
            "**.******.....**",
            "**........***.**",
            "*************.**",
            ".************...",
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
/*  Grass Gfx Data      */
/*                      */
/************************/

USHORT  Grass[] =
{
    0x5C00, 0xA67C, 0x513C, 0x286C,
    0x241C, 0x996C, 0x46FC, 0x227C,
    0x106C, 0x447C, 0x446C, 0x52DC,
    0x786C, 0x7D7C, 0x0002, 0x0000,
    
    0x33FF, 0x19FF, 0x8EFF, 0xC77F,
    0xC6FF, 0x67FF, 0x69FF, 0xDDFF,
    0xEF7F, 0xF6FF, 0xF6FF, 0xFDFF,
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
    0x23F0, 0x2FF8, 0x38F8, 0x1B98,
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
/*  Chr Sil Buffer      */
/*                      */
/************************/

USHORT  Sil[]   =
{
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000
};
    
struct  GfxBase     *GfxBase;
struct  IntuitionBase   *IntuitionBase;
struct  Window      *FirstWindow;
struct  Screen      *GameScreen;
struct  ViewPort    *GamePort;

struct  NewScreen   NewGameScreen =
{
    0, 0,
    320, 200,
    5,
    7, 4,
    NULL,
    CUSTOMSCREEN,
    NULL,
    (UBYTE *)"Ultima Walking Test, V. 1.1",
    NULL,
    NULL,
};

struct  NewWindow   FirstNewWindow =
{
    50, 50,                                     /* XPos, YPos       */
    130, 130,                                   /* Width, Height    */
    8, 10,                                      /* Pen Colors       */
    VANILLAKEY | NEWSIZE | CLOSEWINDOW,         /* IDCME Flags      */
    SMART_REFRESH | ACTIVATE | WINDOWDEPTH |    /* Window Flags     */ 
    WINDOWSIZING | WINDOWDRAG | WINDOWCLOSE,
    NULL,                                       /* Gadget Ptr       */
    NULL,                                       /* Chk Mrk Gfx Ptr  */
    (UBYTE *) "Walk 1.1",                       /* Window Title Ptr */
    NULL,                                       /* Screen Ptr       */
    NULL,                                       /* BitMap Ptr       */
    130, 130,                                   /* Min Width/Height */
    320, 200,                                   /* Max Width/Height */
    CUSTOMSCREEN,                               /* Window Type      */
};

struct  Image   Output =
{
    0, 0,       /* Left, Top Edge       */
    16, 16,     /* Width, Height        */
    5,          /* Depth (Bit Planes)   */
    &Avatar[0], /* Image Ptr            */
    31, 0,      /* Plane Pick, On/Off   */
    NULL        /* Next Image Prt       */
};

struct  IntuiMessage    *message;

/************************/
/*                      */
/* Main Program Loop    */
/*                      */
/************************/

void    main()

{
    int x = 9;
    int y = 8;
    int tx, ty;
    char    code;
    ULONG   MessageClass;
                
    Open_All();
    
    draw(x,y);
    FOREVER
    {
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
        
        if(MessageClass & NEWSIZE) draw(x,y);
        
        if(MessageClass & VANILLAKEY)
        {
            if(code == '8')
            {
                ty = y - 1;
                if(ty < 0) ty = ty + MAPSIZE;
            }
            
            if(code == '2')
            {
                ty = y + 1;
                if(ty > MAPSIZE-1) ty = ty - MAPSIZE;
            }
            
            if(code == '4')
            {
                tx = x - 1;
                if(tx < 0) tx = tx + MAPSIZE;
            }
            
            if(code == '6')
            {
                tx = x + 1;
                if(tx > MAPSIZE-1) tx = tx - MAPSIZE;
            }
            
            if(code == '7')
            {
                ty = y - 1;
                if(ty < 0) ty = ty + MAPSIZE;
                tx = x - 1;
                if(tx < 0) tx = tx + MAPSIZE;
            }
            
            if(code == '9')
            {
                ty = y - 1;
                if(ty < 0) ty = ty + MAPSIZE;
                tx = x + 1;
                if(tx > MAPSIZE-1) tx = tx - MAPSIZE;
            }
            
            if(code == '1')
            {
                ty = y + 1;
                if(ty > MAPSIZE-1) ty = ty - MAPSIZE;
                tx = x - 1;
                if(tx < 0) tx = tx + MAPSIZE;
            }
            
            if(code == '3')
            {
                ty = y + 1;
                if(ty > MAPSIZE-1) ty = ty - MAPSIZE;
                tx = x + 1;
                if(tx > MAPSIZE-1) tx = tx - MAPSIZE;
            }
            
            if(map [ty] [tx] == '*')
            {
                DisplayBeep(NULL);
/*              printf("\nBlocked!!\n");    */
            }
            else
            {
                y = ty;
                x = tx; 
            }                   
            
            draw(x,y);
        }
        
        if(MessageClass & CLOSEWINDOW) Close_All(TRUE);
    }
}


/****************************************/
/*                                      */
/* Draw Screen                          */
/*                                      */
/* draw(x,y) : x = XPos, Y = YPos       */
/*                                      */
/****************************************/

draw(x,y)

register int x, y;

{
    struct  RastPort    *MyWindowsRastPort;
    
    int     xx, yy, cx, cy, dx, dy;
    USHORT  *ChrBakImageData;

    MyWindowsRastPort = FirstWindow->RPort;
    
/*  printf("\n%d, %d\n", x, y); */
    for(dy = y - 2, cy = 0; dy <= y + 2; dy++, cy++)
    {
        yy = dy;
        
        if(yy < 0) yy = yy + MAPSIZE;
        if(yy > MAPSIZE-1) yy = yy - MAPSIZE;
    
        for(dx = x - 2, cx = 0; dx <= x + 2; dx++, cx++)
        {
            xx = dx;
            
            if(xx < 0) xx = xx + MAPSIZE;
            if(xx > MAPSIZE-1) xx = xx - MAPSIZE;
            
            if(map[yy] [xx] == '*') Output.ImageData = &Wall[0];
            if(map[yy] [xx] == '.') Output.ImageData = &Grass[0];
            if(xx == x && yy == y)
            {
                ChrBakImageData = Output.ImageData;
                DrawPlayer(ChrBakImageData);
            }
                        
            Output.LeftEdge = (5 + 16 * cx);
            Output.TopEdge = (5 + 16 * cy);
            DrawImage(MyWindowsRastPort, &Output, 10L, 10L);
/*          printf("%c", map[yy] [xx]); */
        }
        DrawImage(MyWindowsRastPort, &Output, 10L, 10L);
            
/*      printf("  %d, %d\n", xx, yy);   */
    }
}

/********************************/
/*                              */
/* Open Intuition and Window    */
/*                              */
/********************************/

Open_All()
{
    void    *OpenLibrary();
    struct  Window  *OpenWindow();
    
    if (!(IntuitionBase = (struct IntuitionBase *) OpenLibrary("intuition.library", 0L)))
    {
        printf("Intuition Library not found!\n");
        Close_All(FALSE);
    }
    
    if (!(GfxBase = (struct GfxBase *) OpenLibrary("graphics.library", 0L)))
    {
        printf("Graphics Library not found!\n");
        Close_All(FALSE);
    }
    
    if(!(GameScreen = (struct Screen *) OpenScreen(&NewGameScreen)))
    {
        printf("Screen cannot be opened!\n");
        Close_All(FALSE);
    }
    
    FirstNewWindow.Screen = GameScreen;
    
    if (!(FirstWindow = (struct Window *) OpenWindow(&FirstNewWindow)))
    {
        printf("Window will not open!\n");
        Close_All(FALSE);
    }
    
    GamePort = (struct ViewPort *) ViewPortAddress(FirstWindow);
    LoadRGB4(GamePort, &Colors[0], 32);
}

/********************************/
/*                              */
/* Close Intuition and Window   */
/*                              */
/********************************/

Close_All(tf)

register    int tf;

{
    if (FirstWindow)    CloseWindow(FirstWindow);
    if (GameScreen)     CloseScreen(GameScreen);
    if (GfxBase)        CloseLibrary(GfxBase);
    if (IntuitionBase)  CloseLibrary(IntuitionBase);

    exit(tf);
}

/************************/
/*                      */
/*  Draw Players        */
/*  V1.1                */
/*                      */
/*  DrawPlayer(o)       */
/*                      */
/************************/

DrawPlayer(o)

register    USHORT  *o;

{
    int c, c2;
    
    for(c = 0; c <= 15; c++)
    {
        Sil[c] = ~(Avatar[c] | Avatar[16 + c] | Avatar[32 + c] |
               Avatar[48 + c] | Avatar[64 + c]);
        for(c2 = 0; c2 <= 64; c2 = c2 + 16)
            Chr[c + c2] = (*(o + c + c2) & Sil[c]) | Avatar[c + c2];
    }
    Output.ImageData = &Chr[0];
}




