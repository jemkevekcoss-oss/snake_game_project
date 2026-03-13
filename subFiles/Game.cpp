#include "Game.h"


using namespace std;

Game::Game() {
    map.setMap("Images/BackgroudMap.png");
    map.loadImageObstacle("Images/lavaBlock.png");
    map.loadImageFixedObstacle("Images/goldenBlock.png");
    loadHighScore();
        
     imagey = LoadImage("Images/youwin.png");
     imagep1 = LoadImage("Images/player01win.png");
     imagep2 = LoadImage("Images/player02win.png");
     imageai = LoadImage("Images/aisnakewin.png");
     imageDraw = LoadImage("Images/drawResult.png");

        ImageResize(&imagey, static_cast<int>(CellSize * 11), static_cast<int>(CellSize * 4));
        ImageResize(&imagep1, static_cast<int>(CellSize * 11), static_cast<int>(CellSize * 4));
        ImageResize(&imagep2, static_cast<int>(CellSize * 11), static_cast<int>(CellSize * 4));
        ImageResize(&imageai, static_cast<int>(CellSize * 11), static_cast<int>(CellSize * 4));
        ImageResize(&imageDraw, static_cast<int>(CellSize * 11), static_cast<int>(CellSize * 4));

        LoadGameOverImage(imagey, imagep1, imagep2, imageai,imageDraw);
        UnloadImage(imagey); UnloadImage(imagep1); UnloadImage(imagep2); UnloadImage(imageai); UnloadImage(imageDraw);


    InitAudioDevice();
    eatSound = LoadSound("Sounds/eat.mp3");
    wallColSound = LoadSound("Sounds/wall.mp3");
    clickButtonSound = LoadSound("Sounds/clickButtonSound.mp3");
    musicGame = LoadMusicStream("Sounds/snakeGameMusic.mp3");
    isMuted = false;
    PlayMusicStream(musicGame); // Bắt đầu phát
    SetMusicVolume(musicGame, 0.4f); // Chỉnh âm lượng vừa phải
    musicGame.looping = true; // Cho nhạc tự động lặp lại khi hết 3 phút
}
Game::~Game() {
    UnloadMusicStream(musicGame);
    UnloadSound(eatSound);
    UnloadSound(wallColSound);
    UnloadSound(clickButtonSound);
    CloseAudioDevice();

    UnloadTexture(youWin);
    UnloadTexture(player1Win);
    UnloadTexture(player2Win);
    UnloadTexture(aisnakeWin);
    
}

void Game::UpdateMusic() { 
    UpdateMusicStream(musicGame); 
}


void Game::Draw() {
    
        snake.Draw();
        food.Draw();

        if (gamemode == PLAYING1P) {
            string highestScore = "Highest Score: " + to_string(highScore);
            DrawText(highestScore.c_str(), offset + CellSize, offset + CellSize * CellCount + 10,30,GREEN);
        }

        else if (gamemode == PLAYING2P) {

            snakep2.Draw();
            map.DrawObstacleWithDFS();
        }
        else if (gamemode == AIBATTLE) {
            displayTimer();
            snakeAI.Draw(); 
            map.DrawFixedObstacle();
        }
    
}
void Game::dealingAIPath() {
    // Cập nhật bản đồ dựa trên vị trí hiện tại của rắn AI
    map.Clear();
    map.setFixedObstacles();
    map.data[(int)food.position.y][(int)food.position.x] = validBlock;
    map.Update(snakeAI.body);
    map.Update(snake.body);
    // Xóa đường đi cũ và tìm đường mới đến quả táo
    snakeAI.PathAI.clear();
    snakeAI.FindPathBFS(snakeAI.body[0], food.position, map, snakeAI.PathAI);

    // Nếu PathAI không trống, lấy bước tiếp theo để đổi hướng
    if (!snakeAI.PathAI.empty()) {
        Vector2 nextStep = snakeAI.PathAI[0]; // Đây là ô sát đầu rắn nhất

        // Tính toán Vector hướng: (Ô mục tiêu - Ô hiện tại)
        // Kết quả sẽ là {1, 0}, {-1, 0}, {0, 1} hoặc {0, -1}
        snakeAI.direction.x = nextStep.x - snakeAI.body[0].x;
        snakeAI.direction.y = nextStep.y - snakeAI.body[0].y;
    }
    else {
        // direction = FindSurvivalDirection(map);
        Vector2 head = snakeAI.body[0];
        Vector2 dirs[] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} }; // Up,Down,Left,Right
        bool foundWay = false;

        // Duyệt qua các hướng để tìm một ô "hợp lệ" (data == 1)
        for (Vector2 d : dirs) {
            // Tránh quay đầu 180 độ (rất quan trọng để không tự cắn cổ)
            if (d.x == -snakeAI.direction.x && d.y == -snakeAI.direction.y) continue; 

            int nextY = (int)head.y + (int)d.y; // Row
            int nextX = (int)head.x + (int)d.x; // Column

            // Kiểm tra biên để tránh truy cập ngoài mảng data[col][row]
            if (nextX >= 0 && nextX < CellCount && nextY >= 0 && nextY < CellCount) {
                // Nếu ô đó là đường trống (data == 1)
                if (map.data[nextY][nextX] == 1) {
                    snakeAI.direction = d;
                    foundWay = true;
                    break; // Tìm thấy một hướng là "vọt" luôn
                }
            }
        }

    }
}

