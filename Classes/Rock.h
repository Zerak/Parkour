#ifndef __ROCK_H__
#define __ROCK_H__

#include "cocos2d.h"
#include "chipmunk.h"
#include "cocos-ext.h"
#include "Utils.h"

USING_NS_CC;
USING_NS_CC_EXT;

class Rock
{
public:
	Rock();
	Rock(CCSpriteBatchNode *spriteSheet, cpSpace *space, CCPoint pos);
	~Rock();

	int getMap();
	void setMap(int newMap);

	void removeFromParent();

	cpShape *getShape();

	// getContentSize��ʯͷ���е�һ����̬����,����ObjectManager.cpp��������������
	static const CCSize* getContentSize()
	{
		if (NULL == gRockContentSize)
		{
			PhysicsSprite *sprite = PhysicsSprite::createWithSpriteFrameName("rock.png");        
			gRockContentSize = &sprite->getContentSize();
		}

		return gRockContentSize;
	}

public:
	cpSpace *space;
	PhysicsSprite *sprite;
	cpShape  *shape;// current chipmunk shape
	//ʯͷ�����ĸ���ͼ,���ֵ��Ҫ��ObjectManager.cpp�н������á�
	int _map;// which map belong to
	static const CCSize *gRockContentSize;
};
//static const CCSize *gRockContentSize = NULL;

#endif // __ROCK_H__
