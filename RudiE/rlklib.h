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
    cantMove,
    helpScreen,
    gameOver,
    slightHungry,
    reallyHungry
};

std::string CONSOLE[] =
{
    "RudiE v 0.0.4 (Press '?' for help)",
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
    "Quit\t\t:\tQ\n"
    "Help\t\t:\t?\n"
    "--------Press Space--------",
    "\n\n\n\n\t\t\t\t-------------"
    "\n\t\t\t\tYou Have Died"
    "\n\t\t\t\t-Press Space-"
    "\n\t\t\t\t-------------",
    "Your stomach rumbles",
    "Your stomach is in great pain"
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
            {'|','.','.','.','.','.','.','#','#','#'},
            {'|','.','.','.','.','.','|',' ',' ','#'},
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
        int health,bdmg = 0,tdmg;
        double hunger = 10.5;
        int stats[MAX] = {10,1,1};//default
        void setStats()
        {
            health = stats[vit];
        }
        void updateDMG()
        {
            tdmg = bdmg+str*0.5+dex*0.5;
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
            s += "\n";
            return s;
        }
};

#endif
