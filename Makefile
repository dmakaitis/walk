walk: walk.o map.o
	vc +aos68k -lamiga -lauto -o $@ $^

.c.o:
	vc +aos68k -c -c99 -I${NDK_INC} $<

walk.o: walk.c map.h

map.o: map.c map.h

clean:
	rm walk
	rm *.o
