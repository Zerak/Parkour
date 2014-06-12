#ifndef __COIN_H__
#define __COIN_H__

#include "cocos2d.h"
#include "chipmunk.h"
#include "cocos-ext.h"
#include "Utils.h"

USING_NS_CC;
USING_NS_CC_EXT;

class Coin
{
public:
	cpSpace *space;
	PhysicsSprite *sprite;
	cpShape  *shape;// current chipmunk shape
	//��������ĸ���ͼ,���ֵ��Ҫ��ObjectManager.cpp�н������á�
	int _map;// which map belong to
	static const CCSize *gCoinContentSize;

public:
	Coin();

	Coin(CCSpriteBatchNode *spriteSheet, cpSpace *space, CCPoint pos);

	~Coin();

	int getMap();
	void setMap(int newMap);

	void removeFromParent();

	cpShape *getShape();

	//CCSize getContentSize();
	// static method for Class Coin.
	// getContentSize�ǽ�����е�һ����̬����,����ObjectManager.cpp��������������
	static  const CCSize* getContentSize()
	{
		//if ((gCoinContentSize->width == 0) && (gCoinContentSize.height == 0))
		if (NULL == gCoinContentSize)
		{
			PhysicsSprite *sprite = PhysicsSprite::createWithSpriteFrameName("coin0.png");        
			gCoinContentSize = &sprite->getContentSize();
		}

		return gCoinContentSize;
	}
};

//static const CCSize *gCoinContentSize = NULL;

#endif // __COIN_H__
