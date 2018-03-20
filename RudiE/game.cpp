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
string walkable = ".#";
bool clearMap = false,updateWorld = false;
int getch(void);
int consoleQueue[10];
int queuePlace = 0;
int queueCounter = -1;
bool hungryMessage = true, hungryMessage1 = true;

Map gameMap;
Map gameMapInstance; // = gameMap
Living player;

void playerInit()
{
    player.setStats();
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
    if(walkable.find(gameMap.mapData[player.y-modY][player.x-modX]) != string::npos)
    {
        player.y -= modY;
        player.x -= modX;
    }
    else
        inMessage = cantMove;
}

void GameOver()
{
    Clear();
    cout << CONSOLE[gameOver];
    while(input != ' ')
    input = getch();
    Clear();
    exit(0);
}

void Load()
{//logic update (IS IT NECESSARY?)
    if(updateWorld)
    {
        player.logicUpdate();
        if(player.hunger <= 0)
            player.health -= 1;
    }
    else
        updateWorld = !updateWorld;
    if(player.hunger <= 3 && hungryMessage)//comparing double to int
    {
        queueCounter += 1;
        consoleQueue[queueCounter] = slightHungry;
        hungryMessage = false;
    }
    else if(player.hunger > 3)
        hungryMessage = true;
    if(player.hunger <= 1 && hungryMessage1)
    {
        queueCounter += 1;
        consoleQueue[queueCounter] = reallyHungry;
        hungryMessage1 = false;
    }
    else if(player.hunger > 1)
        hungryMessage1 = true;
    if(player.health == 0)
        GameOver();
}

void Draw()
{//draw update
    if(clearMap)
    {
        Clear();
        clearMap = false;
    }
    cout << player.printStats();
    drawMap();
    if(queueCounter == -1)
        cout << CONSOLE[inMessage] << endl;
    if(queueCounter == 0)
    {
        inMessage = consoleQueue[queueCounter];
        cout << CONSOLE[inMessage] << endl;
        consoleQueue[queueCounter] = -1;
        queueCounter = -1;
    }
    if(queueCounter > 0)
    {
        while(queueCounter > -1)
        {
            input = '/';
            inMessage = consoleQueue[queuePlace];
            consoleQueue[queuePlace] = -1;
            Clear();
            cout << player.printStats();
            drawMap();
            if(queueCounter > 0)
            {
                cout << CONSOLE[inMessage] << " -more-" << endl;
                while(input == '/')
                input = getch();
            }
            else
                cout << CONSOLE[inMessage] << endl;
            queuePlace += 1;
            queueCounter -= 1;
        }
        queuePlace = 0;
    }
    //cout << CONSOLE[enterCommand];
    input = getch();
    //cout << input << endl;
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
            updateWorld = false;
            break;
        case 'k': //up
            clearMap = true;
            //inMessage = moveUp;
            queueCounter += 1;
            consoleQueue[queueCounter] = moveUp;
            moveEntity(1,0);
            break;
        case 'j': //down
            clearMap = true;
            queueCounter += 1;
            consoleQueue[queueCounter] = moveDown;
            moveEntity(-1,0);
            break;
        case 'h': //left
            clearMap = true;
            queueCounter += 1;
            consoleQueue[queueCounter] = moveLeft;
            moveEntity(0,1);
            break;
        case 'l': //right
            clearMap = true;
            queueCounter += 1;
            consoleQueue[queueCounter] = moveRight;
            moveEntity(0,-1);
            break;
        case 'y': //up-Left
            clearMap = true;
            queueCounter += 1;
            consoleQueue[queueCounter] = moveUpLeft;
            moveEntity(1,1);
            break;
        case 'u': //up-right
            clearMap = true;
            queueCounter += 1;
            consoleQueue[queueCounter] = moveRight;
            moveEntity(1,-1);
            break;
        case 'b': //down-left
            clearMap = true;
            queueCounter += 1;
            consoleQueue[queueCounter] = moveDownLeft;
            moveEntity(-1,1);
            break;
        case 'n': //down-right
            clearMap = true;
            queueCounter += 1;
            consoleQueue[queueCounter] = moveDownRight;
            moveEntity(-1,-1);
            break;
        case '?':
            Clear();
            cout << CONSOLE[helpScreen];
            while(input != ' ')
            input = getch();
            clearMap = true;
            updateWorld = false;
            break;
        case 'T': //TEST REMOVE LATER
            clearMap = true;
            queueCounter += 1;
            consoleQueue[queueCounter] = moveUp;
            queueCounter += 1;
            consoleQueue[queueCounter] = moveDown;
            queueCounter += 1;
            consoleQueue[queueCounter] = moveLeft;
            queueCounter += 1;
            consoleQueue[queueCounter] = moveRight;
            break;
        default:
            clearMap = true;
            updateWorld = false;
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
