#pragma once
#include "Asset.h"

#include "miniaudio.h"

class SoundPlayer
{
public:
	SoundPlayer();
	~SoundPlayer();

    void PlaySoundFromFile(std::filesystem::path Path);

private:
    ma_engine engine;
};
