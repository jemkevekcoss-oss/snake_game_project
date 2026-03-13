#include "Snake.h"

using namespace std;

void Snake::FindPathBFS(Vector2 startPos, Vector2 endPos, Map& map, deque<Vector2>& Path) {
    //map.Clear();
    int dr[4] = { -1,0,1,0 };
    int dc[4] = { 0,1,0,-1 };
    Vector2 parent[40][40];
    // ToDo
    bool found = false;
    queue<Vector2> q;  q.push(startPos); map.vis[(int)startPos.y][(int)startPos.x] = true;

    while (!q.empty()) {
        Vector2 Pos = q.front(); q.pop();
        for (int k = 0; k < 4; k++) {
            int y1 = Pos.y + dr[k];
            int x1 = Pos.x + dc[k];
            if (x1 >= 0 && x1 < 40 && y1 >= 0 && y1 < 40 && !map.vis[y1][x1] && map.data[y1][x1] != bodyBlock && map.data[y1][x1] != fixObstacle) {
                map.vis[y1][x1] = true;
                parent[y1][x1] = Pos;
                q.push(Vector2{ (float)x1 ,(float)y1 });

                if (Vector2{ (float)x1 ,(float)y1 } == endPos) {
                    found = true;
                    break;
                } // Found the target! Not return here 

            }
        }
        if (found) break;
    }
    //Trace back
    if (found) {
        while (endPos != startPos) {
            Path.push_front(endPos);
            endPos = parent[(int)endPos.y][(int)endPos.x];
        }
    }
    //Path.push_front(startPos);
}

Snake::Snake(const snakeFirstPos& FP, bool isai,bool isplayer1) {
    running = false; //Do not move at first
    

    body = { FP.Poshead,FP.Posbody,FP.Postail };
    direction = { 1,0 };
    addSegment = false;
    isAI = isai;
    score = 0;
    this->isplayer1 = isplayer1;

    canChangeDirection = true;

    Image headImage;
    Image bodyImage;
    Image tailImage;
    if (isAI) {
            headImage = LoadImage("Images/AIHead.png");
            bodyImage = LoadImage("Images/AIBody.png");
            tailImage = LoadImage("Images/AITail.png");
    }
    else {
        if (isplayer1) {
            headImage = LoadImage("Images/Player01Head.png");
            bodyImage = LoadImage("Images/Player01Body.png");
            tailImage = LoadImage("Images/Player01Tail.png");
        }
        else {
            headImage = LoadImage("Images/Player02Head.png");
            bodyImage = LoadImage("Images/Player02Body.png");
            tailImage = LoadImage("Images/Player02Tail.png");
        }
    }
    ImageResize(&headImage, CellSize, CellSize);
    ImageResize(&bodyImage, CellSize, CellSize);
    ImageResize(&tailImage, CellSize, CellSize);

    headTexture = LoadTextureFromImage(headImage);
    bodyTexture = LoadTextureFromImage(bodyImage);
    tailTexture = LoadTextureFromImage(tailImage);

    UnloadImage(headImage); UnloadImage(bodyImage); UnloadImage(tailImage);


}
Snake::~Snake() {
    UnloadTexture(headTexture);
    UnloadTexture(bodyTexture);
    UnloadTexture(tailTexture);
}

