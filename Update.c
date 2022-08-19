#include "Action.h"
#include "AI.h"
#include "Movecoll.h"

/* State logic, status etc. update functions */

extern GameData_t Game;

void titleUpdate()
{
    // do nothing atm
}

void gameUpdate()
{
    AILoop();
    physics();
    checkForInteractive();
    entityLoop();
}

void pauseUpdate()
{
    // do nothing atm
}