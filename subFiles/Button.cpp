#include "Button.h"

using namespace std;

//Button Class methods
Button::Button(const char* imagePath, Vector2 ImagePosition, float scale)
{
    


    Image image = LoadImage(imagePath);
    int originalWidth = image.width;
    int originalHeigth = image.height;

    int newWidth = static_cast<int>(originalWidth * scale);
    int newHeight = static_cast<int>(originalHeigth * scale);

    ImageResize(&image, newWidth, newHeight);
    texture = LoadTextureFromImage(image);
    UnloadImage(image);

    position = ImagePosition;
    //void Draw();
}

Button::~Button()
{
    UnloadTexture(texture);
    
}

void Button::Draw()
{
    Color tint = (isHovered ? Fade(WHITE, 0.7f) : WHITE);
    DrawTextureV(texture, position, tint);
}

bool Button::isPressed(Vector2 mousePos, bool mousePressed)
{
    Rectangle rect = { position.x,position.y,static_cast<int>(texture.width),static_cast<int>(texture.height) };

    if (CheckCollisionPointRec(mousePos, rect) && mousePressed) {
        
        return true;
    }
    return false;
}

void Button::setPosition(Vector2 Pos) {
    position = Pos;
}

void Button::UpdateHover(Vector2 mousePos) {
    Rectangle rect = { position.x,position.y,static_cast<int>(texture.width),static_cast<int>(texture.height) };
    if (CheckCollisionPointRec(mousePos, rect)) {
        isHovered = true;
    } else isHovered = false;
}
