#include "ObjectManager.h"
#include "resource.h"
#include "Runner.h"
#include "Coin.h"
#include "Rock.h"


ObjectManager::ObjectManager()
{

}

ObjectManager::ObjectManager(CCSpriteBatchNode *spriteSheet, cpSpace *space)
{
	this->spriteSheet = NULL;
	this->space = NULL;
	this->spriteSheet = spriteSheet;
	this->space = space;
	// objects will keep when new ObjectManager();
	// we need clean here
	m_vecCoinsList.clear();
	m_vecRocksList.clear();
}

ObjectManager::~ObjectManager()
{
	
}

// ��ͼ��ʼ���������Ҫ�߼�
void ObjectManager::initObjectOfMap(int map, float mapWidth)
{
	int initCoinNum = 7;
	float jumpRockHeight = Runner::getCrouchContentSize()->height + g_groundHight;
	float coinHeight = Coin::getContentSize()->height + g_groundHight;

	//���������������ȷ���ĸ��㴴����һ���ʯ
	//random the center point of 7 coins.
	//int randomCoinFactor = Math.round(Math.random()*2+1);
	int randomCoinFactor = CCRANDOM_0_1()*(CCRANDOM_0_1()*2 + 1);
	//int randomRockFactor = Math.round(Math.random()*2+1);
	int randomRockFactor = CCRANDOM_0_1()*(CCRANDOM_0_1()*2 + 1);
	float jumpRockFactor = 0;

	//ͨ���������������ÿ�ŵ�ͼ�н�Һ���ʯ�� ��ʼλ��,��ÿ��������ӵ���ͼ��
	float coinPoint_x = mapWidth/4 * randomCoinFactor+mapWidth*map;
	float RockPoint_x = mapWidth/4 * randomRockFactor+mapWidth*map;

	float coinWidth = Coin::getContentSize()->width;
	float rockWith = Rock::getContentSize()->width;
	float rockHeight =  Rock::getContentSize()->height;

	float startx = coinPoint_x - coinWidth/2*11;
	float xIncrement = coinWidth/2*3;

	//add a rock
	Rock *rock = new Rock(spriteSheet, space,
		ccp(RockPoint_x, g_groundHight+rockHeight/2));
	if ((NULL != rock) && (NULL != rock->shape))
	{
		rock->setMap(map);
		m_vecRocksList.push_back(rock);
	}
	
	if(map == 0 && randomCoinFactor==1){
		randomCoinFactor = 2;
	}

	//add coins
	for(int i = 0; i < initCoinNum; i++)
	{
		//�ý�����ٸ�����,�����ҵĿ�ʼλ�ø���ʯ��һ����ô��Ҫ�����ĵ����
		//��ʯͷ�߶ȸ߻��ߵ���ʯͷ�ĵײ�
		if((startx + i*xIncrement > RockPoint_x-rockWith/2)
			&&(startx + i*xIncrement < RockPoint_x+rockWith/2))
		{
			Coin *coin = new Coin(spriteSheet, space,
				ccp(startx + i*xIncrement, coinHeight+rockHeight));
			coin->setMap(map);
			m_vecCoinsList.push_back(coin);
		} 
		else
		{
			Coin *coin = new Coin(spriteSheet, space,
				ccp(startx + i*xIncrement, coinHeight));
			if ((NULL != coin) && (NULL != coin->shape))
			{
				coin->setMap(map);
				m_vecCoinsList.push_back(coin);
			}
		}
	}

	for(int i=1;i<4;i++){
		if((i!=randomCoinFactor) && (i!=randomRockFactor))
		{
			jumpRockFactor = i;
		}
	}

	//add jump rock
	float JumpRockPoint_x = mapWidth/4 * jumpRockFactor + mapWidth*map;
	Rock *jumpRock = new Rock(spriteSheet, space,
		ccp(JumpRockPoint_x, jumpRockHeight+rockHeight/2));

	if ((NULL != jumpRock) && (NULL != jumpRock->shape))
	{
		jumpRock->setMap(map);
		m_vecRocksList.push_back(jumpRock);
	}
}

