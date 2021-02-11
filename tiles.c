/************************************/
/*                                  */
/* Tiles                            */
/*                                  */
/************************************/

#include "tiles.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <clib/dos_protos.h>
#include <clib/iffparse_protos.h>

#define ID_ILBM MAKE_ID('I', 'L', 'B', 'M')
#define ID_BMHD MAKE_ID('B', 'M', 'H', 'D')
#define ID_CMAP MAKE_ID('C', 'M', 'A', 'P')
#define ID_BODY MAKE_ID('B', 'O', 'D', 'Y')

typedef UBYTE Masking;
typedef UBYTE Compression;

typedef struct {
    UWORD width, height;
    UWORD positionX, positionY;
    UBYTE numPlanes;
    Masking masking;
    Compression compression;
    UBYTE pad1;
    UWORD transparentColor;
    UBYTE aspectX, aspectY;
    WORD pageWidth, pageHeight;
} BitMapHeader;

void ReadColorMap(Tiles *tiles, struct StoredProperty *property) {
    UWORD color;
    UWORD *palettePtr = tiles->palette;
    UBYTE *propPtr = property->sp_Data;

    for(int i = 0; i < property->sp_Size - 2; i += 3, palettePtr++, propPtr += 3) {
        *palettePtr = ((propPtr[0] & 0xf0) << 4) | (propPtr[1] & 0xf0) | ((propPtr[2] & 0xf0) >> 4);
    }   
}

void ReadTileImages(Tiles *tiles, BitMapHeader *bitMapHeader, struct IFFHandle *iff) {
    if(bitMapHeader) {
        unsigned int width = bitMapHeader->width;
        unsigned int height = bitMapHeader->height;

        printf("Width: %u, Height: %u, Bitplanes: %u\n", width, height, bitMapHeader->numPlanes);

        unsigned int widthTiles = width / 16;
        unsigned int heightTiles = height / 16;
        unsigned int planeOffset = (width + 15) / 16;
        unsigned int rowOffset = bitMapHeader->numPlanes * planeOffset;

        printf("Width in Tiles: %u, Height in Tiles: %u\n", widthTiles, heightTiles);
        printf("Plane offset (in words): %u\n", planeOffset);
        printf("Row offset (in words): %u\n", rowOffset);

        printf("Image %s compressed.\n", bitMapHeader->compression ? "is" : "is not");

        struct ContextNode *node = CurrentChunk(iff);
        printf("Image chunk size: %i\n", (int) node->cn_Size);

        UBYTE *chunk = malloc(node->cn_Size);
        ReadChunkBytes(iff, chunk, node->cn_Size);

        printf("Expected uncompressed size: %u\n", ((width + 7) / 8) * height * bitMapHeader->numPlanes);

        UBYTE *bitmapBytes = malloc(((width + 7) / 8) * height * bitMapHeader->numPlanes);

        int chunkPtr = 0;
        int bitmapPtr = 0;
        while(chunkPtr < node->cn_Size) {
            UBYTE value = chunk[chunkPtr++];
            if(value > 128) {
                UBYTE value2 = chunk[chunkPtr++];
                for(int i = 0; i < (257 - value); i++) {
                    bitmapBytes[bitmapPtr++] = value2;
                }
            } else if(value < 128) {
                for(int i = 0; i < value + 1; i++) {
                    bitmapBytes[bitmapPtr++] = chunk[chunkPtr++];
                }
            }
        }

        printf("Uncompressed %i bytes.\n", bitmapPtr);

        unsigned int tileCount = widthTiles * heightTiles;

        tiles->count = tileCount;
        tiles->images = malloc(tileCount * 16 * 5 * sizeof(UWORD));

        printf("Tile count: %u\n", tileCount);
        printf("Tile image size: %i\n", (int)(tileCount * 16 * 5 * sizeof(UWORD)));

        UWORD *bitmap = (UWORD *)bitmapBytes;
        UWORD *src = bitmap;
        UWORD *dest = tiles->images;
        int destPtr = 0;

        for(int tileY = 0; tileY < heightTiles; tileY++) {
            src = bitmap + tileY * 16 * rowOffset;
            for(int tileX = 0; tileX < widthTiles; tileX++) {
                for(int plane = 0; plane < bitMapHeader->numPlanes; plane++) {
                    for(int row = 0; row < 16; row++) {
                        dest[destPtr++] = src[row * rowOffset + plane * planeOffset];
                    }
                }
                src++;
            }
        }

        printf("Stored %i tile image bytes.\n", destPtr * 2);

        free(bitmapBytes);
        free(chunk);
    }  
}

Tiles *LoadTiles(const char *filename) {
    Tiles *tiles = NULL;

    struct StoredProperty *prop = NULL;
    BitMapHeader *header = NULL;

    struct IFFHandle *iff = AllocIFF();

    iff->iff_Stream = Open(filename, MODE_OLDFILE);
    if(iff->iff_Stream) {
        InitIFFasDOS(iff);
        if(!OpenIFF(iff, IFFF_READ)) {
            tiles = malloc(sizeof(Tiles));
            memset(tiles, 0, sizeof(Tiles));

            PropChunk(iff, ID_ILBM, ID_BMHD);
            PropChunk(iff, ID_ILBM, ID_CMAP);
            StopChunk(iff, ID_ILBM, ID_BODY);

            ParseIFF(iff, IFFPARSE_SCAN);

            if(prop = FindProp(iff, ID_ILBM, ID_BMHD)) {
                header = (BitMapHeader *) prop->sp_Data;
            }

            if(prop = FindProp(iff, ID_ILBM, ID_CMAP)) {
                ReadColorMap(tiles, prop);
            }

            ReadTileImages(tiles, header, iff);

            CloseIFF(iff);
        } else {
           printf("Failed to start parsing IFF file: %s\n", filename);
        }
        Close(iff->iff_Stream);
    } else {
        printf("Failed to open tile image file: %s\n", filename);
    }

    FreeIFF(iff);

    return tiles;
}

void ReleaseTiles(Tiles *tiles) {
    if(tiles->images) {
        free(tiles->images);
    }
    free(tiles);
}
