#pragma once
#include "ConstValue.h"




class Food {
public:
    Vector2 position;
    Texture2D texture;
    Texture2D textureGolden;
    bool isGolden = false;

    Food(deque <Vector2> SnakeBody); //Constructor
    ~Food();

    void Draw();

    Vector2 GetRandomCell();

    Vector2 GenerateRandomPos(deque <Vector2> SnakeBody1);

    Vector2 GenerateRandomPos2(deque <Vector2> SnakeBody1, deque <Vector2> SnakeBody2, int maze[40][40]);
};
