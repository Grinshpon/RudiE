//Game code goes here
#include <iostream>
#include <time.h>
#include <stdlib.h>
//getch emulator
#include <unistd.h>
#include <termios.h>
//roguelike library (custom)
#include "rlklib.h"

using namespace std;

int inMessage = init;
char input = ' ';
string inputNo = "Nn";
bool clearMap = false;
int getch(void);

Map gameMap;
Map gameMapInstance; // = gameMap
Living player;

void playerInit()
{
    player.x = 1; //randomize in future
    player.y = 3; //randomize in future
}

void Clear()
{
    system("clear");
    system("clear");
}

void Start()
{//initializer
    Clear();
    playerInit();
}

void Load()
{//logic update (IS IT NECESSARY?)
    //..
}

void drawMap()
{
    //draw map, then NPCs and Player(@)
    gameMapInstance = gameMap;
    gameMapInstance.mapData[player.y][player.x] = '@';
    for(int i = 0; i < gameMapInstance.HEIGHT; i++)
    {
        for(int j = 0; j < gameMapInstance.WIDTH; j++)
        {
            cout << gameMapInstance.mapData[i][j];
        }
        cout << endl;
    }
}

void moveEntity(int modY, int modX)//add parameter for living:entity object and replace player
{
    if(gameMap.mapData[player.y-modY][player.x-modX] == '.')
    {
        player.y -= modY;
        player.x -= modX;
    }
    else
        inMessage = cantMove;
}

void Draw()
{//draw update
    if(clearMap)
    {
        Clear();
        clearMap = false;
    }
    drawMap();
    cout << CONSOLE[inMessage] << endl;
    cout << CONSOLE[enterCommand];
    input = getch();
    cout << input << endl;
    switch(input)
    {
        case 'Q':
            cout << CONSOLE[quitCheck];
            input = getch();
            if (inputNo.find(input) == string::npos)
            {
                system("clear");
                exit(0);
            }
            clearMap = true;
            break;
        case 'k': //up
            clearMap = true;
            inMessage = moveUp;
            moveEntity(1,0);
            break;
        case 'j': //down
            clearMap = true;
            inMessage = moveDown;
            moveEntity(-1,0);
            break;
        case 'h': //left
            clearMap = true;
            inMessage = moveLeft;
            moveEntity(0,1);
            break;
        case 'l': //right
            clearMap = true;
            inMessage = moveRight;
            moveEntity(0,-1);
            break;
        case 'y': //up-Left
            clearMap = true;
            inMessage = moveUpLeft;
            break;
        case 'u': //up-right
            clearMap = true;
            inMessage = moveUpRight;
            break;
        case 'b': //down-left
            clearMap = true;
            inMessage = moveDownLeft;
            break;
        case 'n': //down-right
            clearMap = true;
            inMessage = moveDownRight;
            break;
        default:
            clearMap = true;
    }
}

void Update()
{
    //...
    Load();
    Draw();
}

int getch(void)
{
    int ch;
    struct termios oldt;
    struct termios newt;
    tcgetattr(STDIN_FILENO, &oldt); /*store old settings */
    newt = oldt; /* copy old settings to new settings */
    newt.c_lflag &= ~(ICANON | ECHO); /* make one change to old settings in new settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); /*apply the new settings immediatly */
    ch = getchar(); /* standard getchar call */
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); /*reapply the old settings */
    return ch; /*return received char */
}
