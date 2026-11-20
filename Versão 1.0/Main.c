#include "raylib.h"
#include <time.h>
#include "soms.h"
#include "sprites.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_BLOCKS 16
#define GRID_COLS 20
#define GRID_ROWS 25
#define TILE_WIDTH (SCREEN_WIDTH / GRID_COLS)
#define TILE_HEIGHT (SCREEN_HEIGHT / GRID_ROWS)

typedef struct {
    Rectangle rect;
    float speed;
    bool active;
} FallingBlock;

typedef enum {
    SCREEN_TITLE,
    SCREEN_INSTRUCTIONS,
    SCREEN_GAME
} GameScreen;

static void ResetGame(Rectangle *player, FallingBlock blocks[MAX_BLOCKS],
                      bool settled[GRID_ROWS][GRID_COLS], int *score,
                      float *elapsed, float *spawnTimer, bool *gameOver,
                      float *playerVelocityY, bool *playerOnGround)
{
    *player = (Rectangle){ SCREEN_WIDTH / 2.0f - 25.0f,
                           SCREEN_HEIGHT - 70.0f, 50.0f, 30.0f };
    for (int i = 0; i < MAX_BLOCKS; i++) {
        blocks[i].active = false;
    }
    for (int row = 0; row < GRID_ROWS; row++) {
        for (int col = 0; col < GRID_COLS; col++) {
            settled[row][col] = false;
        }
    }
    *score = 0;
    *elapsed = 0.0f;
    *spawnTimer = 0.0f;
    *gameOver = false;
    *playerVelocityY = 0.0f;
    *playerOnGround = true;
}

static void ClearCompletedRows(bool settled[GRID_ROWS][GRID_COLS], int *score)
{
    for (int row = GRID_ROWS - 1; row >= 0; row--) {
        bool full = true;
        for (int col = 0; col < GRID_COLS; col++) {
            if (!settled[row][col]) {
                full = false;
                break;
            }
        }

        if (full) {
            *score += 100;
            for (int moveRow = row; moveRow > 0; moveRow--) {
                for (int col = 0; col < GRID_COLS; col++) {
                    settled[moveRow][col] = settled[moveRow - 1][col];
                }
            }
            for (int col = 0; col < GRID_COLS; col++) settled[0][col] = false;
            row++;
        }
    }
}

