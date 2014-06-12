#include "PlayScene.h"
#include "resource.h"
#include "Utils.h"
#include "Runner.h"
#include "SimpleRecognizer.h"
#include "GeometricRecognizer.h"
#include "Map.h"
#include "BackgroundLayer.h"
#include "StatusLayer.h"
#include "GameOverLayer.h"

#include "CCActionCamera.h"
USING_NS_CC;

using namespace DollarRecognizer;
using namespace Parkour;

Runner *PlayScene::runner = NULL;
GameStat PlayScene::gameStat = GameStatNormal;
std::vector<cpShape> PlayScene::shapesToRemove;
StatusLayer *PlayScene::statusLayer = NULL;

PlayScene::PlayScene()
{
	//class member variable
	map = NULL;
	lastEyeX = 0;
	runner = NULL;
	space = NULL;// chipmunk space
	recognizer = NULL;
	//dollar = NULL;
	objectManager = NULL;
	//shapesToRemove.clear();
	gameStat = GameStatNormal;
	spriteSheet = NULL;
	this->init();
}

PlayScene::~PlayScene()
{
	//free the pSpace
	// manually Free rogue shapes
	//cpShapeFree(wallBottom);

	cpSpaceFree(space);
}

CCScene* PlayScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	//PlayScene *layer = PlayScene::create();
	PlayScene *layer = new PlayScene();
	// add layer as a child to scene
	scene->addChild(layer);

	StatusLayer *status = new StatusLayer();
	scene->addChild(status);

	// return the scene
	return scene;
}

bool PlayScene::init()
{
	// objects will keep when new ObjectManager();
	// we need clean here
	shapesToRemove.clear();

	this->setTag(0);
	this->initPhysics();

	// enable touch
	this->setTouchEnabled(true);
	// set touch mode to kCCTouchesOneByOne
	this->setTouchMode(kCCTouchesOneByOne);

	//this.dollar = new DollarRecognizer();

	//�Լ���չ�ļ�����ʶ��
	recognizer = new SimpleRecognizer();

	//����ģ��Ȼ���¼��������(������ֻ����������Ƶ�·��)
	geometricRecognizer = new GeometricRecognizer;
	geometricRecognizer->loadTemplates();

	//��ʼ����ͼ
	map = new Parkour::Map(this, space);
	//ʹ���µ�ͼ
	backgroundLayer = new BackgroundLayer();
	this->addChild(backgroundLayer);

	// create sprite sheet of PlayLayer
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(s_plist_parkour,s_img_parkour);
	spriteSheet = CCSpriteBatchNode::create(s_img_parkour);
	this->addChild(spriteSheet);

	//runner = Runner::create(spriteSheet, space);
	runner = new Runner(spriteSheet, space);
	// runner is base on Node, addChild to make scheduleOnce and onExit call.
	this->addChild(this->runner);

	objectManager = new ObjectManager(spriteSheet, space);
	objectManager->initObjectOfMap(1, map->getMapWidth());

	//audioEngine.playMusic(s_music_background, true);

	//statusLayer = new StatusLayer(this);

	//this->statusLayer = new StatusLayer();
	//this->addChild(this->statusLayer);
	//this->statusLayer = (StatusLayer*)this->getParent()->getChildByTag(1);

	//create main loop
	this->schedule(schedule_selector(PlayScene::update));
	
	return true;
}

bool PlayScene::onTouchBegan(CCTouch* touch, CCEvent* event)
{
	CCLOG("PlayScene::ccTouchBegan");
	CCPoint pos = touch->getLocation();
	recognizer->beginPoint(pos.x, pos.y);

	return true;
}

void PlayScene::onTouchMoved(CCTouch* touch, CCEvent* event)
{
	//CCLOG("PlayScene::ccTouchMoved");

#if 1
	CCPoint pos = touch->getLocation();
	recognizer->movePoint(pos.x, pos.y);
#else
	//Layer��ccTouchesMoved�����м�¼����
	//CCSetIterator it = pTouches->begin();
	//CCTouch* touch = (CCTouch*)(*it);

	CCPoint location = touch->getLocation();
	Point2D p_Point2DTemp;
	p_Point2DTemp.x = location.x;
	p_Point2DTemp.y = location.y;
	//��¼
	p_2dPath.push_back(p_Point2DTemp);
#endif
}

