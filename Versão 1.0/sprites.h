#ifndef SPRITES_H
#define SPRITES_H

#include "raylib.h"

void InitGameSprites(void);
void DrawBackgroundSprite(Rectangle destination);
void DrawPlayerSprite(Rectangle destination, bool moving, bool jumping, float deltaTime);
void DrawBlockSprite(Rectangle destination);
void CloseGameSprites(void);

#endif
