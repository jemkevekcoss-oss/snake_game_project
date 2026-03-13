#pragma once
#include "ConstValue.h"

class Button {
public:

    bool isHovered = false; // Biến mới để check hover
    

    Button(const char* imagePath, Vector2 ImagePosition, float scale);
    ~Button();
    void Draw();

    bool isPressed(Vector2 mousePos, bool mousePressed);
    void setPosition(Vector2);
    void UpdateHover(Vector2);

private:
    Texture2D texture;
    Vector2 position;
};


