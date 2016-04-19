#ifndef GameManager_h
#define GameManager_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

#include "stdio.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace cocos2d;
using namespace std;

class GameManager
{
public:
	~GameManager();

	static GameManager* sharedGameManager();

	//Score Methods
	void AddToScore(int increment);
	void SetScore(int value);
	void ResetScore();
	int GetScore();

private:
	GameManager();
	static GameManager* instance;

	int _score;
};
#endif //GameManager_h