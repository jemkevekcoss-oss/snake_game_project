//#pragma once
//#include <iostream>
//#include "raylib.h"
//#include <deque>
//#include <raymath.h>
//#include <string>
//#include "AdditionalFunc.h"
//#include "ConstValue.h"
//#include <queue>
//
//using namespace std;
//
//class Map {
//public:
//    // 0: vat can, than ran | 1: hop le 
//    //CellCount == 40
//    int data[40][40];
//    bool vis[40][40];
//
//    Map();
//    
//    void Clear();
//    void Update(deque<Vector2> snakeBody);
//
//};
//
//class Snake {
//public:
//    deque <Vector2> body;
//    deque <Vector2> PathAI;
//    Vector2 direction;
//    bool addSegment;
//    bool isAI;
//    void FindPathBFS(Vector2 startPos, Vector2 endPos, Map& map, deque<Vector2>& Path);
//    int score;
//
//    bool canChangeDirection = true;
//
//    Texture2D headTexture;
//    Texture2D bodyTexture;
//    Texture2D tailTexture;
//
//    Snake(Vector2,Vector2,Vector2,bool);
//    ~Snake();
//
//    void Draw();
//    void Update();
//    void Reset();
//
//};
//
//class Food {
//public:
//    Vector2 position;
//    Texture2D texture;
//
//    Food(deque <Vector2> SnakeBody); //Constructor
//    ~Food();
//
//    void Draw();
//
//    Vector2 GetRandomCell();
//
//    Vector2 GenerateRandomPos(deque <Vector2> SnakeBody);
//
//};
//
//class Game {
//public:
//    //Menu menu = Menu();
//
//    Snake snake = Snake(Vector2{6,1}, Vector2{ 5,1 }, Vector2{ 4,1 },false);
//    Snake snakeAI = Snake(Vector2{ 6,5 }, Vector2{ 5,5 }, Vector2{ 4,5 },true);
//    Map map = Map();
//
//    Food food = Food(snake.body);
//    bool running = true;
//    //int score = 0;
//    //int scoreAI = 0; ///////////
//    bool snakePCrashed = false;
//    bool snakeAICrashed = false;
//
//    GameState gamestate = MENU;
//
//    Sound eatSound;
//    Sound wallColSound;
//     
//    Game();
//    ~Game();
//
//    void Draw();
//    void Update();
//
//    void CheckCollisionwithFood();
//
//    void GameOver();
//    void checkCollisionWithWalls();
//    void checkCollisionWithBody();
//};
//
//class Button {
//public:
//    Button(const char* imagePath, Vector2 ImagePosition, float scale);
//    ~Button();
//    void Draw();
//
//    bool isPressed(Vector2 mousePos, bool mousePressed);
//
//private:
//    Texture2D texture;
//    Vector2 position;
//};
//
//
