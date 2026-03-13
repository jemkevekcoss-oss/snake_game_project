#pragma once
#include "ConstValue.h"
#include <fstream>



class Map {
public:
    // 0: vat can, than ran | 1: hop le 
    //CellCount == 40
    
    int data[40][40]; //For Mode vs AI snake
    bool vis[40][40];

    int maze[40][40]; //For Mode 2 player, draw only 1 time
    
    Texture textureMap;
    Texture textureObstacle;
    Texture textureFixedObstacle;

    Map();
    ~Map();

    void Clear();
    void Update(deque<Vector2> snakeBody);
    void setMap(const char* imagePath);
    void Draw();


    void loadImageObstacle(const char* imagePath);
    void loadImageFixedObstacle(const char* imagePath);
    void setFixedObstacles();////////////////////////////
    void setObstaclesWithDFS( snakeFirstPos , snakeFirstPos , const int& );

    void DrawFixedObstacle();
    void DrawObstacleWithDFS();
};
