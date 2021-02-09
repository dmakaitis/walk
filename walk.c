/****************/
/*              */
/* Walking Test */
/*              */
/* Version 0.1  */
/*              */
/* 09/25/91     */
/*              */
/****************/

void    main()
{
    int x = 16;
    int y = 16;
    int exit = 0;
    char    input;
    
    printf("\n%d, %d\n\n:");
    do
    {
        exit = 1;
        
        scanf("%c", &input);
        
        if(input == 'i')
        {
            if(y > 0)
                y = y - 1;
        }
        
        if(input == 'k')
        {
            if(y < 32)
                y = y + 1;
        }
        
        if(input == 'j')
        {
            if(x > 0)
                x = x - 1;
        }
        
        if(input == 'l')
        {
            if(x < 32)
                x = x + 1;
        }
        
        if(input != 'q')
        {
            exit = 0;
            printf("\n%d, %d\n\n:", x, y);
        }
    } while(exit == 0);
}


