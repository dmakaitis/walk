/****************/
/*		*/
/* Walking Test	*/
/*		*/
/* Version 0.2	*/
/*		*/
/* 09/25/91	*/
/*		*/
/****************/

void	main()
{
	int	x = 8;
	int	y = 8;
	int	exit = 0;
	char	input;
				
	draw(x,y);
	do
	{
		exit = 1;
		
		scanf("%c", &input);
		
		if(input == 'i')
		{
			if(y > 2)
				y = y - 1;
		}
		
		if(input == 'k')
		{
			if(y < 13)
				y = y + 1;
		}
		
		if(input == 'j')
		{
			if(x > 2)
				x = x - 1;
		}
		
		if(input == 'l')
		{
			if(x < 13)
				x = x + 1;
		}
		
		if(input != 'q')
		{
			exit = 0;
			draw(x,y);
		}
	} while(exit == 0);
}


draw(x,y)

register int x, y;

{
	
	char	map[16] [16] = {
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

	int 	xx, yy;

	printf("\n%d, %d\n\n", x, y);
	for(yy = y - 2; yy <= y + 2; yy++)
	{
		for(xx = x - 2; xx <= x + 2; xx++)
			printf("%c", map[yy] [xx]);
		printf("\n");
	}
	printf("\n%c\n:", map[x] [y]);
}

			


