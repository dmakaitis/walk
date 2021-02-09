/****************/
/*              */
/* Walking Test */
/*              */
/* Version 0.7  */
/*              */
/* 09/29/91     */
/*              */
/****************/

#include    <exec/types.h>
#include    <intuition/intuition.h>

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
/*  Wall Gfx Data       */
/*                      */
/************************/

USHORT  Wall[] =
{
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    
    0x0000,
    0xFFF7,
    0xFFF7,
    0xFFF7,
    0x0000,
    0xF7FF,
    0xF7FF,
    0xF7FF,
    0x0000,
    0xFFF7,
    0xFFF7,
    0xFFF7,
    0x0000,
    0xF7FF,
    0xF7FF,
    0xF7FF
};

/************************/
/*                      */
/*  Grass Gfx Data      */
/*                      */
/************************/

USHORT  Grass[] =
{
    0x2002,
    0x0110,
    0x0000,
    0x2080,
    0x0008,
    0x0401,
    0x0000,
    0x0002,
    0x0100,
    0x4010,
    0x0000,
    0x0400,
    0x0001,
    0x2000,
    0x0010,
    0x0100,
    
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000
};

/************************/
/*                      */
/*  Player Gfx Data     */
/*                      */
/************************/

USHORT  Avatar[] =
{
    0x01C0,
    0x01C0,
    0x01C0,
    0x0080,
    0x1F70,
    0x1EB8,
    0x1F58,
    0x1F28,
    0x0ED4,
    0x05E8,
    0x03D4,
    0x03E0,
    0x0770,
    0x0770,
    0x0770,
    0x0770,
    
    0x01C0,
    0x01C0,
    0x01C0,
    0x0080,
    0x1F70,
    0x1EB8,
    0x1F58,
    0x1F28,
    0x0ED4,
    0x05E8,
    0x03D4,
    0x03E0,
    0x0770,
    0x0770,
    0x0770,
    0x0770
};
    
struct  IntuitionBase   *IntuitionBase;
struct  Window      *FirstWindow;

struct  NewWindow   FirstNewWindow =
{
    230, 110,                                   /* XPos, YPos       */
    130, 130,                                   /* Width, Height    */
    0, 1,                                       /* Pen Colors       */
    VANILLAKEY | NEWSIZE | CLOSEWINDOW,         /* IDCME Flags      */
    SMART_REFRESH | ACTIVATE | WINDOWDEPTH |    /* Window Flags     */ 
    WINDOWSIZING | WINDOWDRAG | WINDOWCLOSE,
    NULL,                                       /* Gadget Ptr       */
    NULL,                                       /* Chk Mrk Gfx Ptr  */
    (UBYTE *) "Walking 0.7",                    /* Window Title Ptr */
    NULL,                                       /* Screen Ptr       */
    NULL,                                       /* BitMap Ptr       */
    130, 130,                                   /* Min Width/Height */
    320, 200,                                   /* Max Width/Height */
    WBENCHSCREEN,                               /* Window Type      */
};

struct  Image   Output =
{
    0, 0,       /* Left, Top Edge       */
    16, 16,     /* Width, Height        */
    2,          /* Depth (Bit Planes)   */
    &Avatar[0], /* Image Ptr            */
    3, 0,       /* Plane Pick, On/Off   */
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
    int exit = 0;
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
            if(code == 'i')
            {
                ty = y - 1;
                if(ty < 0) ty = ty + 16;
            }
            
            if(code == 'k')
            {
                ty = y + 1;
                if(ty > 15) ty = ty - 16;
            }
            
            if(code == 'j')
            {
                tx = x - 1;
                if(tx < 0) tx = tx + 16;
            }
            
            if(code == 'l')
            {
                tx = x + 1;
                if(tx > 15) tx = tx - 16;
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

    MyWindowsRastPort = FirstWindow->RPort;
    
/*  printf("\n%d, %d\n", x, y); */
    for(dy = y - 2, cy = 0; dy <= y + 2; dy++, cy++)
    {
        yy = dy;
        
        if(yy < 0) yy = yy + 16;
        if(yy > 15) yy = yy - 16;
    
        for(dx = x - 2, cx = 0; dx <= x + 2; dx++, cx++)
        {
            xx = dx;
            
            if(xx < 0) xx = xx + 16;
            if(xx > 15) xx = xx - 16;
            
            if(map[yy] [xx] == '*') Output.ImageData = &Wall[0];
            if(map[yy] [xx] == '.') Output.ImageData = &Grass[0];
            Output.LeftEdge = (5 + 16 * cx);
            Output.TopEdge = (5 + 16 * cy);
            DrawImage(MyWindowsRastPort, &Output, 10L, 10L);
/*          printf("%c", map[yy] [xx]); */
        }
/*      printf("  %d, %d\n", xx, yy);   */
    }
    Output.LeftEdge = 37;
    Output.TopEdge = 37;
    Output.ImageData = &Avatar[0];
    DrawImage(MyWindowsRastPort, &Output, 10L, 10L);
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
    
    if (!(FirstWindow = (struct Window *) OpenWindow(&FirstNewWindow)))
    {
        printf("Window will not open!\n");
        Close_All(FALSE);
    }
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
    if (IntuitionBase)  CloseLibrary(IntuitionBase);

    exit(tf);
}