/****************/
/*              */
/* Walking Test */
/*              */
/* Version 0.6  */
/*              */
/* 09/29/91     */
/*              */
/****************/

#include    <exec/types.h>
#include    <intuition/intuition.h>

char    map[17] [17] = {
            "****************",
            "****************",
            "**..*****.**..**",
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
            "****************",
            "****************",
               };

USHORT  Wall[] =
{
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
    0x0100
};

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
    0x0770
};
    
struct  IntuitionBase   *IntuitionBase;
struct  Window      *FirstWindow;

struct  NewWindow   FirstNewWindow =
{
    230, 110,
    130, 130,
    0, 1,
    VANILLAKEY,
    SMART_REFRESH | ACTIVATE,
    NULL,
    NULL,
    (UBYTE *) "Walking 0.6",
    NULL,
    NULL,
    90, 90,
    90, 90,
    WBENCHSCREEN,
};

struct  Image   Output =
{
    0, 0,
    16, 16,
    1,
    &Avatar[0],
    1, 0,
    NULL
};

struct  IntuiMessage    *message;

void    main()

{
    int x = 9;
    int y = 8;
    int tx, ty;
    int exit = 0;
    char    code;
                
    Open_All();
    
    draw(x,y);
    FOREVER
    {
        tx = x;
        ty = y;
        
        exit = 1;
        
        if ((message = (struct IntuiMessage *) GetMsg(FirstWindow->UserPort)) == NULL)
        {
            Wait(1L << FirstWindow->UserPort->mp_SigBit);
            continue;
        }
        
        code = message->Code;
        
        if(code == 'i')
        {
            if(y > 2)
                ty = y - 1;
        }
        
        if(code == 'k')
        {
            if(y < 13)
                ty = y + 1;
        }
        
        if(code == 'j')
        {
            if(x > 2)
                tx = x - 1;
        }
        
        if(code == 'l')
        {
            if(x < 13)
                tx = x + 1;
        }
        
        if(code != 'q')
        {
            exit = 0;
            if(map [ty] [tx] == '*')
            {
                DisplayBeep(NULL);
                printf("\nBlocked!!\n");
            }
            else
            {
                y = ty;
                x = tx;
            }
            draw(x,y);
        }
        else
            Close_All(TRUE);
    }
}


draw(x,y)

register int x, y;

{
    struct  RastPort    *MyWindowsRastPort;
    
    int     xx, yy, cx, cy;

    MyWindowsRastPort = FirstWindow->RPort;
    
    printf("\n%d, %d\n", x, y);
    for(yy = y - 2, cy = 0; yy <= y + 2; yy++, cy++)
    {
        for(xx = x - 2, cx = 0; xx <= x + 2; xx++, cx++)
        {
            if(map[yy] [xx] == '*') Output.ImageData = &Wall[0];
            if(map[yy] [xx] == '.') Output.ImageData = &Grass[0];
            Output.LeftEdge = (5 + 16 * cx);
            Output.TopEdge = (5 + 16 * cy);
            DrawImage(MyWindowsRastPort, &Output, 10L, 10L);
        }
    }
    Output.LeftEdge = 37;
    Output.TopEdge = 37;
    Output.ImageData = &Avatar[0];
    DrawImage(MyWindowsRastPort, &Output, 10L, 10L);
}

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

Close_All(tf)

register    int tf;

{
    if (FirstWindow)    CloseWindow(FirstWindow);
    if (IntuitionBase)  CloseLibrary(IntuitionBase);

    exit(tf);
}