void PlayScene::onTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCLOG("PlayScene::ccTouchEnded");

#if 1
	SimpleGestures rtn = recognizer->endPoint();

	switch (rtn) {
	case SimpleGesturesUp:
		CCLOG("Runner::jump");
		runner->jump();
		break;

	case SimpleGesturesDown:
		CCLOG("Runner::crouch");
		runner->crouch();
		break;

	case SimpleGesturesNotSupport:
	case SimpleGesturesError:
		// try dollar Recognizer
		// 0:Use Golden Section Search (original) 
		// 1:Use Protractor (faster)
#if 0
		var result = dollar.Recognize(this.recognizer.getPoints(), 1);
		CCLOG(result.Name);
		if (result.Name == "circle") {
			runner->incredibleHulk();
		}
#endif
		CCLOG("not support or error touch,use geometricRecognizer!!");
		runner->incredibleHulk();
#if 0
		//ͨ��GeometricRecognizerУ׼
		//����ѡ��������ҵ�������
		if (p_2dPath.size() < 1){
			return ;
		}

		RecognitionResult r = geometricRecognizer->recognize(p_2dPath);
		if((r.name != "Unknown") && (r.score > 0.5))
		{
			runner->incredibleHulk();
			//return;
		}
#endif

#if 0
		CCLOG(r.name.c_str());

		if (r.name == "Circle") {
			runner->incredibleHulk();
		}
#endif
		break;
	}

#endif
}

void PlayScene::onTouchCancelled(CCTouch* touch, CCEvent* event)
{
	CCLOG("onTouchCancelled!!!");
}

int PlayScene::collisionCoinBegin(cpArbiter *arbiter, cpSpace *space, void *unused)
{
	CCLOG("collisionCoinBegin");
	CP_ARBITER_GET_SHAPES(arbiter, a, b);

	shapesToRemove.push_back(*a);

	//StatusLayer *statusLayer = (StatusLayer*)getParent()->getChildByTag(1);
	statusLayer->addCoin(1);

	//audioEngine.playEffect(s_music_pickup_coin);
	return 0;
}

int PlayScene::collisionRockBegin(cpArbiter *arbiter, cpSpace *space, void *unused) 
{
	CCLOG("collisionRockBegin");

	bool rtn = runner->meetRock();
	if (rtn == true) 
	{
		gameStat = GameStatOver;
		//GameOverLayer *gameover = new GameOverLayer();
		//this->getParent()->addChild(gameover);
		//PlayScene::addChild(gameover);

		//audioEngine.stopMusic();
		CCLOG("Game Over");
	} 
	else 
	{
		// break Rock
		//var shapes = arbiter.getShapes();
		//this.shapesToRemove.push(shapes[1]);
		CP_ARBITER_GET_SHAPES(arbiter, a, b);
		shapesToRemove.push_back(*a);
	}
	return 0;
}

void PlayScene:: onEnter()
{
	CCLayer::onEnter();
}

void PlayScene:: onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
}

void PlayScene:: onExit()
{
	CCLayer::onExit();
}

//ʹ��Chipmunk2D�������档Cocos2d-x��������Chipmunk JSB API��
//һ������������,��һ����������̵ġ�����ʹ�ø����Ѻõ��������ӿ�
void PlayScene::initPhysics()
{
	//��������chipmunk API����������һ����������
	this->space = cpSpaceNew();

	//�����������������
	this->space->gravity = cpv(0, -350);

	//set up Walls
	//�ܿ����õĵ��棬chipmunk��ʹ�þ�̬��״��������������ռ��½�һ����̬SegmentShape,Ȼ������ӵ�����ռ�
	//
	// rogue shapes
	// We have to free them manually
	//
	// bottom
	cpShape *wallBottom = cpSegmentShapeNew( this->space->staticBody,
		cpv(0,g_groundHight),// start point
		cpv(4294967295, g_groundHight), // MAX INT:4294967295
		0.0f);// thickness of wall

	cpSpaceAddStaticShape(this->space, wallBottom);

	//wallBottom->e = 1.0f;
	//wallBottom->u = 1.0f;
	// setup chipmunk CollisionHandler
	cpSpaceAddCollisionHandler(this->space,SpriteTagRunner, SpriteTagCoin, 
		PlayScene::collisionCoinBegin, NULL, NULL, NULL,NULL);

	cpSpaceAddCollisionHandler(space, SpriteTagRunner,SpriteTagRock, 
		PlayScene::collisionRockBegin, NULL, NULL, NULL,NULL);
}

