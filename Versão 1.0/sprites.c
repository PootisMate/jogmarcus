#include "raylib.h"
#include "sprites.h"

/* Colocar os sprites depois aqui */
#define PLAYER_SPRITE_PATH "assets/images/player.png"
#define BLOCK_SPRITE_PATH "assets/images/block.png"
#define BACKGROUND_SPRITE_PATH "assets/images/background.png"

static Texture2D playerSprite;
static Texture2D blockSprite;
static Texture2D backgroundSprite;

void InitGameSprites(void)
{
    playerSprite = LoadTexture(PLAYER_SPRITE_PATH);
    blockSprite = LoadTexture(BLOCK_SPRITE_PATH);
    backgroundSprite = LoadTexture(BACKGROUND_SPRITE_PATH);
}

static void DrawSprite(Texture2D texture, Rectangle destination, Color fallbackColor)
{
    if (IsTextureValid(texture)) {
        Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
        DrawTexturePro(texture, source, destination, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
    } else {
        DrawRectangleRec(destination, fallbackColor);
    }
}

void DrawBackgroundSprite(Rectangle destination)
{
    if (IsTextureValid(backgroundSprite)) {
        Rectangle source = { 0.0f, 0.0f, (float)backgroundSprite.width,
                             (float)backgroundSprite.height };
        DrawTexturePro(backgroundSprite, source, destination,
                       (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
    }
}

void DrawPlayerSprite(Rectangle destination)
{
    DrawSprite(playerSprite, destination, SKYBLUE);
}

void DrawBlockSprite(Rectangle destination)
{
    DrawSprite(blockSprite, destination, ORANGE);
}

void CloseGameSprites(void)
{
    if (IsTextureValid(playerSprite)) UnloadTexture(playerSprite);
    if (IsTextureValid(blockSprite)) UnloadTexture(blockSprite);
    if (IsTextureValid(backgroundSprite)) UnloadTexture(backgroundSprite);
}
