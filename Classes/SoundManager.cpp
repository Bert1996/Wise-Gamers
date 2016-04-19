#include "SoundManager.h"

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
}

void SoundManager::PlayBackGroundMusic()
{
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Sounds/MainMenuSound.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Sounds/MainMenuSound.mp3", true);
}

void SoundManager::StopBackGroundMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void SoundManager::PlayMainMenuMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Sounds/GamePlaySound.wav", true);
}

void SoundManager::StopMainMenuMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void SoundManager::PlayButtonSound()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sounds/ButtonPressedSutton.wav");
}

void SoundManager::PlayRobotSound()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sounds/RobotSound.wav");
}

void SoundManager::PlayErrorSound()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sounds/ErrorSound.wav");
}