// be called when the runner end it's incredible mode.
void PlayScene::cleanScreen()
{
	objectManager->recycleObjectOfMap(map->getCurMap());
}

void PlayScene::initStatusLayer()
{
	this->labelCoin = NULL;
	this->labelMeter = NULL;
	this->coins = 0;

	CCSize winSize=CCDirector::sharedDirector()->getWinSize();

	//CCLabelTTF::create����һ�����ֱ�ǩ����һ����������ʾ�����֣��ڶ��������������壨����չ������Щ�����ѡ�񣩣�
	//�������������������С
	this->labelCoin = CCLabelTTF::create("Coins:0", "Helvetica", 25);
	this->labelCoin->setColor(ccc3(0,0,0));//black color
	//this->labelCoin->setAnchorPoint(ccp(0, 0));
	this->labelCoin->setPosition(ccp(70, winSize.height - 20));
	//labelCoin->setPosition(ccp(70, 300));

	this->addChild(this->labelCoin);
}

//update()����ÿ֡�����á��������������chipmunk setp�������������綯����
void PlayScene::update(float delta)
{
	if (NULL == this->statusLayer)
	{
		statusLayer = (StatusLayer*)this->getParent()->getChildByTag(1);
	}
	
	//this->statusLayer = (StatusLayer*)this->getParent()->getChildByTag(1);

	if (gameStat == GameStatOver)
	{
		GameOverLayer *gameover = new GameOverLayer();
		this->getParent()->addChild(gameover);
		return;
	}

#if 0
	// Should use a fixed size step based on the animation interval.
	int steps = 2;
	float dt = CCDirector::sharedDirector()->getAnimationInterval()/(float)steps;

	for(int i=0; i<steps; i++)
	{
		cpSpaceStep(space, dt);
	}
#else
	// chipmunk step
	cpSpaceStep(space, delta);
#endif

	// Simulation cpSpaceAddPostStepCallback
	for(unsigned int i = 0; i < shapesToRemove.size(); i++) 
	{
		cpShape *shape = &shapesToRemove[i];

		Coin *objCoin = objectManager->getCoinByShape(shape);
		if (NULL != objCoin)
		{
			//TODO add remove animation
			objectManager->removeCoin(objCoin);
		}

		Rock *objRock = objectManager->getRockByShape(shape);
		if (NULL != objRock)
		{
			//TODO add remove animation
			objectManager->removeRock(objRock);
		}
	}

	shapesToRemove.clear();

	// check and reload mapbackgroundLayer
	if (true == map->checkAndReload(lastEyeX))
	{
	//if (true == backgroundLayer->checkAndReload(lastEyeX)) {
		objectManager->recycleObjectOfMap(map->getCurMap() - 1);
		objectManager->initObjectOfMap(map->getCurMap() + 1, map->getMapWidth());
		//level up
		runner->levelUp();
	}

	// runner step, to change animation
	runner->step(delta);

	// move Camera
	//runner����λ�û�������������ÿ֡���㣬�����Ҫ����runner���ƶ�����
	//lastEyeX = runner->getPositionX() - runner->offsetPx;
	lastEyeX = runner->getPositionX() - g_runnerStartX;

	// update meter display
	//StatusLayer *statusLayer =(StatusLayer*) this->getParent()->getChildByTag(1);
	statusLayer->updateMeter(runner->getPositionX());

	//// move Camera
	//CCCamera *camera = this->getCamera();
	//float eyeZ = CCCamera::getZEye();
	//camera->setEyeXYZ(lastEyeX, 0, eyeZ);
	//camera->setCenterXYZ(lastEyeX, 0, 0);
	AffineTransform tran = runner->getNodeToParentAffineTransform();
	runner->setAdditionalTransform( AffineTransformTranslate( tran, lastEyeX, 0 ) );
}



