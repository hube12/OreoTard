#include <iostream>
#include <vector>
#include <limits.h>
#include "generationByCubitect/generator.hpp"
#include "generationByCubitect/layers.hpp"
#define ABS(N) ((N<0)?(-N):(N))
#define DESERT 2
#define DESERT_HILLS 17
#define DESERT_LAKES 130
#define STRIDE 240
#define SIZE  464


bool isDesert(unsigned long biomeID) {
    return biomeID == DESERT || biomeID == DESERT_HILLS || biomeID == DESERT_LAKES;
}




bool checkArea(long x, long z, LayerStack g) {
    std::cout << "Searching for " << x << " and " << z << std::endl;

    int *map = allocCache(&g.layers[g.layerNum - 1], 1, 1);
    genArea(&g.layers[g.layerNum - 1], map, x, z, 1, 1);

    if (!isDesert(map[0])) {
        free(map);
        return false;
    }
    free(map);

    int *biomess = allocCache(&g.layers[g.layerNum - 1], SIZE, SIZE);
    genArea(&g.layers[g.layerNum - 1], biomess, x - 14 * 16, z - 14 * 16, SIZE, SIZE);
    bool desertChunks[29][29];
    for (unsigned long chunkX = 0; chunkX < 29; chunkX++) {

        for (unsigned long chunkZ = 0; chunkZ < 29; chunkZ++) {
            for (unsigned long dx = 0; dx < 16; dx++) {
                for (unsigned long dz = 0; dz < 16; dz++) {
                    if (!isDesert(biomess[chunkX * 16 + dx + SIZE * (chunkZ * 16 + dz)])) {
                        goto innerChunkLoop;
                    }
                }
            }
            desertChunks[chunkX][chunkZ] = true;
        }
        innerChunkLoop:;
    }
    for (unsigned long chunkX = 0; chunkX < 15; chunkX++) {

        for (unsigned long chunkZ = 0; chunkZ < 15; chunkZ++) {
            for (unsigned long dx = 0; dx < 15; dx++) {
                for (unsigned long dz = 0; dz < 15; dz++) {
                    if (!desertChunks[chunkX + dx][chunkZ + dz])
                        goto innerOuter;
                }
            }
            return true;
        }
        innerOuter:;
    }
    free(biomess);
    return false;

}


int main() {
    initBiomes();
    LayerStack g = setupGenerator(MC_1_12);
    applySeed(&g, (int64_t) 2791111690993685248);
    unsigned long minDistance = ULONG_MAX;
    unsigned long x = 0, z = 0;
    if (!checkArea(0, 0,g)) {
        for (unsigned long radius = STRIDE; radius * radius <= minDistance; radius += STRIDE) {
            for (unsigned long dx = radius; dx > -radius; dx -= STRIDE) {
                unsigned long dz = ABS(dx) - STRIDE;
                if (checkArea(dx, dz,g)) {
                    unsigned long distance = dx * dx + dz * dz;
                    if (distance < minDistance) {
                        minDistance = distance;
                        x = dx;
                        z = dz;
                    }
                }
            }
            for (unsigned long dx = -radius; dx < radius; dx += STRIDE) {
                unsigned long dz = STRIDE - ABS(dx);
                if (checkArea(dx, dz,g)) {
                    unsigned long distance = dx * dx + dz * dz;
                    if (distance < minDistance) {
                        minDistance = distance;
                        x = dx;
                        z = dz;
                    }
                }
            }
        }
    }

    std::cout<<x << ", " << z<<std::endl;
    freeGenerator(g);
}

