#ifndef MAPGEN_H
#define MAPGEN_H
#include <stdlib.h>
//Might be necessary, might put mapGen code into Map object (?)
//maybe instead of screwing with 2d char arrays, return string of ints to use as seed
    // "222000131000222000"
    //
    // becomes->
    //
    // "222000"
    // "131000"
    // "222000"

enum mapGenSymbols
{
    empty, // nothing
    vWall, // vertical wall |
    hWall, // horizontal wall -
    room, // room space .
    hall, // hallway #
    door, // door +
    mapGenMax
};

int *GenerateMap(int width, int height)
{
    int *level = new int[width*height];
    for(int i = 0; i < width*height; i++)
        level[i] = 0;
    srand (time(NULL));

    int wsize = rand()%5+5, hsize = rand()%3+5; //5+5 , 3+5
    int x = rand()%(width - wsize), y = rand()%(height - hsize);
    for(int j = 0; j < hsize; j++)
    {
        for(int i = 0; i < wsize; i++)
        {
            if(j == 0 || j == (hsize-1))
                level[(y+j)*width+(x+i)] = hWall;
            else if(i == 0 || i == (wsize-1))
                level[(y+j)*width+(x+i)] = vWall;
            else
                level[(y+j)*width+(x+i)] = room;
        }
    }
    return level;
}

#endif
