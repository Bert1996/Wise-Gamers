#pragma once
#include "SimpleAudioEngine.h"

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	static void PlayBackGroundMusic();
	static void StopBackGroundMusic();

	static void PlayMainMenuMusic();
	static void StopMainMenuMusic();

	static void PlayButtonSound();
	static void PlayRobotSound();
	static void PlayErrorSound();
};