static void SpawnBlock(FallingBlock blocks[MAX_BLOCKS], float elapsed)
{
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (!blocks[i].active) {
            int col = GetRandomValue(0, GRID_COLS - 1);
            blocks[i].rect = (Rectangle){
                (float)(col * TILE_WIDTH),
                (float)-TILE_HEIGHT,
                (float)TILE_WIDTH,
                (float)TILE_HEIGHT
            };
            blocks[i].speed = 180.0f + elapsed * 4.0f + (float)GetRandomValue(0, 80);
            blocks[i].active = true;
            return;
        }
    }
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Dodge the Blocks");
    InitGameAudio();
    InitGameSprites();
    SetTargetFPS(60);
    SetRandomSeed((unsigned int)time(NULL));

    Rectangle player;
    FallingBlock blocks[MAX_BLOCKS] = { 0 };
    bool settled[GRID_ROWS][GRID_COLS] = { false };
    int score;
    float elapsed;
    float spawnTimer;
    bool gameOver;
    float playerVelocityY;
    bool playerOnGround;
    ResetGame(&player, blocks, settled, &score, &elapsed, &spawnTimer, &gameOver,
              &playerVelocityY, &playerOnGround);
    GameScreen screen = SCREEN_TITLE;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        bool playerMoving = false;
        UpdateGameMusic();

        if (screen == SCREEN_TITLE) {
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                screen = SCREEN_INSTRUCTIONS;
            }
        } else if (screen == SCREEN_INSTRUCTIONS) {
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                ResetGame(&player, blocks, settled, &score, &elapsed, &spawnTimer,
                          &gameOver, &playerVelocityY, &playerOnGround);
                screen = SCREEN_GAME;
            }
        } else if (gameOver) {
            if (IsKeyPressed(KEY_R)) {
                ResetGame(&player, blocks, settled, &score, &elapsed, &spawnTimer, &gameOver,
                          &playerVelocityY, &playerOnGround);
            }
        } else {
            elapsed += deltaTime;

            float move = 0.0f;
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) move -= 1.0f;
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) move += 1.0f;
            playerMoving = move != 0.0f;
            player.x += move * 360.0f * deltaTime;
            if (player.x < 0.0f) player.x = 0.0f;
            if (player.x + player.width > SCREEN_WIDTH) {
                player.x = SCREEN_WIDTH - player.width;
            }

            if (playerOnGround && (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP))) {
                playerVelocityY = -520.0f;
                playerOnGround = false;
                PlayPlayerJumpSound();
            }

            playerVelocityY += 1400.0f * deltaTime;
            player.y += playerVelocityY * deltaTime;
            float groundY = SCREEN_HEIGHT - 40.0f - player.height;
            if (player.y >= groundY) {
                player.y = groundY;
                if (!playerOnGround && playerVelocityY > 0.0f) {
                    PlayPlayerLandingSound();
                }
                playerVelocityY = 0.0f;
                playerOnGround = true;
            }

            spawnTimer += deltaTime;
            float spawnInterval = 0.8f - elapsed * 0.008f;
            if (spawnInterval < 0.22f) spawnInterval = 0.22f;
            if (spawnTimer >= spawnInterval) {
                SpawnBlock(blocks, elapsed);
                spawnTimer = 0.0f;
            }

            for (int i = 0; i < MAX_BLOCKS; i++) {
                if (!blocks[i].active) continue;

                blocks[i].rect.y += blocks[i].speed * deltaTime;
                if (CheckCollisionRecs(player, blocks[i].rect)) {
                    gameOver = true;
                }

                int col = (int)(blocks[i].rect.x / TILE_WIDTH);
                int landingRow = GRID_ROWS;
                for (int row = 0; row < GRID_ROWS; row++) {
                    if (settled[row][col]) {
                        landingRow = row;
                        break;
                    }
                }
                float landingY = (float)(landingRow * TILE_HEIGHT) - blocks[i].rect.height;
                if (blocks[i].rect.y >= landingY) {
                    blocks[i].rect.y = landingY;
                    int row = landingRow - 1;
                    if (row < 0) {
                        gameOver = true;
                    } else {
                        settled[row][col] = true;
                        score += 10;
                        blocks[i].active = false;
                        ClearCompletedRows(settled, &score);
                    }
                }
            }

            for (int row = 0; row < GRID_ROWS; row++) {
                for (int col = 0; col < GRID_COLS; col++) {
                    if (settled[row][col]) {
                        Rectangle cell = { (float)(col * TILE_WIDTH),
                                           (float)(row * TILE_HEIGHT),
                                           (float)TILE_WIDTH, (float)TILE_HEIGHT };
                        if (CheckCollisionRecs(player, cell)) gameOver = true;
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground((Color){ 18, 22, 35, 255 });
        DrawBackgroundSprite((Rectangle){ 0.0f, 0.0f,
                                          (float)SCREEN_WIDTH,
                                          (float)SCREEN_HEIGHT });

        if (screen == SCREEN_TITLE) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){ 0, 0, 0, 135 });
            DrawText("DESVIE DOS BLOCOS", SCREEN_WIDTH / 2 - 225, 190, 48, RAYWHITE);
            DrawText("Desvie dos blocos e sobreviva!", SCREEN_WIDTH / 2 - 190,
                     270, 22, LIGHTGRAY);
            DrawText("Aperte ENTER", SCREEN_WIDTH / 2 - 125, 360,
                     22, YELLOW);
        } else if (screen == SCREEN_INSTRUCTIONS) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){ 0, 0, 0, 155 });
            DrawText("Como jogar", SCREEN_WIDTH / 2 - 145, 135, 40, RAYWHITE);
            DrawText("Mover: Esquerda / Direita setas ou A / D", SCREEN_WIDTH / 2 - 205,
                     220, 22, LIGHTGRAY);
            DrawText("Pular: barra de espaço ou seta para cima", SCREEN_WIDTH / 2 - 155,
                     265, 22, LIGHTGRAY);
            DrawText("Desvie dos blocos caindo. Os blocos se empilham!",
                     SCREEN_WIDTH / 2 - 245, 310, 20, LIGHTGRAY);
            DrawText("Aperte ENTER para começar", SCREEN_WIDTH / 2 - 190,
                     390, 22, YELLOW);
        } else {
        DrawText("DESVIE DOS BLOCOS", 20, 18, 28, RAYWHITE);
        DrawText(TextFormat("Tempo: %.1f", elapsed), 20, 55, 20, LIGHTGRAY);
        DrawText(TextFormat("Pontos: %d", score), 20, 82, 20, YELLOW);

        for (int row = 0; row < GRID_ROWS; row++) {
            for (int col = 0; col < GRID_COLS; col++) {
                if (settled[row][col]) {
                    Rectangle cell = { (float)(col * TILE_WIDTH),
                                       (float)(row * TILE_HEIGHT),
                                       (float)TILE_WIDTH, (float)TILE_HEIGHT };
                    DrawBlockSprite(cell);
                }
            }
        }

        for (int i = 0; i < MAX_BLOCKS; i++) {
            if (blocks[i].active) {
                DrawBlockSprite(blocks[i].rect);
            }
        }
        DrawPlayerSprite(player, playerMoving, !playerOnGround, deltaTime);

        if (gameOver) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){ 0, 0, 0, 170 });
            DrawText("FIM DE JOGO", SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 - 50,
                     40, RAYWHITE);
            DrawText(TextFormat("Voce sobreviveu %.1f segundos", elapsed),
                     SCREEN_WIDTH / 2 - 145, SCREEN_HEIGHT / 2 + 5, 20, LIGHTGRAY);
            DrawText("Aperte R para jogar de novo", SCREEN_WIDTH / 2 - 120,
                     SCREEN_HEIGHT / 2 + 45, 20, YELLOW);
        }

        }

        EndDrawing();
    }

    CloseGameSprites();
    CloseGameAudio();
    CloseWindow();
    return 0;
}
