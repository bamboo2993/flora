#include "C2_Scene_02.h"
#include "C2_Scene_03.h"

#include "labScene.h"


USING_NS_CC;
#define SPEED 3
using namespace cocostudio::timeline;

int C2_Scene_02::_from = 0;

Scene* C2_Scene_02::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = C2_Scene_02::create();

    // add layer as a child to scene
    scene->addChild(layer);

	
    // return the scene
    return scene;
}

C2_Scene_02::C2_Scene_02() {

	for (int i = 0; i < 3; i++) {
		_toSpot[i] = false;
	}
	_isWalking = false;
}
// on "init" you need to initialize your instance
bool C2_Scene_02::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("C2Scene02.csb");
    addChild(rootNode);

	//background setting
	_bg = Sprite::create("GameScene/C2Scene/C2_Z2.png");
	_bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(_bg);

	//character setting
	if (_from == 1) {
		_boy = new CPlayer(false, *this, Point(40, 380), true);
		_boy->setAnimation("Animation/boyanim.plist");
		_boy->SetReachSpot(0, true);
	}
	else { // from room
		_boy = new CPlayer(false, *this, Point(830, 245), true);
		_boy->setAnimation("Animation/boyanim.plist");
		_boy->SetReachSpot(2, true);
	}


	//spots
	_spot[0] = (cocos2d::Sprite*)rootNode->getChildByName("spot_0");
	Size size = _spot[0]->getContentSize();
	Point pos = _spot[0]->getPosition();
	_spotRect[0] = Rect(pos.x-size.width*1.9f/2, pos.y-size.height*6/2, size.width*1.9f, size.height*6);

	_spot[1] = (cocos2d::Sprite*)rootNode->getChildByName("spot_1");
	size = _spot[1]->getContentSize();
	pos = _spot[1]->getPosition();
	_spotRect[1] = Rect(pos.x - size.width*1.5 / 2, pos.y - size.height*1.5 / 2, size.width*1.5f, size.height*1.5f);
	
	_spot[2] = (cocos2d::Sprite*)rootNode->getChildByName("spot_2");
	size = _spot[2]->getContentSize();
	pos = _spot[2]->getPosition();
	_spotRect[2] = Rect(pos.x - size.width*1.5 / 2, pos.y - size.height*1.5 / 2, size.width*1.5f, size.height*1.5f);

		
	//door
	_doorArea= (cocos2d::Sprite*)rootNode->getChildByName("doorArea");
	size = _doorArea->getContentSize();
	pos = _doorArea->getPosition();
	_doorRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	_paintArea = (cocos2d::Sprite*)rootNode->getChildByName("paintArea");
	size = _paintArea->getContentSize();
	pos = _paintArea->getPosition();
	_paintRect = Rect(pos.x - size.width / 2, pos.y - size.height / 2, size.width, size.height);

	//add lock=============
	_elock = new CeLock();
	_elock->init("GameScene/lab_door.png");
	this->addChild(_elock);
	_elock->SetArea(_doorRect);
	_elock->SetExitArea(cocos2d::Point(72.0f, 1463.0f), 144.0f, 144.0f);
	_elock->SetKeyArea(Point(896.8f, 692.6f), 287, 254);
	_elock->SetEnterArea(Point(896.8f, 617.6f), 287, 72);
	_elock->SetNumAppear(1149.0f + 40.0f, 1032.0f);
	_elock->SetPassword(5, 17341);

	_listener1 = EventListenerTouchOneByOne::create();	//創建一個一對一的事件聆聽器
	_listener1->onTouchBegan = CC_CALLBACK_2(C2_Scene_02::onTouchBegan, this);		//加入觸碰開始事件
	_listener1->onTouchMoved = CC_CALLBACK_2(C2_Scene_02::onTouchMoved, this);		//加入觸碰移動事件
	_listener1->onTouchEnded = CC_CALLBACK_2(C2_Scene_02::onTouchEnded, this);		//加入觸碰離開事件

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//加入剛創建的事件聆聽器
	this->schedule(CC_SCHEDULE_SELECTOR(C2_Scene_02::doStep));
	return true;

}

void C2_Scene_02::doStep(float dt) {
	if (_toSpot[0]) {
		if (!_boy->GetReachSpot(0)) {
			if (ToSpot0(dt)) {
				_isWalking = false;
				_toSpot[0] = false;
				this->unschedule(schedule_selector(C2_Scene_02::doStep));
				C2_Scene_03::_from = 2;
				Director::getInstance()->replaceScene(C2_Scene_03::createScene());
			}
		}
		else
		{
			this->unschedule(schedule_selector(C2_Scene_02::doStep));
			C2_Scene_03::_from = 2;
			Director::getInstance()->replaceScene(C2_Scene_03::createScene());
		}
	}
	
	if (_toSpot[2]) {
	//	if (!_boy->GetReachSpot(2)) {
			if (ToSpot2(dt)) {
				_toSpot[2] = false;
				_isWalking = false;
				_elock->SetReached(true);

			}
	//	}
	}


	_elock->doStep(dt);
}

bool C2_Scene_02::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
	Point touchLoc = pTouch->getLocation();
	if (!_elock->GetState()) {
		//testing
		if (!_isWalking) {
			if (_spotRect[0].containsPoint(touchLoc)) {
				_toSpot[0] = true;
				log("clicked0");
			}
			if (_doorRect.containsPoint(touchLoc)) {
				_toSpot[2] = true;
				log("door");
			}
		}
	}
	_elock->TouchBegan(touchLoc);

	return true;
}

void C2_Scene_02::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
	Point touchLoc = pTouch->getLocation();
	_elock->TouchMoved(touchLoc);
}

void C2_Scene_02::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
	Point touchLoc = pTouch->getLocation();
	if (_elock->TouchEnded(touchLoc)) {
		this->unschedule(schedule_selector(C2_Scene_02::doStep));;
		Director::getInstance()->replaceScene(labScene::createScene());
	}
}


bool C2_Scene_02::ToSpot0(float dt) {
	Point pos = _boy->getPosition();
	if (_spotRect[0].containsPoint(pos)) {
		_boy->SetReachSpot(2, false);
		_boy->SetReachSpot(0, true);
		_boy->Stop(true);
		return true;
	}
	else { 
		_isWalking = true;
		_boy->Mirror(false);
		_boy->go(true);
		_boy->setPosition(Vec2(pos.x - 178.0f*SPEED*dt, pos.y + 27.0f*SPEED*dt));
		return false;
	}
}

bool C2_Scene_02::ToSpot2(float dt) {
	Point pos = _boy->getPosition();
	if (_spotRect[2].containsPoint(pos)) {
		_boy->SetReachSpot(0, false);
		_boy->SetReachSpot(2, true);
		_boy->Mirror(true);
		_boy->Stop(true);
		return true;
	}
	else {
		_isWalking = true;
		_boy->Mirror(true);
		_boy->go(false);
		_boy->setPosition(Vec2(pos.x + 178.0f*SPEED*dt, pos.y - 27.0f*SPEED*dt));
		return false;
	}
}


