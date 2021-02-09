/****************/
/*              */
/* Walking Test */
/*              */
/* Version 0.4  */
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
    90, 90,
    0, 1,
    NULL,
    SMART_REFRESH,
    NULL,
    NULL,
    (UBYTE *) "Walking 0.4",
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

void    main()

{
    int x = 8;
    int y = 8;
    int tx, ty;
    int exit = 0;
    char    input;
                
    Open_All();
    
    draw(x,y);
    do
    {
        tx = x;
        ty = y;
        
        exit = 1;
        
        scanf("%c", &input);
        
        if(input == 'i')
        {
            if(y > 2)
                ty = y - 1;
        }
        
        if(input == 'k')
        {
            if(y < 13)
                ty = y + 1;
        }
        
        if(input == 'j')
        {
            if(x > 2)
                tx = x - 1;
        }
        
        if(input == 'l')
        {
            if(x < 13)
                tx = x + 1;
        }
        
        if(input != 'q')
        {
            exit = 0;
            if(map [ty] [tx] == '*')
                printf("\nBlocked!!\n");
            else
            {
                y = ty;
                x = tx;
            }
            draw(x,y);
        }
    } while(exit == 0);
    Close_All();
}


draw(x,y)

register int x, y;

{
    struct  RastPort    *MyWindowsRastPort;
    
    char    *Output = "*";
    
    int     xx, yy, cx, cy;

    MyWindowsRastPort = FirstWindow->RPort;
    
    printf("\n%d, %d\n\n", x, y);
    for(yy = y - 2, cy = 0; yy <= y + 2; yy++, cy++)
    {
        for(xx = x - 2, cx = 0; xx <= x + 2; xx++, cx++)
        {
            *Output = map[yy] [xx];
            FirstText.LeftEdge = (5 + 10 * cx);
            FirstText.TopEdge = (5 + 8 * cy);
            FirstText.IText = (UBYTE *) Output;
            PrintIText(MyWindowsRastPort, &FirstText, 10L, 10L);
            printf("%c", map[yy] [xx]);
        }
        printf("\n");
    }
    printf("\n%c\n:", map[y] [x]);
}

Open_All()
{
    void    *OpenLibrary();
    struct  Window  *OpenWindow();
    
    if (!(IntuitionBase = (struct IntuitionBase *) OpenLibrary("intuition.library", 0L)))
    {
        printf("Intuition Library not found!\n");
        Close_All();
        exit(FALSE);
    }
    
    if (!(FirstWindow = (struct Window *) OpenWindow(&FirstNewWindow)))
    {
        printf("Window will not open!\n");
        Close_All();
        exit(FALSE);
    }
}

Close_All()
{
    if (FirstWindow)    CloseWindow(FirstWindow);
    if (IntuitionBase)  CloseLibrary(IntuitionBase);
}
