//#include "ObjectClasses.h"
//using namespace std;
//
//Map::Map() {
//    Clear();
//}
//
//void Map::Clear() {
//    for (int i = 0; i < 40; i++)
//        for (int j = 0; j < 40; j++) data[i][j] = 1; // Mặc định là đường trống
//    memset(vis, false, sizeof(vis));
//}
//void Map::Update(deque<Vector2> snakeBody) {
//    Clear();
//    for (auto segment : snakeBody) {
//        //data[(int)(segment.x)][(int)(segment.y)] = 0;  // Wrong -> x:column & y:row
//        data[(int)segment.y][(int)segment.x] = 0; //Unable to move
//    }
//    // Giải phóng ô ĐẦU RẮN để BFS có thể bắt đầu chạy từ đó
//    data[(int)snakeBody[0].y][(int)snakeBody[0].x] = 1; //startPos
//    //data[(int)foodPos.y][(int)foodPos.x] = 1; //endPos Unnessary
//}
//
//void Snake::FindPathBFS(Vector2 startPos, Vector2 endPos, Map& map, deque<Vector2>& Path) {
//    //map.Clear();
//    int dr[4] = { -1,0,1,0 };
//    int dc[4] = { 0,1,0,-1 };
//    Vector2 parent[40][40];
//    // ToDo
//    bool found = false;
//    queue<Vector2> q;  q.push(startPos); map.vis[(int)startPos.y][(int)startPos.x] = true;
//
//    while (!q.empty()) {
//        Vector2 Pos = q.front(); q.pop();
//        for (int k = 0; k < 4; k++) {
//            int y1 = Pos.y + dr[k];
//            int x1 = Pos.x + dc[k];
//            if (x1 >= 0 && x1 < 40 && y1 >= 0 && y1 < 40 && !map.vis[y1][x1] && map.data[y1][x1] != 0) {
//                map.vis[y1][x1] = true;
//                parent[y1][x1] = Pos;
//                q.push(Vector2{ (float)x1 ,(float)y1 });
//
//                if (Vector2{ (float)x1 ,(float)y1 } == endPos) {
//                    found = true;
//                    break;
//                } // Found the target! Not return here 
//
//            }
//        }
//        if (found) break;
//    }
//    //Trace back
//    if (found) {
//        while (endPos != startPos) {
//            Path.push_front(endPos);
//            endPos = parent[(int)endPos.y][(int)endPos.x];
//        }
//    }
//    //Path.push_front(startPos);
//}
//
//Snake::Snake(Vector2 X1, Vector2 X2, Vector2 X3,bool isai) {
//    body = { X1,X2,X3 };
//    direction = { 1,0 };
//    addSegment = false;
//    isAI = isai;
//    score = 0;
//
//    canChangeDirection = true;
//
//    Image headImage;
//    Image bodyImage;
//    Image tailImage;
//    if (!isAI) {
//        headImage = LoadImage("Images/SnakeHead.png");
//        bodyImage = LoadImage("Images/snakeBody.png");
//        tailImage = LoadImage("Images/snakeTail.png");
//    }
//    else {
//        headImage = LoadImage("Images/AIsnake_head.png");
//        bodyImage = LoadImage("Images/AIsnake_body.png");
//        tailImage = LoadImage("Images/AIsnake_tail.png");
//    }
//    ImageResize(&headImage, CellSize, CellSize);
//    ImageResize(&bodyImage, CellSize, CellSize);
//    ImageResize(&tailImage, CellSize, CellSize);
//    
//    headTexture = LoadTextureFromImage(headImage);
//    bodyTexture = LoadTextureFromImage(bodyImage);
//    tailTexture = LoadTextureFromImage(tailImage);
//
//    UnloadImage(headImage); UnloadImage(bodyImage); UnloadImage(tailImage);
//
//
//}
//Snake::~Snake() {
//    UnloadTexture(headTexture);
//    UnloadTexture(bodyTexture);
//    UnloadTexture(tailTexture);
//}
//
//void Snake::Draw() {
//    for (unsigned int i = 0; i < body.size(); i++) {
//        float x = body[i].x;
//        float y = body[i].y;
//        //Rectangle segment = Rectangle{ offset + x * CellSize,offset + y * CellSize,(float)CellSize,(float)CellSize }
//        //DrawRectangle(x * CellSize, y * CellSize, CellSize, CellSize, darkgreen);
//        //DrawRectangleRounded(segment, 0.5, 6, darkgreen);
//        
//        if(i == 0) 
//            DrawTexture(headTexture, (float)offset + x * CellSize, (float)offset + y * CellSize, WHITE);
//        else if(i == body.size()-1) 
//            DrawTexture(tailTexture, (float)offset + x * CellSize, (float)offset + y * CellSize, WHITE);
//        else 
//            DrawTexture(bodyTexture, (float)offset + x * CellSize, (float)offset + y * CellSize, WHITE);
//    }
//}
//
//
//
//void Snake::Update() {
//    body.push_front(Vector2Add(body[0], direction));
//    if (addSegment) {
//        addSegment = false;
//    }
//    else {
//        body.pop_back();
//    }
//}
//
//void Snake::Reset() {
//    //KHởi tạo trạng thái, vị trí rắn ban đầu
//    if (!isAI) body = { Vector2{6,1}, Vector2{ 5,1 }, Vector2{ 4,1 } };
//    else body = { Vector2{6,5}, Vector2{ 5,5 }, Vector2{ 4,5 } };
//    direction = { 1,0 };
//}
//
//Food::Food(deque <Vector2> SnakeBody) {
//    Image image = LoadImage("Images/Food.png");
//
//    // Ép ảnh về đúng kích thước
//    ImageResize(&image, 5+CellSize, 5+CellSize);
//    // Đẩy lên GPU
//    texture = LoadTextureFromImage(image);
//    // Giải phóng 
//    UnloadImage(image);
//    position = GenerateRandomPos(SnakeBody); //Vị trí đầu tiên của mồi
//
//}
//
//Food::~Food() {
//    UnloadTexture(texture);
//}
//
//void Food::Draw() {
//
//    DrawTexture(texture, (float)offset + position.x * CellSize, (float)offset + position.y * CellSize, WHITE);
//
//}
//
//Vector2 Food::GetRandomCell() {
//    float x = GetRandomValue(0, CellCount - 1);
//    float y = GetRandomValue(0, CellCount - 1);
//
//    return Vector2{ x,y };
//}
//
//Vector2 Food::GenerateRandomPos(deque <Vector2> SnakeBody) {
//
//
//    Vector2 pos = GetRandomCell();
//    while (ElementOnDeque(pos, SnakeBody)) { //Truyền vào pos ko phải position
//        pos = GetRandomCell();
//    }
//    return pos;
//}
//
//
//Game::Game() {
//    InitAudioDevice();
//    eatSound = LoadSound("Sounds/eat.mp3");
//    wallColSound = LoadSound("Sounds/wall.mp3");
//}
//Game::~Game() {
//    UnloadSound(eatSound);
//    UnloadSound(wallColSound);
//    CloseAudioDevice();
//
//}
//
//
//void Game::Draw() {
//    snake.Draw();
//    snakeAI.Draw();
//    food.Draw();
//}
//void Game::Update() {
//    if (running) {
//        snakePCrashed = false;
//        snakeAICrashed = false;
//        snake.canChangeDirection = true;
//        snake.canChangeDirection = true;
//
//        if (snakeAI.isAI) {
//            // Cập nhật bản đồ dựa trên vị trí hiện tại của rắn AI
//            map.data[(int)food.position.y][(int)food.position.x] = 1;
//            map.Update(snakeAI.body);
//            map.Update(snake.body);
//            // Xóa đường đi cũ và tìm đường mới đến quả táo
//            snakeAI.PathAI.clear();
//            snakeAI.FindPathBFS(snakeAI.body[0], food.position, map, snakeAI.PathAI);
//
//            // Nếu PathAI không trống, lấy bước tiếp theo để đổi hướng
//            if (!snakeAI.PathAI.empty()) {
//                Vector2 nextStep = snakeAI.PathAI[0]; // Đây là ô sát đầu rắn nhất
//
//                // Tính toán Vector hướng: (Ô mục tiêu - Ô hiện tại)
//                // Kết quả sẽ là {1, 0}, {-1, 0}, {0, 1} hoặc {0, -1}
//                snakeAI.direction.x = nextStep.x - snakeAI.body[0].x;
//                snakeAI.direction.y = nextStep.y - snakeAI.body[0].y;
//            }
//        }
//
//
//        snake.Update();
//        snakeAI.Update();
//
//        CheckCollisionwithFood();
//        checkCollisionWithWalls();
//        checkCollisionWithBody();
//    }
//}
//
//void Game::CheckCollisionwithFood() {
//    //if (snake.body[0] == food.position) food.position = food.GenerateRandomPos();
//    if (Vector2Equals(snake.body[0], food.position)) {
//        cout << "Player is Eating food\n";
//        food.position = food.GenerateRandomPos(snake.body);
//        snake.addSegment = true;
//        ++snake.score;
//        PlaySound(eatSound);
//        //Vector2 segment = snake.body[snake.body.size() - 1];
//        //snake.body.push_back(segment);
//    }
//    if (Vector2Equals(snakeAI.body[0], food.position)) {
//        cout << "snakeAI is Eating food\n";
//        food.position = food.GenerateRandomPos(snake.body);
//        snakeAI.addSegment = true;
//        ++snakeAI.score;
//        PlaySound(eatSound);
//        //Vector2 segment = snake.body[snake.body.size() - 1];
//        //snake.body.push_back(segment);
//    }
//
//}
//
//
//void Game::GameOver() {
//    cout << "Game Over!\n";
//   
//    snake.Reset();
//    snakeAI.Reset();
//    food.position = food.GenerateRandomPos(snake.body);
//    running = false;
//    snake.score = 0;
//    snakeAI.score = 0;
//    PlaySound(wallColSound);
//    gamestate = GAMEOVER;
//}
//
//void Game::checkCollisionWithWalls() { //////////////////////////////////////
//    
//
//    if (snake.body[0].x == CellCount || snake.body[0].x < 0) snakePCrashed = true;
//    if (snake.body[0].y == CellCount || snake.body[0].y < 0) snakePCrashed = true;
//
//    if (snakeAI.body[0].x == CellCount || snakeAI.body[0].x < 0) snakeAICrashed = true;
//    if (snakeAI.body[0].y == CellCount || snakeAI.body[0].y < 0) snakeAICrashed = true;
//
//    if (snakePCrashed || snakeAICrashed) GameOver();
//}
//
//void Game::checkCollisionWithBody() {
//    //Snake without it's head
//    deque <Vector2> headlessBody = snake.body;
//    deque <Vector2> headlessBodyAI = snakeAI.body; 
//    headlessBody.pop_front(); //erase snake's head 
//    headlessBodyAI.pop_front(); //erase snakeAI's head
//    if (ElementOnDeque(snake.body[0], headlessBody)) 
//    {
//        snakePCrashed = true;
//    }
//    //AI snake
//    
//    if (ElementOnDeque(snakeAI.body[0], headlessBodyAI) ) 
//    {
//        snakeAICrashed = true;
//    }
//    if (snakePCrashed || snakeAICrashed) GameOver();
//
//}
//
//
////Button Class methods
//Button::Button(const char* imagePath, Vector2 ImagePosition, float scale) 
//{
//
//    Image image = LoadImage(imagePath);
//    int originalWidth = image.width;
//    int originalHeigth = image.height;
//
//    int newWidth = static_cast<int>(originalWidth * scale);
//    int newHeight = static_cast<int>(originalHeigth * scale);
//
//    ImageResize(&image, newWidth, newHeight);
//    texture = LoadTextureFromImage(image);
//    UnloadImage(image);
//
//    position = ImagePosition;
//    void Draw();
//}
//
//Button::~Button() 
//{
//    UnloadTexture(texture);
//}
//
//void Button::Draw() 
//{
//    DrawTextureV(texture, position, WHITE);
//}
//
//bool Button::isPressed(Vector2 mousePos, bool mousePressed) 
//{
//    Rectangle rect = { position.x,position.y,static_cast<int>(texture.width),static_cast<int>(texture.height) };
//
//    if (CheckCollisionPointRec(mousePos, rect) && mousePressed) {
//        return true;
//    }
//    return false;
//}
//
