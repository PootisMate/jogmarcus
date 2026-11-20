#ifndef SOMS_H
#define SOMS_H

#include <stdbool.h>

void InitGameAudio(void);
void UpdateGameMusic(void);
void PlayPlayerJumpSound(void);
void PlayPlayerLandingSound(void);
void PlayBlockLandingSound(void);
void CloseGameAudio(void);

#endif
