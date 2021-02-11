/************************************/
/*                                  */
/* Tiles                            */
/*                                  */
/************************************/

#ifndef __TILES_H__
#define __TILES_H__

#include <exec/types.h>

typedef struct {
    UWORD count;
    UWORD palette[32];
    UWORD *images;
} Tiles;

/**
 * Load a tileset from an ILBM IFF file. As many tiles will be loaded
 * as the size of the image allows. Tiles may have up to 32 colors. The
 * first tile will be read from the upper left corner of the image, and
 * additional tiles will be read left to right, then top to bottom.
 * 
 *
 * @param filename the name of the file containing the tile set.
 *
 * @return the loaded tile set.
 */
Tiles *LoadTiles(const char *filename);

/**
 * Releases the tile set.
 *
 * @param the tile set to release.
 */
void ReleaseTiles(Tiles *tiles);

/**
 * Returns a pointer to the tile with the given index.
 *
 * @param tiles the tile set.
 * @param index the tile index.
 *
 * @return a pointer to the tile image.
 */
#define GetTile(tiles,index) &(tiles->images[index*80])

#endif