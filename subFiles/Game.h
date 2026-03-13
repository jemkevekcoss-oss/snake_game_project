#pragma once
#include "ConstValue.h"
#include "Food.h"
#include "Snake.h"
#include "Map.h"
#include "Button.h"
#include <fstream>

class Game {
public:
    
    Map map = Map();

    snakeFirstPos FP_snake   = snakeFirstPos{ Vector2{2,0},Vector2{1,0},Vector2{0,0} };
    snakeFirstPos FP_snakep2 = snakeFirstPos{ Vector2{2,39},Vector2{1,39},Vector2{0,39} };
    snakeFirstPos FP_snakeAI = snakeFirstPos{ Vector2{2,15},Vector2{1,15},Vector2{0,15} };

    Snake snake = Snake(FP_snake , false, true);
    Snake snakep2 = Snake(FP_snakep2, false, false);
    Snake snakeAI = Snake(FP_snakeAI, true, false);
    
    Food food = Food(snake.body);
    

    int highScore = 0;

    void loadHighScore();  //void saveHighScore(); 
    void checkNewHighScore();

    bool running = true;
    //bool isPause = false;

    bool snakePCrashed = false;
    bool snakeP2Crashed = false;
    bool snakeAICrashed = false;


    GameMode gamemode;
    GameState gamestate = MENU;

    Music musicGame;
    bool isMuted;

    Sound eatSound;
    Sound wallColSound;
    Sound clickButtonSound; 
    
    //Mode VS AI snake
    int timer;
    const int timeLimited = 60; // Secconds

    // Cho chế độ 2P
    int TARGET_SCORE = TARGETSCORE; 

    Game();
    ~Game();

    Texture youWin;
    Texture player1Win;
    Texture player2Win;
    Texture aisnakeWin;
    Texture drawResult;

    Image imagey ;
    Image imagep1 ;
    Image imagep2 ;
    Image imageai ;
    Image imageDraw;

    //Buttons
    Button startButton = Button("Images/startGame.png", Vector2{ (float)(heightWin / 2 - 1.25 * offset) , heightWin / 2 + 40}, 0.7);
    Button exitButton = Button("Images/exitGame.png", Vector2{ (float)(heightWin / 2 - 1.25 * offset), heightWin / 2 + 110}, 0.7);
    Button classicButton = Button("Images/PLAYING1P.png", Vector2{ heightWin / 2 - 2*offset +10 , heightWin / 2 - 30 }, 0.7);
    Button duoButton = Button("Images/PLAYING2P.png", Vector2{ heightWin / 2 - 2 * offset + 10, heightWin / 2 + 40 }, 0.7);
    Button vsAIModeButton = Button("Images/PLAYINGAI.png", Vector2{ heightWin / 2 - 2 * offset + 2  , heightWin / 2 + 110 }, 0.7);
    Button unmutedButton = Button("Images/unmutedButton.png", Vector2{0,heightWin - offset},0.2);
    Button mutedButton = Button("Images/mutedButton.png",Vector2{0,heightWin - offset },0.2);
    Button musicButton = unmutedButton;

    Button playAgainButton = Button("Images/playagainButton.png", Vector2{ (float)(heightWin / 2 - 2.5 * offset)  , (float)(heightWin / 2 + 100)}, 0.3);
    void UpdateMusic();



    void Draw();
    void dealingAIPath();
    void Update();
    
    void dealingEatingFood( Snake& Sn);
    void CheckCollisionwithFood();

    void GameOver();
    void checkCollisionWithWalls();
    void checkCollisionWithObstacles();
    void checkCollisionWithFixedObstacles();
    void checkCollisionWithBody();
    void checkCollisionWithBody2P();
    void checkCollisionWithBodyVSAI();
    void checkReachedTargetScore();
    void checkTimeUp();
    
    void displayScore();
    void displayTimer();
    void LoadGameOverImage(const  Image& , const  Image& , const  Image& imagep2, const  Image& , const  Image& );
    void dealingGameOver();




    void Reset();
};
