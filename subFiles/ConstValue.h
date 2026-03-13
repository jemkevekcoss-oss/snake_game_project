#pragma once
#include <iostream>
#include "raylib.h"
#include <deque>
#include <raymath.h>
#include <string>
#include <vector>
#include "AdditionalFunc.h"
#include <queue>
#include <stack>
#include <algorithm>
#include <random>
#include <ctime>

#ifndef CONST_VALUE_H
#define CONST_VALUE_H

#endif
/*
Valid Colors : LIGHTGRAY, GRAY, DARKGRAY, YELLOW, GOLD, ORANGE, PINK, RED, MAROON, GREEN, LIME, DARKGREEN,WHITE, BLACK, BLANK
*/

struct snakeFirstPos {
    Vector2 Poshead;
    Vector2 Posbody;
    Vector2 Postail;

};

const int obstacle = -1;
const int fixObstacle = 2;
const int validBlock = 1;
const int bodyBlock = 0;

enum GameMode {PLAYING1P, PLAYING2P, AIBATTLE};

enum GameState {
    MENU,
    MODE_SELECT,      // Màn hình chọn 1 trong 3 chế độ
    PLAYING,
    PAUSE,
    GAMEOVER
};

inline int TARGETSCORE = 100;
inline Color green = { 173,204,96,100 };
inline Color darkgreen = { 43,51,24,255 };

inline int CellSize = 25;
inline int CellCount = 40;
inline float offset = (float)CellSize*2;


inline float heightWin = ( 2*offset + CellSize * CellCount ) ;
inline float widthWin = ( 2*offset + CellSize * CellCount ) ;
//Link2

