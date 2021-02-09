walk: walk.o
	vc +aos68k -lamiga -lauto -o $@ $^

.c.o:
	vc +aos68k -c -I${NDK_INC} $<

clean:
	rm walk
	rm *.o
