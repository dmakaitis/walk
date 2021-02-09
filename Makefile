walk: walk.c
	vc +aos68k -c99 -I${NDK_INC} walk.c -lamiga -lauto -o walk
