/****************/
/*              */
/* Walking Test */
/*              */
/* Version 0.3  */
/*              */
/* 09/28/91     */
/*              */
/****************/

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
void    main()
{
    int x = 8;
    int y = 8;
    int tx, ty;
    int exit = 0;
    char    input;
                
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
}


draw(x,y)

register int x, y;

{
    int     xx, yy;

    printf("\n%d, %d\n\n", x, y);
    for(yy = y - 2; yy <= y + 2; yy++)
    {
        for(xx = x - 2; xx <= x + 2; xx++)
            printf("%c", map[yy] [xx]);
        printf("\n");
    }
    printf("\n%c\n:", map[y] [x]);
}
