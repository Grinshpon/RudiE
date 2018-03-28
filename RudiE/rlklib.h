#ifndef RLKLIB_H
#define RLKLIB_H

#include <stdlib.h>
#include "mapgen.h"

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
    cantMove,
    helpScreen,
    gameOver,
    slightHungry,
    reallyHungry,
    deathCause,
    consoleMessageMax
};

std::string CONSOLE[consoleMessageMax] =
{
    "RudiE v 0.1.0 (Press '?' for help)",
    "Enter Command: ",
    "Are you sure you want to quit? y/N\n",
    "Move North",
    "Move South",
    "Move West",
    "Move East",
    "Move North-West",
    "Move North-East",
    "Move South-West",
    "Move South-East",
    "There's a wall in the way",
    "-----------Help-----------\n"
    "Move North\t:\tk\n"
    "Move South\t:\tj\n"
    "Move West\t:\th\n"
    "Move East\t:\tl\n"
    "Move North-West\t:\ty\n"
    "Move North-East\t:\tu\n"
    "Move South-West\t:\tb\n"
    "Move South-East\t:\tn\n"
    "Character Sheet\t:\tC\n"
    "Quit\t\t:\tQ\n"
    "Help\t\t:\t?\n"
    "--------Press Space-------",
    "\n\n\n\n\t\t\t\t-------------"
    "\n\t\t\t\tYou Have Died"
    "\n\t\t\t\t-Press Space-"
    "\n\t\t\t\t-------------",
    "Your stomach rumbles",
    "Your stomach is in great pain",
    "\n\t\t\t\tCause of death:\n\n\t\t\t\t   "
};

enum CauseOfDeath
{
    dHunger,
    dDefault,
    CauseOfDeathMax
};

std::string deathCauseText[CauseOfDeathMax] =
{
    "Hunger",
    "dDefault"
};

enum livingEntityStats
{
    vit, //max health
    str, //damage(scale)/armor
    dex, //damage(scale)/evasion
    xp,
    lvl,
    MAX
};

class Map
{
    public:
        static const int WIDTH = 80; //80: default terminal screen width
        static const int HEIGHT = 20; //20: 24 minus four lines of text for console (so that default terminal screen compatible)
        int depth;
        char mapData[HEIGHT][WIDTH] = //REPLACE WITH RANDOM ALGORITHM LATER
        {// (Y,X)
            {'-','-','-','-','-','-','-'},
            {'|','.','.','.','.','.','|'},
            {'|','.','.','.','.','.','.','#','#','#'},
            {'|','.','.','.','.','.','|',' ',' ','#'},
            {'|','.','.','.','.','.','|'},
            {'|','.','.','.','.','.','|'},
            {'-','-','-','-','-','-','-'},
        };
        int *seed;
        void genSeed(int depth)
        {
            seed = GenerateMap(WIDTH,HEIGHT);
        }
        void buildLevel()
        {
            for(int i = 0; i < HEIGHT; i++)
            {
                for(int j = 0; j < WIDTH; j++)
                {
                    switch (seed[WIDTH*i+j])
                    {
                        case empty:
                            mapData[i][j] = ' ';
                            break;
                        case vWall:
                            mapData[i][j] = '|';
                            break;
                        case hWall:
                            mapData[i][j] = '-';
                            break;
                        case room:
                            mapData[i][j] = '.';
                            break;
                        case hall:
                            mapData[i][j] = '#';
                            break;
                        case door:
                            mapData[i][j] = '+';
                            break;
                        default:
                            break;
                    }
                }
            }
        }
};

class Entity
{
    public:
        int x, y;
        std::string id;
};

class Living: public Entity
{
    public:
        int health,bdmg = 0,tdmg;
        double hunger = 10.5;
        int stats[MAX] = {10,1,1,0,1};//default
        void setStats()
        {
            health = stats[vit];
        }
        void updateDMG()
        {
            tdmg = bdmg+stats[str]*0.5+stats[dex]*0.5;
        }
        void randStats()
        {
            srand (time(NULL));
            stats[vit] = rand()%5+10;
            stats[str] = rand()%4+1;
            stats[dex] = rand()%4+1;
        }
        void logicUpdate()
        {
            if(hunger > 0)
                hunger -= 0.025;
        }
        std::string printStats()
        {
            //"hp: " + health + " (" + stats[vit] + ")\tdmg: " + (dmg+str*0.5+dex*0.5);
            updateDMG();
            std::string s = "Hp: ";
            s += std::to_string(health);
            s += "(";
            s += std::to_string(stats[vit]);
            s += ")\tDmg: ";
            s += std::to_string(tdmg);
            s += "\tHunger: ";
            s += std::to_string((int)hunger);
            s += "\t(";
            s += std::to_string(x);
            s += ",";
            s += std::to_string(y);
            s += ")\n";
            return s;
        }
        std::string printCharSheet()
        {
            std::string s = "Character Sheet\n\nHealth:\t\t";
            s += std::to_string(health);
            s += "/";
            s += std::to_string(stats[vit]);
            s += "\nDamage:\t\t";
            s += std::to_string(tdmg);
            s += "\nHunger:\t\t";
            s += std::to_string((int)hunger);
            s += "\n\nVitality:\t";
            s += std::to_string(stats[vit]);
            s += "\nStrength:\t";
            s += std::to_string(stats[str]);
            s += "\nDexterity:\t";
            s += std::to_string(stats[dex]);
            s += "\n\nLevel:\t\t";
            s += std::to_string(stats[lvl]);
            s += "\nExperience:\t";
            s += std::to_string(stats[xp]);
            s += "\n\n-Press Space-";
            return s;
        }
};

#endif
