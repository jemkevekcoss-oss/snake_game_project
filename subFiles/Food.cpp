#include "Food.h"

using namespace std;

Food::Food(deque <Vector2> SnakeBody) {
    //Image image = LoadImage("Images/Food.png");
    Image image = LoadImage("Images/Apple.png");
    Image imageG = LoadImage("Images/Golden_Apple.png");
    // Ép ảnh về đúng kích thước
    ImageResize(&image, 5 + CellSize, 5 + CellSize);
    ImageResize(&imageG, 5 + CellSize, 5 + CellSize);
    // Đẩy lên GPU
    
    texture = LoadTextureFromImage(image);
    textureGolden = LoadTextureFromImage(imageG);
    // Giải phóng 
    UnloadImage(image);
    UnloadImage(imageG);
    position = GenerateRandomPos(SnakeBody); //Vị trí đầu tiên của mồi

}
//"Images/Apple.bmp"
Food::~Food() {
    UnloadTexture(texture);
}

void Food::Draw() {
    if(!isGolden)
        DrawTexture(texture, (float)offset + position.x * CellSize, (float)offset + position.y * CellSize, WHITE);
    else 
        DrawTexture(textureGolden, (float)offset + position.x * CellSize, (float)offset + position.y * CellSize, WHITE);

}

Vector2 Food::GetRandomCell() {
    float x = GetRandomValue(0, CellCount - 1);
    float y = GetRandomValue(0, CellCount - 1);

    return Vector2{ x,y };
}

Vector2 Food::GenerateRandomPos(deque <Vector2> SnakeBody1) {

    Vector2 pos = GetRandomCell();
    while (ElementOnDeque(pos, SnakeBody1) ) { //Truyền vào pos ko phải position
        pos = GetRandomCell();
    }
    return pos;
}
//For mode 2 players
Vector2 Food::GenerateRandomPos2(deque <Vector2> SnakeBody1, deque <Vector2> SnakeBody2,  int maze[40][40]) { //Chu y khi truyen mang 2 chieu
    Vector2 pos = GetRandomCell();
    while (ElementOnDeque(pos, SnakeBody1) || ElementOnDeque(pos, SnakeBody2) || maze[(int)pos.y][(int)pos.x] == obstacle || maze[(int)pos.y][(int)pos.x] == fixObstacle) { //Truyền vào pos ko phải position
        pos = GetRandomCell();
    }
    return pos;
}


