#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "SoundPlayer.h"


SoundPlayer::SoundPlayer()
{
    ma_engine_init(NULL, &engine);
}

SoundPlayer::~SoundPlayer()
{
    ma_engine_uninit(&engine);
}

void SoundPlayer::PlaySoundFromFile(std::filesystem::path Path)
{
    if (!IsExistingPath(&Path))
    {
        return;
    }
    ma_engine_play_sound(&engine, Path.string().c_str(), NULL);
}
