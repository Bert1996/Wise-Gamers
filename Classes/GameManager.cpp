#include "GameManager.h"

GameManager* GameManager::instance = nullptr;

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

GameManager* GameManager::sharedGameManager()
{
	if (instance == nullptr)
	{
		instance = new GameManager();
	}

	return instance;
}

void GameManager::AddToScore(int increment)
{
	this->_score += increment;
}

void GameManager::SetScore(int value)
{
	this->_score = value;
}

void GameManager::ResetScore()
{
	_score = 0;
}

int GameManager::GetScore()
{
	return _score;
}