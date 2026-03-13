#include <iostream>
#include "ConstValue.h"
#include "Game.h"

using namespace std;



int main() {
    


    //Creating the Window
    InitWindow(heightWin, widthWin, "SNAKE GAME");
    SetTargetFPS(60);

    double lastUpdateTime = 0;
    double lastTime = 0;

    Game game = Game();
    
    
    //Background
    Image image = LoadImage("Images/SnakeGameBackground.png");
    ImageResize(&image, widthWin, heightWin);
    Texture2D background = LoadTextureFromImage(image);
    UnloadImage(image);

    

    bool exit = false;

    // Input -> Update -> Draw
    while (!WindowShouldClose() && !exit) {

        game.UpdateMusic();
        

        BeginDrawing();

        game.musicButton.Draw();
        bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        Vector2 mousePos = GetMousePosition();
        if (game.musicButton.isPressed(mousePos, mousePressed)) {
            game.isMuted = !game.isMuted; // Đảo trạng thái
            PlaySound(game.clickButtonSound);
            if (game.isMuted) {
                PauseMusicStream(game.musicGame);
                // Thay texture của nút sang hình Mute
                game.musicButton = game.mutedButton;
            }
            else {
                ResumeMusicStream(game.musicGame);
                // Thay texture của nút sang hình Unmute
                game.musicButton = game.unmutedButton;
                
            }
        }

        switch (game.gamestate) {

            case MENU: {
                ClearBackground(RAYWHITE);
                DrawTexture(background, 0, 0, WHITE); game.musicButton.Draw();

                DrawText("SNAKE GAME", heightWin / 2 - 6 * offset  , heightWin / 2 - 130, 100, DARKGRAY);
                
                //mousePos = GetMousePosition();
                game.startButton.UpdateHover(mousePos);
                game.exitButton.UpdateHover(mousePos);
                game.startButton.Draw();
                game.exitButton.Draw();
                

                bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT); 
                mousePos = GetMousePosition();
                if (game.startButton.isPressed(mousePos, mousePressed)) {
                    PlaySound(game.clickButtonSound);
                    cout << "Starting the Game\n";
                    game.gamestate = MODE_SELECT;
                }
                else if (game.exitButton.isPressed(mousePos, mousePressed)) {
                    PlaySound(game.clickButtonSound);
                    cout << "Exit the Game\n";
                    exit = true;
                    
                }


                break;
            }
            case MODE_SELECT: {
                ClearBackground(RAYWHITE); 
                DrawTexture(background, 0, 0, WHITE); game.musicButton.Draw();
                DrawText("SELECT A MODE", heightWin / 2 - 5 * offset + 20, heightWin / 2 - 100, 60, DARKGRAY);

                game.classicButton.UpdateHover(mousePos);
                game.duoButton.UpdateHover(mousePos);
                game.vsAIModeButton.UpdateHover(mousePos);
                game.classicButton.Draw();
                game.duoButton.Draw();
                game.vsAIModeButton.Draw();
                bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
                Vector2 mousePos = GetMousePosition();

                bool chosenMode = false;

                if (game.classicButton.isPressed(mousePos, mousePressed)) {
                    cout << "Gamemode 1 player\n";
                    game.gamemode = PLAYING1P; chosenMode = true;
                    
                }
                else if (game.duoButton.isPressed(mousePos, mousePressed)) {
                    cout << "Gamemode 2 players\n";
                    game.gamemode = PLAYING2P; 
                    chosenMode = true;
                    int scale = 2;
                    game.map.setObstaclesWithDFS(game.FP_snake, game.FP_snakep2, scale);

                    game.snake.running = game.snakep2.running = false;
                    
                }
                else if(game.vsAIModeButton.isPressed(mousePos, mousePressed)) {
                    cout << "Gamemode vs ai snake\n";
                    game.gamemode = AIBATTLE; chosenMode = true;
                    game.snake.running  = game.snakeAI.running = false;
                    
                    game.timer = game.timeLimited; // 1:00 -> 0:59 -> ... -> 0:00

                    
                }
                if (chosenMode) { PlaySound(game.clickButtonSound); game.Reset(); game.gamestate = PLAYING; }
                    

                break;
            }
            case PLAYING: { // Đặt trong 1 cặp {}
                // PLAYING PROCESS
                // ---- 1. INPUT (Xử lý phím bấm) ---- 
                
                game.snake.HandleInput1(game.running,game.snakeAI.running);
                
                if (game.gamemode == PLAYING2P) {
                    game.snakep2.HandleInput2(game.running);
                }

                if (eventTrigger(1, lastTime) && game.gamemode == AIBATTLE) {
                    if (game.snake.running || game.snakeAI.running) game.timer--;
                    //game.timer--;
                }

                // ---- 2. UPDATE (Cập nhật logic) ----
                double latency = 0.1; //second
                if (eventTrigger(latency, lastUpdateTime)) {
                    game.Update();
                }

                // ---- 3. DRAW (Vẽ lên màn hình) ----

                
                game.map.Draw();
                game.Draw();
                
                
                game.displayScore();

                if (IsKeyPressed(KEY_P)) {
                    game.gamestate = PAUSE;
                    game.snake.running = game.snakep2.running = game.snakeAI.running = false;
                    //game.isPause = true;
                }
                break;
            }
            case PAUSE: {
                
                DrawText("Press P to continue", CellSize * CellCount / 2 - 160, CellSize * CellCount / 2, 50, WHITE);
                if (IsKeyPressed(KEY_P)) {
                    game.gamestate = PLAYING;
                    game.snake.running = game.snakep2.running = game.snakeAI.running = true;
                    //game.isPause = false;
                    //////////////////////////////////////////////////////
                }
                break;
            }
            case GAMEOVER:
                {
                ClearBackground(RAYWHITE);
                DrawTexture(background, 0, 0, WHITE); game.musicButton.Draw();

                DrawText("GAME OVER", heightWin / 2 - 4.5 * offset , heightWin / 2 - 150, 80, RED);
                game.dealingGameOver();
                

                Vector2 pos1 = Vector2{ (float)(heightWin / 2 - 2.5 * offset)  , (float)(heightWin / 2 + 100) };
                Vector2 pos2 = Vector2{ (float)(heightWin / 2 - 2.5 * offset)  , (float)(heightWin / 2 - CellSize) };
                if (game.gamemode == PLAYING1P) {
                    game.playAgainButton.setPosition(pos2);
                }
                else  game.playAgainButton.setPosition(pos1);
                game.playAgainButton.UpdateHover(mousePos);
                game.playAgainButton.Draw();
                
                DrawText("Press R to return to Menu", (float)(heightWin / 2 - 2.5 * offset), heightWin / 2 + 50, 20, DARKGRAY);
                if (IsKeyPressed(KEY_R)) { game.gamestate = MENU;  break; }
                bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
                Vector2 mousePos = GetMousePosition();
                if (game.playAgainButton.isPressed(mousePos, mousePressed)) {
                    PlaySound(game.clickButtonSound);
                    cout << "restart the Game\n";
                    game.gamestate = MODE_SELECT;
                }
                break;
                }
        }

        
        EndDrawing();
    }
    UnloadTexture(background);
    CloseWindow();
    return 0;
}



/*

*/