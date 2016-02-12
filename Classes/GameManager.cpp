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

void GameManager::ResetScore()
{
	_score = 0;
}

int GameManager::GetScore()
{
	return _score;
}