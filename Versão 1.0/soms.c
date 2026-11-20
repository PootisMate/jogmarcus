#include "raylib.h"
#include "soms.h"

/* Colocar os audios depois aqui*/
#define JUMP_SOUND_PATH "assets/audio/player_jump.wav"
#define PLAYER_LAND_SOUND_PATH "assets/audio/player_land.wav"
#define BLOCK_LAND_SOUND_PATH "assets/audio/block_land.wav"
#define MUSIC_PATH "assets/audio/background_music.ogg"


static Sound jumpSound;
static Sound playerLandSound;
static Sound blockLandSound;
static Music backgroundMusic;
static bool audioReady;


void InitGameAudio(void)
{
    InitAudioDevice();
    audioReady = IsAudioDeviceReady();
    if (!audioReady) return;

    
    jumpSound = LoadSound(JUMP_SOUND_PATH);
    playerLandSound = LoadSound(PLAYER_LAND_SOUND_PATH);
    blockLandSound = LoadSound(BLOCK_LAND_SOUND_PATH);
    backgroundMusic = LoadMusicStream(MUSIC_PATH);

    if (IsMusicValid(backgroundMusic)) {
        PlayMusicStream(backgroundMusic);
    }
}

void UpdateGameMusic(void)
{
    if (audioReady && IsMusicValid(backgroundMusic)) {
        UpdateMusicStream(backgroundMusic);
    }
}



void PlayPlayerJumpSound(void)
{
    if (audioReady && IsSoundValid(jumpSound)) PlaySound(jumpSound);
}

void PlayPlayerLandingSound(void)
{
    if (audioReady && IsSoundValid(playerLandSound)) PlaySound(playerLandSound);
}

void PlayBlockLandingSound(void)
{
    if (audioReady && IsSoundValid(blockLandSound)) PlaySound(blockLandSound);
}

void CloseGameAudio(void)
{
    if (!audioReady) return;

    
    if (IsSoundValid(jumpSound)) UnloadSound(jumpSound);
    if (IsSoundValid(playerLandSound)) UnloadSound(playerLandSound);
    if (IsSoundValid(blockLandSound)) UnloadSound(blockLandSound);
    if (IsMusicValid(backgroundMusic)) {
        StopMusicStream(backgroundMusic);
        UnloadMusicStream(backgroundMusic);
    }

    CloseAudioDevice();
    audioReady = false;
}
