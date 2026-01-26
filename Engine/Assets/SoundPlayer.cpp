#include "SoundPlayer.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "Asset.h"



SoundPlayer::SoundPlayer()
{
    ma_engine_init(NULL, &Engine);
}

SoundPlayer::~SoundPlayer()
{
    ma_engine_uninit(&Engine);
}

void SoundPlayer::PlaySoundFromFile(std::filesystem::path Path)
{
    if (!IsExistingPath(&Path))
    {
        return;
    }
    ma_engine_play_sound(&Engine, Path.string().c_str(), NULL);
}
