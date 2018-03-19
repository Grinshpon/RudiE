#ifndef RLKLIB_H
#define RLKLIB_H

enum consoleMessage
{
    init,
    enterCommand,
    quitCheck,
    moveUp,
    moveDown,
    moveLeft,
    moveRight,
    moveUpLeft,
    moveUpRight,
    moveDownLeft,
    moveDownRight,
    cantMove
};

std::string CONSOLE[] =
{
    "RudiE v 0.0.3",
    "Enter Command: ",
    "Are you sure you want to quit? Y/n\n",
    "Move North",
    "Move South",
    "Move West",
    "Move East",
    "Move North-West",
    "Move North-East",
    "Move South-West",
    "Move South-East",
    "There's a wall in the way"
};

enum livingEntityStats
{
    vit, //max health
    str, //damage(scale)/armor
    dex, //damage(scale)/evasion
    MAX
};

class Map
{
    public:
        static const int WIDTH = 80;
        static const int HEIGHT = 20; //24 minus four lines of text for console (so that default terminal screen compatible)
        int depth;
        char mapData[HEIGHT][WIDTH] = //REPLACE WITH RANDOM ALGORITHM LATER
        {// (Y,X)
            {'-','-','-','-','-','-','-'},
            {'|','.','.','.','.','.','|'},
            {'|','.','.','.','.','.','|'},
            {'|','.','.','.','.','.','|'},
            {'|','.','.','.','.','.','|'},
            {'|','.','.','.','.','.','|'},
            {'-','-','-','-','-','-','-'},
        };
        void buildLevel(int depth);
};

class Entity
{
    public:
        int x, y;
};

class Living: public Entity
{
    public:
        int health;
        int stats[MAX];

};

#endif