void Game::Update() {
    
    if (running ) {
            
            snakePCrashed = snakeP2Crashed = snakeAICrashed = false;
            snake.canChangeDirection = true; snakep2.canChangeDirection = true;

            if (gamemode == AIBATTLE ) { 
                dealingAIPath();
                if (snakeAI.running) snakeAI.Update();
                //snakeAI.Update();
            }
            
            if (snake.running) { snake.Update(); }

            if (gamemode == PLAYING1P) {
                checkCollisionWithBody(); 
                checkNewHighScore();
            }

            
            if (gamemode == PLAYING2P ) { 
                if(snakep2.running) snakep2.Update();
                checkCollisionWithObstacles();
                checkCollisionWithBody2P();
                checkReachedTargetScore(); ///////////
            }
            
            else if (gamemode == AIBATTLE) {
                checkTimeUp();
                checkCollisionWithBodyVSAI(); 
                checkCollisionWithFixedObstacles();
            }
            
            if (!running) return; //////////
            CheckCollisionwithFood();
            checkCollisionWithWalls();    
    }
}

void Game::dealingEatingFood(Snake& Sn) {

    if (gamemode == PLAYING1P) {
        food.position = food.GenerateRandomPos(snake.body);
    }
    else if (gamemode == PLAYING2P) {
        food.position = food.GenerateRandomPos2(snake.body, snakep2.body, map.maze);
    }
    else if (gamemode == AIBATTLE) {
        food.position = food.GenerateRandomPos2(snake.body, snakeAI.body, map.data);
    }

    Sn.addSegment = true;
    if(food.isGolden) Sn.score += 15;
    else Sn.score += 10;
    PlaySound(eatSound);

    if (gamemode == PLAYING1P) food.isGolden = false;
    else {
        int chance = GetRandomValue(1, 100);
        if (chance <= 20) { // 20% tỉ lệ ra táo vàng
            food.isGolden = true;
        }
        else {
            food.isGolden = false;
        }
    }
    
}

void Game::CheckCollisionwithFood() {
    //if (snake.body[0] == food.position) food.position = food.GenerateRandomPos();
    if (gamemode == PLAYING1P  ) {
        if (Vector2Equals(snake.body[0], food.position)) {
            cout << "Player is Eating food\n";
            dealingEatingFood(snake);
        }
        
    }
    else if (gamemode == PLAYING2P ) {
        if (Vector2Equals(snake.body[0], food.position)) {
            cout << "Player1 is Eating food\n";
            dealingEatingFood(snake);
        }
        else if (Vector2Equals(snakep2.body[0], food.position)) {
            cout << "Player2 is Eating food\n";
            dealingEatingFood(snakep2);
        }
    }
    else if (gamemode == AIBATTLE ) {
        if (Vector2Equals(snake.body[0], food.position)) {
            cout << "Your snake is Eating food\n";
            dealingEatingFood(snake);
        }
        else if(Vector2Equals(snakeAI.body[0], food.position)) {
            cout << "AI snake is Eating food\n";
            dealingEatingFood(snakeAI);
        }
        
    }
}

void Game::GameOver() {
    
    snake.Reset();
    snakeAI.Reset();
    food.position = food.GenerateRandomPos(snake.body); //Mode 1 player
    

    running = false;
    if (!((gamemode == PLAYING2P) && (snake.score >= TARGET_SCORE || snakep2.score >= TARGET_SCORE))
        && !(gamemode == AIBATTLE && timer <= 0) )
        PlaySound(wallColSound);
    

    snake.score = 0;
    snakep2.score = 0;
    snakeAI.score = 0;
    
    gamestate = GAMEOVER;
    cout << "Game Over!\n";
    
}

