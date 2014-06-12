#include "GameOverLayer.h"
#include "resource.h"
#include "PlayScene.h"

USING_NS_CC;

GameOverLayer::GameOverLayer()
{
	this->init();
}

GameOverLayer::~GameOverLayer()
{

}

// on "init" you need to initialize your instance
bool GameOverLayer::init()
{
	//CCLayerColor *background = CCLayerColor::create(ccc4(0, 0, 0, 180));
	//this->addChild(background);

	//��ȡ��Ļ��С
	CCSize size=CCDirector::sharedDirector()->getWinSize();
	CCPoint centerPos = ccp(size.width/2, size.height/2);

	CCSprite *spriteBG = CCSprite::create(s_HelloBG);
	spriteBG->setPosition(centerPos);
	this->addChild(spriteBG);

	//���������С  
	CCMenuItemFont::setFontSize( 30 );  
	//����������������  
	CCMenuItemFont::setFontName("Courier New");  
	//�����ܵ��  
	setTouchEnabled(true);  

	// ����һ��CCMenuItemSprite   
	CCMenuItemSprite *menuItemRestart = CCMenuItemSprite::create(
		CCSprite::create(s_restart_n),
		CCSprite::create(s_restart_s),
		CC_CALLBACK_1(GameOverLayer::onRestart,this) );

	// create menu, it's an autorelease object
	CCMenu *menu = CCMenu::create(menuItemRestart, NULL);
	menu->setPosition(centerPos);
	this->addChild(menu);

	return true;
}
void GameOverLayer::onRestart(CCObject* pSender)
{
	CCLOG("Restart a new game!!!");
	CCScene *playScene = PlayScene::scene();

	//����PlayScene
	CCDirector::sharedDirector()->replaceScene(playScene);
}
