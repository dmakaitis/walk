/****************/
/*              */
/* Walking Test */
/*              */
/* Version 0.5  */
/*              */
/* 09/28/91     */
/*              */
/****************/

#include    <exec/types.h>
#include    <intuition/intuition.h>

char    map[16] [16] = {
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
    (UBYTE *) "Walking 0.5",
    NULL,
    NULL,
    90, 90,
    90, 90,
    WBENCHSCREEN,
};

struct  IntuiText   FirstText =
{
    1, 0,
    JAM2,
    0, 0,
    NULL,
    (UBYTE *) "x",
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
    
    char    *Output = "*";
    
    int     xx, yy, cx, cy;

    MyWindowsRastPort = FirstWindow->RPort;
    
    printf("\n%d, %d\n", x, y);
    for(yy = y - 2, cy = 0; yy <= y + 2; yy++, cy++)
    {
        for(xx = x - 2, cx = 0; xx <= x + 2; xx++, cx++)
        {
            *Output = map[yy] [xx];
            FirstText.LeftEdge = (5 + 10 * cx);
            FirstText.TopEdge = (5 + 8 * cy);
            FirstText.IText = (UBYTE *) Output;
            PrintIText(MyWindowsRastPort, &FirstText, 10L, 10L);
        }
    }
    *Output = 'A';
    FirstText.LeftEdge = 25;
    FirstText.TopEdge = 21;
    FirstText.IText = (UBYTE *) Output;
    FirstText.DrawMode = JAM1; 
    PrintIText(MyWindowsRastPort, &FirstText, 10L, 10L);
    FirstText.DrawMode = JAM2;
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