void Game::checkCollisionWithWalls() { //////////////////////////////////////

    if (snake.body[0].x == CellCount || snake.body[0].x < 0) snakePCrashed = true;
    else if (snake.body[0].y == CellCount || snake.body[0].y < 0) snakePCrashed = true;

    if (gamemode == PLAYING2P) {
        if (snakep2.body[0].x == CellCount || snakep2.body[0].x < 0) snakeP2Crashed = true;
        else if (snakep2.body[0].y == CellCount || snakep2.body[0].y < 0) snakeP2Crashed = true;
    }

    if (gamemode == AIBATTLE) {
        if (snakeAI.body[0].x == CellCount || snakeAI.body[0].x < 0) snakeAICrashed = true;
        else if (snakeAI.body[0].y == CellCount || snakeAI.body[0].y < 0) snakeAICrashed = true;
    }

    if (snakePCrashed || snakeP2Crashed || snakeAICrashed)
    {
        if (snakePCrashed) cout << "Player1 Colided with walls\n";
        if (snakeP2Crashed) cout << "Player2 Colided with walls\n";
        if (snakeAICrashed) cout << "AI snake Colided with walls\n";
        GameOver();
        
    }
}
void Game::checkCollisionWithObstacles() {
    if (map.maze[(int)snake.body[0].y][(int)snake.body[0].x] == obstacle) { snakePCrashed = true; }
    if (map.maze[(int)snakep2.body[0].y][(int)snakep2.body[0].x] == obstacle) { snakeP2Crashed = true; }

    if (snakePCrashed || snakeP2Crashed )
    {
        if (snakePCrashed) cout << "Player1 Colided with Obstacle\n";
        if (snakeP2Crashed) cout << "Player2 Colided with Obstacle\n";
       
        GameOver();
    }
}
void Game::checkCollisionWithFixedObstacles() {
    if (map.data[(int)snake.body[0].y][(int)snake.body[0].x] == fixObstacle) { snakePCrashed = true; }
    if (map.data[(int)snakeAI.body[0].y][(int)snakeAI.body[0].x] == fixObstacle) { snakeAICrashed = true; }

    if (snakePCrashed || snakeAICrashed)
    {
        if (snakePCrashed) cout << "your snake Colided with Obstacle\n";
        if (snakeAICrashed) cout << "AI snake Colided with Obstacle\n";

        GameOver();
    }

}

void Game::checkCollisionWithBody() {
    //Snake without it's head
    deque <Vector2> headlessBody = snake.body;

    headlessBody.pop_front(); //erase snake's head

    if (ElementOnDeque(snake.body[0], headlessBody) )
    {
        snakePCrashed = true;
    }
    if (snakePCrashed) {
        cout << "Snake hit body\n";  GameOver();

    }
}

void Game::checkCollisionWithBody2P() {
    //Snake without it's head
    deque <Vector2> headlessBody = snake.body;
    deque <Vector2> headlessBody2 = snakep2.body;
    headlessBody.pop_front(); //erase snake's head
    headlessBody2.pop_front();

    if (ElementOnDeque(snake.body[0], headlessBody) || ElementOnDeque(snake.body[0], headlessBody2)) 
        snakePCrashed = true;
    if (ElementOnDeque(snakep2.body[0], headlessBody2) || ElementOnDeque(snakep2.body[0], headlessBody))
        snakeP2Crashed = true;

    if (snakePCrashed || snakeP2Crashed) {
        if (snakePCrashed) cout << "Player1 Colided with body\n";
        if (snakeP2Crashed) cout << "Player2 Colided with body\n";
        GameOver();
    }
}
void Game::checkCollisionWithBodyVSAI() {

    //Snake without it's head
    deque <Vector2> headlessBody = snake.body;
    deque <Vector2> headlessBodyAI = snakeAI.body;
    headlessBody.pop_front(); //erase snake's head
    headlessBodyAI.pop_front();

    if (ElementOnDeque(snake.body[0], headlessBody) || ElementOnDeque(snake.body[0], headlessBodyAI))
        snakePCrashed = true;
    else if (ElementOnDeque(snakeAI.body[0], headlessBodyAI) || ElementOnDeque(snakeAI.body[0], headlessBody))
        snakeAICrashed = true;

    if (snakePCrashed || snakeAICrashed) {
        if (snakePCrashed) cout << "Player Colided with body\n";
        if (snakeAICrashed) cout << "AI snake Colided with body\n";
        GameOver();
    }

}

