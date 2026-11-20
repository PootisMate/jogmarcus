#ifndef SPRITES_H
#define SPRITES_H

#include "raylib.h"

void InitGameSprites(void);
void DrawBackgroundSprite(Rectangle destination);
void DrawPlayerSprite(Rectangle destination);
void DrawBlockSprite(Rectangle destination);
void CloseGameSprites(void);

#endif
