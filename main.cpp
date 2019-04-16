#include <iostream>
#include <vector>
#include "generationByCubitect/generator.hpp"
#include "generationByCubitect/layers.hpp"

#define DESERT 2
#define DESERT_HILLS 17
#define DESERT_LAKES 130
#define SIZE 29
#define STEP 10000
enum versions {
    MC_1_7, MC_1_8, MC_1_9, MC_1_10, MC_1_11, MC_1_12, MC_1_13, MC_1_13_2, MC_1_14, MC_LEG
};

typedef struct Pos {
    long long x, z;
} Pos;
int main() {
    initBiomes();
    LayerStack g = setupGenerator(MC_1_12);
    applySeed(&g, (int64_t) 2791111690993685248);

    unsigned long maxMegaChunks=20000;
    int threadID;
    Pos pos;
    pos.x =0;
    pos.z =0;
    int iter=0;
    while(true){
        int direction=threadID;
        int turn=8/threadID;
        switch (direction){
            case 0:
                pos.x++;
                int *map = allocCache(&g.layers[g.layerNum - 1], 1, STEP);
                genArea(&g.layers[g.layerNum - 1], map, pos.x, pos.z, 1, STEP);
                free(map);
                break;
            case 1:
                pos.z++;
                int *map = allocCache(&g.layers[g.layerNum - 1], STEP, 1);
                genArea(&g.layers[g.layerNum - 1], map, pos.x, pos.z, 1, STEP);
                free(map);
                break;
            case 2:
                pos.x--;
                break;
            case 3:
                pos.x--;
                break;
            default:
                exit(-3);
        }
        if(iter>100){
            break;
        }
    }



    freeGenerator(g);
}

std::vector<bool> getPos(int *map){
    std::vector<bool> Pos;
    for (long i=0;i<STEP;i++){
        int biomeID=map[i];
        if (biomeID==DESERT_HILLS || biomeID==DESERT|| biomeID==DESERT_LAKES){
            Pos.push_back(i);
        }
    }
    return Pos
}

