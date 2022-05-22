#pragma once
#include "raylib.h"
class AudioManager
{	
	Sound humanitySound;

	const int feedSoundsAmount = 4;
	Sound feedSounds[4];

public:
	void Init()
	{
		InitAudioDevice();
		humanitySound = LoadSound("resources//Audio//Humanity.wav");
		
		int i;
		for (i = 0; i < feedSoundsAmount; i++)
		{
			char fileName[256];
			snprintf(fileName, 256, "resources//Audio//hurt%d.wav", (i + 1));
			feedSounds[i] = LoadSound(fileName);
		}
	}

	void PlayHumanity()
	{
		PlaySound(humanitySound);
	}

	void PlayFeedSound()
	{
		int index = GetRandomValue(0, feedSoundsAmount - 1);
		PlaySound(feedSounds[index]);
	}
};