#if 1
/** remove all objects of the map
    * @param {int}
    */
//ÿһ�ε�ͼ����,��ͼ�еĶ���Ҫ����
bool ObjectManager::recycleObjectOfMap(int map)
{
	//������
	std::list<Coin*>::iterator it1 = m_vecCoinsList.begin();
	std::list<Coin*>::iterator end1 = m_vecCoinsList.end();
#if 0
	while (it1 != end1)
	{
		Coin *coin = *it1++;
		if (coin->_map == map) 
		{
			coin->removeFromParent();
			delete coin;
			break;
		}
	}
#else
	for(;it1!=end1;++it1)
	{
		if((*it1)->_map == map)
		{
			(*it1)->removeFromParent();
			m_vecCoinsList.erase(it1); 
			//delete *it1;
			break;
			//end1 = m_vecCoinsList.end();
		}
	}
#endif

	//���ʯͷ
	std::list<Rock*>::iterator it2 = m_vecRocksList.begin();
	std::list<Rock*>::iterator end2 = m_vecRocksList.end();
#if 0
	while (it2 != end2)
	{
		Rock *rock = *it2++;
		if (rock->_map == map) 
		{
			rock->removeFromParent();
			delete rock;
			break;
		}
	}
#else
	for(;it2!=end2;++it2)
	{
		if((*it2)->_map == map)
		{
			(*it2)->removeFromParent();
			m_vecRocksList.erase(it2); 
			//delete *it2;
			break;
			//end1 = m_vecCoinsList.end();
		}
	}
#endif

	return true;
}
#else
/** remove all objects of the map
    * @param {int}
    */
//ÿһ�ε�ͼ����,��ͼ�еĶ���Ҫ����
bool ObjectManager::recycleObjectOfMap(int map)
{
	//������
	for (unsigned int i = 0; i < m_vecCoinsList.size(); i++) {
		if (obj[i].map == map) {
			obj[i].removeFromParent();
			obj.splice(i, 1);
			goto leave;
		}
	}

leave:
	//���ʯͷ
	for (var i = 0; i < obj.length; i++) {
		if (obj[i].map == map) {
			obj[i].removeFromParent();
			obj.splice(i, 1);
			return true;
		}
	}

	return true;
}
#endif
/** remove specified object
    * @param {Coin *} Coin
    */
//����Ϸ��ɫ�õ����ʱ,�������Ҵ����ĸ����к��б����Ƴ�
void ObjectManager::removeCoin(Coin *obj)
{
	if(obj){
		obj->removeFromParent();
		std::list<Coin*>::iterator it= m_vecCoinsList.begin();
		for(;it!=m_vecCoinsList.end();++it){
			if(*it == obj){
				m_vecCoinsList.erase(it);
				delete (obj);
				//m_vecCoinsList.splice()
				break;
			}
		}
	}
}

/** remove specified object
    * @param {Rock *}Rock
    */
void ObjectManager::removeRock(Rock *obj)
{
	if(obj){
		obj->removeFromParent();
		std::list<Rock*>::iterator it= m_vecRocksList.begin();
		for(;it!=m_vecRocksList.end();++it){
			if(*it == obj){
				m_vecRocksList.erase(it);

				//obj->removeFromParent();

				delete (obj);
				return;
			}
		}
	}
}

Coin * ObjectManager::getCoinByShape(cpShape *shape)
{
	std::list<Coin*>::iterator it1 = m_vecCoinsList.begin();
	std::list<Coin*>::iterator end1 = m_vecCoinsList.end();
	for(;it1!=end1;++it1)
	{
		if((*it1)->getShape() == shape)
		{
			return *it1;
		}
	}
	return NULL;
}

Rock * ObjectManager::getRockByShape(cpShape *shape)
{
	std::list<Rock*>::iterator it2 = m_vecRocksList.begin();
	std::list<Rock*>::iterator end2 = m_vecRocksList.end();
	for(;it2!=end2;++it2)
	{
		if((*it2)->getShape() == shape)
		{
			return *it2;
		}
	}
	return NULL;
}
