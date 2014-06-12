#include "Map.h"
#include "resource.h"
#include "Utils.h"

namespace Parkour{

Map::~Map()
{

}

Map::Map()
{

}

/** Constructor
    * @param {cc.Layer *}
    * @param {cp.Space *}
    */

Map::Map(CCLayer *layer, cpSpace *space)
{
	this->layer = NULL;
	this->space = NULL;
	//MapCountӦ��������Դ�ļ����е��ļ���������Ӧ����������
	this->mapCount = 2;
	this->spriteWidth = 0;
	this->map0 = NULL;
	this->map1 = NULL;
	this->ground0 = NULL;
	this->ground1 = NULL;
	this->curMap = 0;

	this->layer = layer;
	this->space = space;

	// init bg map for the first time
	//��ͼ���ϰ벿����һ��ê���Ϊ(0,0)�ľ���,�ø���ê����������ļ���
	map0 = CCSprite::create("res/Map00.png");
	map0->setAnchorPoint(ccp(0, 0));
	map0->setPosition(ccp(0, 0));
	layer->addChild(map0);

	//�ϲ㲿�ָ��²㲿�ֵĲ�֮ͬ�������ǵ�λ������,���²㲿��Y����������Ϊ 
	//g_groundHight �C ground0->getContentSize().height��ȷ����Ϸ��ɫ�Ľ���̤�ڵ����ϵġ�
	ground0 = CCSprite::create("res/Ground00.png");
	ground0->setAnchorPoint(ccp(0, 0));
	CCSize size = ground0->getContentSize();
	ground0->setPosition(ccp(0, g_groundHight - size.height));
	layer->addChild(ground0);

	spriteWidth = map0->getContentSize().width;

	map1 = CCSprite::create("res/Map01.png");
	map1->setAnchorPoint(ccp(0, 0));
	// �ڶ��ŵ�ͼ�Ŀ�ʼλ���Ǳ����Ŀ��
	map1->setPosition(ccp(spriteWidth, 0));
	layer->addChild(map1);

	ground1 = CCSprite::create("res/Ground01.png");
	ground1->setAnchorPoint(ccp(0, 0));
	ground1->setPosition(ccp(spriteWidth, g_groundHight - size.height));
	layer->addChild(ground1);
}

/**
    * @return width of the map sprite.
    */ 
float Map::getMapWidth()
{
	return spriteWidth;
}

/**
    * @return which map of the runner current run on.
    */ 
int Map::getCurMap() {
    return this->curMap;
}

/**
    * @param {float} X-coordinate of the eye of camera of PlayLayer
    */
bool Map::checkAndReload(float eyeX) {
	//�����ַ�ʽ�����ͼ����
	int newCur = int(eyeX / spriteWidth);

    if (curMap == newCur) {
        return false;
    }

    CCSprite * map;
    CCSprite * ground;

    if (0 == newCur % 2) {
        // change mapSecond
        map = this->map1;
        ground = this->ground1;
    } else {
        // change mapFirst
        map = this->map0;
        ground = this->ground0;
    }

    CCLOG("==load map:%d", (newCur + 1));

    this->curMap = newCur;

	//��һ�ű�������֮����Ҫ�л�һ���µ�ͼƬ
    // load curMap + 1
    //var fileName = "res/Map" + FormatNumberLength((newCur + 1) % mapCount, 2) + ".png";

	//char fileMapName[100] = {0};
	//sprintf(fileName, "res/Map%i.png", i);
	//fileMapName = "res/Map" + FormatNumberLength((newCur + 1) % mapCount, 2) + ".png";
	//CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(fileMapName);

	//��ʱ��,����չ
	std::string fileMapName;
	if ((newCur + 1) % mapCount)
	{
		fileMapName = "res/Map00.png";
	} 
	else
	{
		fileMapName = "res/Map01.png";
	}

	CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(fileMapName.c_str());

    map->setTexture(texture);
    map->setPositionX(this->spriteWidth * (newCur + 1));

    // load ground
#if 0
	var fileGroundName = "res/Ground" + FormatNumberLength((newCur + 1) % mapCount, 2) + ".png";
	CCTexture2D *textureGround = CCTextureCache::sharedTextureCache()->addImage(fileGroundName);
	ground->setTexture(textureGround);
	ground->setPositionX(this->spriteWidth * (newCur + 1));

#else
	std::string fileGroundName;
	if ((newCur + 1) % mapCount)
	{
		fileGroundName = "res/Ground00.png";
	} 
	else
	{
		fileGroundName = "res/Ground01.png";
	}
	CCTexture2D *textureGround = CCTextureCache::sharedTextureCache()->addImage(fileGroundName.c_str());

	ground->setTexture(textureGround);
	ground->setPositionX(this->spriteWidth * (newCur + 1));
#endif

    return true;
}
}