//Game code goes here
#include <iostream>
//#include <time.h>
#include <stdlib.h>
//getch emulator
#include <unistd.h>
#include <termios.h>
//roguelike library (custom)
#include "rlklib.h"

using namespace std;

int inMessage = init, dCTMessage = dDefault;
char input = ' ';
string inputYes = "Yy";
//string walkable = ".#+";
bool clearMap = false,updateWorld = false;
int getch(void);
//int consoleQueue[10];
//int queuePlace = 0;
//int queueCounter = -1;
bool hungryMessage = true, hungryMessage1 = true;

Map gameMap;
Map gameMapInstance; // = gameMap
Living player;
AIHandler Handler;

// test peon
Living peon1;

void playerInit()
{
    player.randStats();
    player.setStats();
    player.updateDMG();
    player.id = '@';
    player.x = -1;
    player.y = -1;

    peon1.randStats();
    peon1.setStats();
    peon1.updateDMG();
    peon1.id = 'p';
}

void Clear()
{
    system("clear");
    system("clear");
}

void LivingStartLocation(Entity& ent, int invalX, int invalY)
{
    srand (time(NULL));
    int counti = 0, countj = 0, tempx = 0, tempy = 0;
    bool playerStartLocation = true;
    for(auto &r : gameMap.mapData)
    {
        if(playerStartLocation)
        {
            for(auto &s : r)
            {
                if(s == '.' && (countj != invalX && counti != invalY))
                {
                    tempx = countj;
                    tempy = counti;
                    if(rand()%5 == 0)
                    {
                        ent.x = countj;
                        ent.y = counti;
                        playerStartLocation = false;
                    }
                }
                else if(counti == (gameMap.HEIGHT-1) && countj == (gameMap.WIDTH-1))
                {
                    ent.x = tempx;
                    ent.y = tempy;
                    playerStartLocation = false;
                }
                ++countj;
            }
            countj = 0;
            ++counti;
        }
    }
}

void Start()
{//initializer
    Clear();
    playerInit();
    gameMap.genSeed(0);
    gameMap.buildLevel();

    LivingStartLocation(player,-1,-1);
    LivingStartLocation(peon1,player.x,player.y);//TEMPORARY
}

void drawMap()
{
    //draw map, then NPCs and Player(@)
    gameMapInstance.seed = gameMap.seed;
    gameMapInstance.buildLevel();
    /*
    for(int i = 0; i < gameMap.HEIGHT; i++)
    {
        for(int j = 0; j < gameMap.WIDTH; j++)
        {
            gameMapInstance.mapData[i][j] = gameMap.mapData[i][j];
        }
    }
    */
    gameMapInstance.mapData[player.y][player.x] = player.id;
    gameMapInstance.mapData[peon1.y][peon1.x] = peon1.id;
    for(int i = 0; i < gameMapInstance.HEIGHT; i++)
    {
        for(int j = 0; j < gameMapInstance.WIDTH; j++)
        {
            cout << gameMapInstance.mapData[i][j];
        }
        cout << endl;
    }
}

void movePlayer(int modY, int modX)//add parameter for living:entity object and replace player
{
    if(walkable.find(gameMap.mapData[player.y-modY][player.x-modX]) != string::npos)
    {
        player.y -= modY;
        player.x -= modX;
    }
    else
    {
        consoleQueue[queueCounter] = cantMove;
        updateWorld = false;
    }
}

void HandlerUpdate()//add parameter for living:entity object and replace player
{
    Handler.updatePeon(peon1,player,gameMap);
}

void GameOver()
{
    Clear();
    cout << CONSOLE[gameOver] << endl << CONSOLE[deathCause] << deathCauseText[dCTMessage];
    while(input != ' ')
    input = getch();
    Clear();
    exit(0);
}

void Load()
{//logic update (IS IT NECESSARY?)
    if(player.stats[xp] >= player.reqXP)
        {
            queueCounter += 1;
            consoleQueue[queueCounter] = levelUp;
        }
    if(updateWorld)
    {
        HandlerUpdate();
        player.logicUpdate();
        if(player.hunger <= 0)
        {
            player.health -= 1;
            dCTMessage = dHunger;
        }
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
    if(player.hunger < 1 && hungryMessage1)
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
            if (inputYes.find(input) != string::npos)
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
            movePlayer(1,0);
            break;
        case 'j': //down
            clearMap = true;
            queueCounter += 1;
            consoleQueue[queueCounter] = moveDown;
            movePlayer(-1,0);
            break;
        case 'h': //left
            clearMap = true;
            queueCounter += 1;
            consoleQueue[queueCounter] = moveLeft;
            movePlayer(0,1);
            break;
        case 'l': //right
            clearMap = true;
            queueCounter += 1;
            consoleQueue[queueCounter] = moveRight;
            movePlayer(0,-1);
            break;
        case 'y': //up-Left
            clearMap = true;
            queueCounter += 1;
            consoleQueue[queueCounter] = moveUpLeft;
            movePlayer(1,1);
            break;
        case 'u': //up-right
            clearMap = true;
            queueCounter += 1;
            consoleQueue[queueCounter] = moveRight;
            movePlayer(1,-1);
            break;
        case 'b': //down-left
            clearMap = true;
            queueCounter += 1;
            consoleQueue[queueCounter] = moveDownLeft;
            movePlayer(-1,1);
            break;
        case 'n': //down-right
            clearMap = true;
            queueCounter += 1;
            consoleQueue[queueCounter] = moveDownRight;
            movePlayer(-1,-1);
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
            player.stats[xp] = player.reqXP;
            break;
        case 'C':
            Clear();
            cout << player.printCharSheet();
            while(input != ' ')
            input = getch();
            clearMap = true;
            updateWorld = false;
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
