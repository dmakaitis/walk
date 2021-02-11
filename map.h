/************************************/
/*                                  */
/* Map                              */
/*                                  */
/************************************/

#ifndef __MAP_H__
#define __MAP_H__

/*
 * Map structure.
 */

typedef struct {
    int width, height;
    char *map;
} GameMap;


/**
 * Loads a game map from disk. The map must later be released by calling
 * ReleaseGameMap.
 *
 * @param filename the name of the file from which to load the map.
 * @returns a newly allocated map populated with the contents of the map file.
 */
GameMap *LoadGameMap(const char *filename);

/**
 * Releases the game map from memory.
 *
 * @param map the map to release from memory.
 */
void ReleaseGameMap(GameMap *map);

/**
 * Returns the value at the given location on the map. Coordinates with automatically
 * be wrapped to within the confines of the map.
 *
 * @param map the game map.
 * @param x the x coordinate.
 * @param y the y coordinate.
 * @return the value of the game map at the specified location.
 */
char ReadMap(GameMap *map, int x, int y);

#endif
