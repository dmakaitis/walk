/************************************/
/*                                  */
/* Map                              */
/*                                  */
/************************************/

#include "map.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

GameMap *LoadGameMap(const char *filename) {
    int width, height;

    FILE *file = fopen(filename, "r");
    fscanf(file, "%i %i", &width, &height);

    if(width < 0 || width > 32) {
        printf("Error loading map '%s' - width out of range: %i\n", filename, width);
        return NULL;
    }
    if(height < 0 || height > 32) {
        printf("Error loading map '%s' - height out of range: %i\n", filename, height);
        return NULL;
    }

    GameMap *map = malloc(sizeof(GameMap));
    map->width = width;
    map->height = height;
    map->map = malloc(width * height);

    char *mapPtr = map->map;
    int counter = width * height;

    while(counter) {
        int value = fgetc(file);
        if(value == EOF) {
            counter = 0;
        } else if(!isspace(value)) {
            *mapPtr = (char) value;
            mapPtr++;
            counter--; 
        }
    }

    fclose(file);

    return map;
}

void ReleaseGameMap(GameMap *map) {
    if(map != NULL) {
        free(map->map);
        free(map);
    }
}

char ReadMap(GameMap *map, int x, int y) {
    while(x < 0) {
        x += map->width;
    }
    while(x >= map->width) {
        x -= map->width;
    }
    
    while(y < 0) {
        y += map->height;
    }
    while(y >= map->height) {
        y -= map->height;
    }
    
    return map->map[y * map->width + x];
}