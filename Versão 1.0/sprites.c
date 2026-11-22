#include "raylib.h"
#include "sprites.h"

#define PLAYER_SPRITE_PATH "sprites/Bun_Idle.png"
#define PLAYER_WALK_SPRITE_PATH "sprites/Bun_Walks1.gif"
#define PLAYER_JUMP_SPRITE_PATH "sprites/Bun_Jump.png"
#define BLOCK_SPRITE_PATH "sprites/brick.png"
#define BACKGROUND_SPRITE_PATH "sprites/background.png"
#define PLAYER_WALK_FPS 10.0f
#define PLAYER_JUMP_FPS 8.0f

static Texture2D playerSprite;
static Texture2D playerWalkSprite;
static Texture2D playerJumpSprite;
static Texture2D blockSprite;
static Texture2D backgroundSprite;
static float playerAnimationTime;
static int playerAnimationState = -1;

void InitGameSprites(void)
{
    playerSprite = LoadTexture(PLAYER_SPRITE_PATH);
    playerWalkSprite = LoadTexture(PLAYER_WALK_SPRITE_PATH);
    playerJumpSprite = LoadTexture(PLAYER_JUMP_SPRITE_PATH);
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

void DrawPlayerSprite(Rectangle destination, bool moving, bool jumping, float deltaTime)
{
    Texture2D animation = { 0 };
    int state = 0;
    float framesPerSecond = PLAYER_WALK_FPS;

    if (jumping && IsTextureValid(playerJumpSprite)) {
        animation = playerJumpSprite;
        state = 2;
        framesPerSecond = PLAYER_JUMP_FPS;
    } else if (moving && IsTextureValid(playerWalkSprite)) {
        animation = playerWalkSprite;
        state = 1;
    }

    if (state == 0 || !IsTextureValid(animation)) {
        DrawSprite(playerSprite, destination, SKYBLUE);
        return;
    }

    if (playerAnimationState != state) {
        playerAnimationState = state;
        playerAnimationTime = 0.0f;
    }
    playerAnimationTime += deltaTime;

    int frameCount = animation.width / animation.height;
    if (frameCount < 1) frameCount = 1;
    int frame = (int)(playerAnimationTime * framesPerSecond) % frameCount;
    float frameWidth = (float)animation.height;
    Rectangle source = { frame * frameWidth, 0.0f, frameWidth,
                         (float)animation.height };
    DrawTexturePro(animation, source, destination, (Vector2){ 0.0f, 0.0f },
                   0.0f, WHITE);
}

void DrawBlockSprite(Rectangle destination)
{
    DrawSprite(blockSprite, destination, ORANGE);
}

void CloseGameSprites(void)
{
    if (IsTextureValid(playerSprite)) UnloadTexture(playerSprite);
    if (IsTextureValid(playerWalkSprite)) UnloadTexture(playerWalkSprite);
    if (IsTextureValid(playerJumpSprite)) UnloadTexture(playerJumpSprite);
    if (IsTextureValid(blockSprite)) UnloadTexture(blockSprite);
    if (IsTextureValid(backgroundSprite)) UnloadTexture(backgroundSprite);
}
