#include "Map.h"


using namespace std;


Map::Map() {
    Clear();
}
Map::~Map() {
    UnloadTexture(textureMap);
    UnloadTexture(textureObstacle);
}


void Map::Clear() {
    //for (int i = 0; i < 40; i++)
    //    for (int j = 0; j < 40; j++) data[i][j] = 1; // Mặc định là đường trống
    memset(vis, false, sizeof(vis));
}

void Map::Update(deque<Vector2> snakeBody) {
    //Clear();
    
    for (auto segment : snakeBody) {
        //data[(int)(segment.x)][(int)(segment.y)] = 0;  // Wrong -> x:column & y:row
        data[(int)segment.y][(int)segment.x] = bodyBlock; //Unable to move
    }
    // Giải phóng ô ĐẦU RẮN để BFS có thể bắt đầu chạy từ đó
    data[(int)snakeBody[0].y][(int)snakeBody[0].x] = validBlock; //startPos
    
}
void Map::setMap(const char* imagePath) {

    Image imageMap = LoadImage(imagePath);
    ImageResize(&imageMap, CellSize*CellCount, CellSize * CellCount);
    textureMap = LoadTextureFromImage(imageMap);
    UnloadImage(imageMap);
    
}
void Map::Draw() {
    ClearBackground(WHITE);
    DrawRectangleLinesEx(Rectangle{ offset - 5,offset - 5,(float)CellSize * CellCount + 10,(float)CellSize * CellCount + 10 }, 5, darkgreen);
    DrawText("SNAKE GAME", offset, offset - 40, 35, darkgreen);
    DrawTexture(textureMap, offset , offset , WHITE);

}

void Map::loadImageObstacle(const char* imagePath) {
    Image imageObs = LoadImage(imagePath);
    ImageResize(&imageObs, CellSize, CellSize);
    textureObstacle = LoadTextureFromImage(imageObs);
    UnloadImage(imageObs);
}
void Map::loadImageFixedObstacle(const char* imagePath) {
    Image imageFObs = LoadImage(imagePath);
    ImageResize(&imageFObs, CellSize, CellSize);
    textureFixedObstacle = LoadTextureFromImage(imageFObs);
    UnloadImage(imageFObs);
}
void Map::setObstaclesWithDFS(snakeFirstPos FP1, snakeFirstPos FP2,const int& scale) {
    
    
    memset(maze, obstacle, sizeof(maze));

    stack <pair<int, int>> st;
    st.push({ 1,1 }); maze[1][1] = 1;
    int dx[] = { 0, 0, scale, -scale };
    int dy[] = { scale, -scale, 0, 0 };

    //srand(time(0));

    mt19937 gen(time(0));

    while (!st.empty()) {
        pair<int, int> cur = st.top(); st.pop();

        int x = cur.first;
        int y = cur.second;

        vector<int> dirs = { 0,1,2,3 };
        shuffle(dirs.begin(), dirs.end(),gen);

        for (int dir : dirs) {
            int xn = x + dx[dir];
            int yn = y + dy[dir];

            if (xn > 0 && xn < CellCount && yn>0 && yn < CellCount && maze[xn][yn] == obstacle) {
                maze[xn][yn] = validBlock;
                //maze[x + dx[dir] / 2][y + dy[dir] / 2] = validBlock;
                for (int i = 1; i < scale; i++) {
                    maze[x + dx[dir] / scale][y + dy[dir] / scale] = validBlock;
                }
                st.push(cur); st.push({ xn,yn });

                break;
            }
        }
    }
    for (int i = 0; i < CellCount; i++) { maze[0][i] = maze[i][0] = 1; }

    float sparseness = 0.85f; // 90% tường sẽ bị xóa
    for (int i = 1; i < CellCount ; i++) {
        for (int j = 1; j < CellCount ; j++) {
            if (maze[i][j] == obstacle) { // Nếu là tường
                if ((float)rand() / RAND_MAX < sparseness) {
                    maze[i][j] = validBlock; // Biến tường thành đường trống
                }
            }
        }
    }
    maze[(int)FP1.Poshead.y][(int)FP1.Poshead.x] = maze[(int)FP1.Posbody.y][(int)FP1.Posbody.x] = maze[(int)FP1.Postail.y][(int)FP1.Postail.x] = validBlock;
    maze[(int)FP2.Poshead.y][(int)FP2.Poshead.x] = maze[(int)FP2.Posbody.y][(int)FP2.Posbody.x] = maze[(int)FP2.Postail.y][(int)FP2.Postail.x] = validBlock;
}
void Map::DrawObstacleWithDFS() {

    for (int y = 0; y < 40; y++) {
        for (int x = 0; x < 40; x++) {
            if (maze[y][x] == obstacle) {
                DrawTexture(textureFixedObstacle, offset + x * CellSize, offset + y * CellSize, WHITE);
            }
        }
    }

}

void Map::setFixedObstacles() {
    //Bring in from file text "Map.txt"
    ifstream in("MAP.txt");

    for (int i = 0; i < 40; i++) {
        for (int j = 0; j < 40; j++) {
            in >> data[i][j];
        }
    }

    in.close();
}

void Map::DrawFixedObstacle() {
    
    for (int y = 0; y < 40; y++) {
        for (int x = 0; x < 40; x++) {
            if (data[y][x] == fixObstacle) {
                DrawTexture(textureObstacle, offset + x * CellSize, offset + y * CellSize, WHITE);
            }
        }
    }
}



