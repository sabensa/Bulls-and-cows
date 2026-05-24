#pragma once
#include "Player2.h"
#include "Player1.h"

class GameRun 
{
    Player2 p2;
    Player1 p1;
    int rounds;
    int n;

public:
    GameRun(int n, int k, int strategyNum, int seed);
    void play();
};

