#pragma once
#include "ConstValue.h"
#include "Map.h"

class Snake {
public:
    

    deque <Vector2> body;
    deque <Vector2> PathAI;
    Vector2 direction;

    bool running; /////////////
    bool addSegment;
    int score;
    bool canChangeDirection = true;

    Texture2D headTexture;
    Texture2D bodyTexture;
    Texture2D tailTexture;

    bool isAI;
    bool isplayer1 = true;
    Snake(const snakeFirstPos&, bool,bool);
    ~Snake();

    void FindPathBFS(Vector2 startPos, Vector2 endPos, Map& map, deque<Vector2>& Path);

    void HandleInput1(bool&,bool&);
    void HandleInput2(bool&);
    void Draw();
    void Update();
    void Reset();

};