void Game::checkReachedTargetScore() {
    if (gamemode == PLAYING2P) {
        if (snake.score >= TARGET_SCORE) {
            snakeP2Crashed = true; 
            cout << "Player 1 reached target!\n";
            GameOver();
        }
        else if (snakep2.score >= TARGET_SCORE) {
            snakePCrashed = true; 
            cout << "Player 2 reached target!\n";
            GameOver();
        }
    }
}


void Game::displayScore() {
    
    string Score1,Score2; 
    if (gamemode == PLAYING1P) Score1 = "YOUR SCORE: ";
    else if (gamemode == PLAYING2P) {

        Score1 = "PLAYER1 SCORE: ";
        Score2 = "PLAYER2 SCORE: " + to_string(snakep2.score);
        
        string TS = "Target Score: "  + to_string(TARGETSCORE) ;
        DrawText(TS.c_str(), (float)widthWin / 2 - 2 * offset, offset + CellSize * CellCount + 10, 30, GREEN);
    }
    else if(gamemode == AIBATTLE) {
        Score1 = "YOUR SCORE: ";
        Score2 = "AI SNAKE SCORE: " + to_string(snakeAI.score);
        
    }
    Score1 += to_string(snake.score);
    


    DrawText(Score1.c_str(), (float)CellSize * CellCount - 180, offset + CellSize * CellCount + 10, 25, RED);
    DrawText(Score2.c_str(), (float)offset + CellSize, offset + CellSize * CellCount + 10, 25, RED);

}
void Game::loadHighScore() {
    ifstream scoreFile("highscore.txt");
    if (scoreFile.is_open()) {
        scoreFile >> highScore; // Đọc con số từ file vào biến
        scoreFile.close();
    } else {
        highScore = 0; 
    }
}

void Game::checkNewHighScore() {
    if (snake.score > highScore) {
        highScore = snake.score;
        ofstream scoreFile("highscore.txt");
        if (scoreFile.is_open()) {
            scoreFile << highScore; // Ghi giá trị mới vào file
            scoreFile.close();
        } // Lưu ngay lập tức
    }
}

void Game::displayTimer() {
    
    int timeLeft = timer;
    int min = timer / 60;
    int sec = timer % 60;
    string T = to_string(min) + ":" + to_string(sec);

    DrawText(T.c_str(), (float)widthWin / 2 -  0.5*offset, offset + CellSize * CellCount + 10, 50, GREEN);
}
void Game::checkTimeUp() {
    if (timer <= 0) {
        if (snake.score == snakeAI.score) {
            snakePCrashed = snakeAICrashed = true;
            cout << "DRAW\n";
        }
        else if (snake.score > snakeAI.score) {
            snakeAICrashed = true;
            cout << "Player Win\n";
        }
        else {
            snakePCrashed = true;
            cout << "AI snake wins\n";
        }
        GameOver();
    }
}

void Game::Reset() {
    snake.Reset(); 
    
   
    if (gamemode == AIBATTLE) {
        map.Clear();
        snakeAI.Reset(); 
    }
    if (gamemode == PLAYING2P) {
        snakep2.Reset();
    }

    snakePCrashed = false;
    snakeAICrashed = false;
    //running = true; 
}
void Game::LoadGameOverImage(const  Image& imagey, const  Image& imagep1, const  Image& imagep2, const  Image& imageai, const  Image& imageDraw) {
    youWin = LoadTextureFromImage(imagey);
    player1Win = LoadTextureFromImage(imagep1);
    player2Win = LoadTextureFromImage(imagep2);
    aisnakeWin = LoadTextureFromImage(imageai);
    drawResult = LoadTextureFromImage(imageDraw);
}
void Game::dealingGameOver() {

    if (gamemode == PLAYING2P) {
        if (snakeP2Crashed)
            DrawTexture(player1Win, heightWin / 2 - 2.5 * offset, heightWin / 2 - 80, WHITE);
        else if (snakePCrashed)
            DrawTexture(player2Win, heightWin / 2 - 2.5 * offset, heightWin / 2 - 80, WHITE);
    }
    else if (gamemode == AIBATTLE) {
        if (snakePCrashed && snakeAICrashed) {
            DrawTexture(drawResult, (float)(heightWin / 2 - 2.5 * offset), heightWin / 2 - 80, WHITE);
            return;
        }
        if (snakePCrashed)
            DrawTexture(aisnakeWin, heightWin / 2 - 2.5 * offset, heightWin / 2 - 80, WHITE);
        else if (snakeAICrashed)
            DrawTexture(youWin, heightWin / 2 - 2.5 * offset, heightWin / 2 - 80, WHITE);
        
    }

}

  