void Snake::Draw() {
    for (unsigned int i = 0; i < body.size(); i++) {
        float x = body[i].x;
        float y = body[i].y;
        // 1. Xác định vị trí vẽ (phải cộng thêm nửa ô để xoay quanh tâm)
        Rectangle dest = { (float)offset + x * CellSize + (float)CellSize / 2,(float)offset + y * CellSize + (float)CellSize / 2,(float)CellSize,(float)CellSize };
        // 2. Tâm xoay là ở giữa ảnh (Tọa độ)
        Vector2 center = Vector2{(float)CellSize/2,(float)CellSize / 2 };
        // 3. Tính góc quay dựa trên hướng (direction)
        float rotation = 0; //Degree


        if (i == 0) {
       
            if (direction.x == 1) rotation = 0;
            else if (direction.x == -1) rotation = 180;
            else if (direction.y == 1) rotation = 90;
            else if (direction.y == -1) rotation = 270;
            Rectangle source = Rectangle{ 0,0,(float)headTexture.width,(float)headTexture.height };
            DrawTexturePro(headTexture, source, dest, center, rotation, WHITE);
        }

        else if (i == body.size() - 1) {

            //Depend on the front position of the Tail
            Vector2 tailDir = Vector2Subtract(body[i - 1], body[i]);
            if (tailDir.x == 1) rotation = 0;
            else if (tailDir.x == -1) rotation = 180;
            else if (tailDir.y == 1) rotation = 90;
            else if (tailDir.y == -1) rotation = 270;

            Rectangle source = Rectangle{ 0,0,(float)tailTexture.width,(float)tailTexture.height };
            DrawTexturePro(tailTexture, source, dest, center, rotation, WHITE);
        }
        else {
            //Depend on the front position of the snake body
            Vector2 bodyDir = Vector2Subtract(body[i - 1], body[i]);
            if (bodyDir.x == 1) rotation = 0;
            else if (bodyDir.x == -1) rotation = 180;
            else if (bodyDir.y == 1) rotation = 90;
            else if (bodyDir.y == -1) rotation = 270;
            Rectangle source = Rectangle{ 0,0,(float)bodyTexture.width,(float)bodyTexture.height };
            DrawTexturePro(bodyTexture, source, dest, center, rotation, WHITE);
        }
            


    }
}


void Snake::Update() {
    body.push_front(Vector2Add(body[0], direction));
    if (addSegment) {
        addSegment = false;
    }
    else {
        body.pop_back();
    }
}

void Snake::Reset() {
    //KHởi tạo trạng thái, vị trí rắn ban đầu
    if (isAI) body = { Vector2{ 2,15 }, Vector2{ 1,15 }, Vector2{ 0,15 } };
    else if(isplayer1)
        body = { Vector2{2,0}, Vector2{ 1,0 }, Vector2{ 0,0 } };
    else 
        body = { Vector2{2,39}, Vector2{ 1,39 }, Vector2{ 0,39 } };
    score = 0; addSegment = false;
    running = false;
    ///////////////////////////////////////////////////////////////////////////////////
    direction = { 1,0 };
}

void Snake::HandleInput1(bool& runningGame,bool& runningAI) {
    if (!canChangeDirection) return; // NOT DOING ANYINGTHING IF LOCKED
    
    if (IsKeyPressed(KEY_UP) && direction.y != 1) {
        direction = { 0, -1 };
        canChangeDirection = false;
        runningGame = true; running = true; runningAI = true;
    }
    else if (IsKeyPressed(KEY_DOWN) && direction.y != -1) {
        direction = { 0, 1 };
        canChangeDirection = false;
        runningGame = true; running = true; runningAI = true;
    }
    else if (IsKeyPressed(KEY_LEFT) && direction.x != 1) {
        direction = { -1, 0 };
        canChangeDirection = false;
        runningGame = true; running = true; runningAI = true;
    }
    else if (IsKeyPressed(KEY_RIGHT) && direction.x != -1) {
        direction = { 1, 0 };
        canChangeDirection = false;
        runningGame = true; running = true; runningAI = true;
    }
}

void Snake::HandleInput2(bool& runningGame) {
    if (!canChangeDirection) return; // NOT DOING ANYINGTHING IF LOCKED

    if (IsKeyPressed(KEY_W) && direction.y != 1) {
        direction = { 0, -1 };
        canChangeDirection = false;
        runningGame = true; running = true;
    }
    else if (IsKeyPressed(KEY_S) && direction.y != -1) {
        direction = { 0, 1 };
        canChangeDirection = false;
        runningGame = true; running = true;
    }
    else if (IsKeyPressed(KEY_A) && direction.x != 1) {
        direction = { -1, 0 };
        canChangeDirection = false;
        runningGame = true; running = true;
    }
    else if (IsKeyPressed(KEY_D) && direction.x != -1) {
        direction = { 1, 0 };
        canChangeDirection = false;
        runningGame = true; running = true;
    }
}

       