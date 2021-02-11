walk: walk.o map.o tiles.o
	vc +aos68k -lamiga -lauto -o $@ $^

.c.o:
	vc +aos68k -c -c99 -I${NDK_INC} $<

walk.o: walk.c map.h tiles.h

map.o: map.c map.h

tiles.o: tiles.c tiles.h

clean:
	rm walk
	rm *.o
