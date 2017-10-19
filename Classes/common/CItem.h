#pragma once
#include "cocos2d.h"
#include "CTrigger.h"
#include "xmlItems.h"
#include "xmlBag .h"

USING_NS_CC;

class CItem :public Node
{
private:
	Sprite* _propSprite;
	Point _prePos; // for drag
	bool _bTouch;
	bool _bVis;
	Rect _propsRect;
	Point _propsPos; // for setRect

	Point _BagPos; 
	Point _ResetPos; 
	bool _isUsed;
	bool _canUse;

	bool _bRetake;

	Rect _targetRect[3]; //set item target rect
	int _targetNum; //number of target rect
	bool _isStagnant;

	const char* _itemName;   //item name ie xxx.png
public:
	static CItem* create();
	CItem();
	~CItem();
//	bool _Del;
	int x = 1;

	void Init(const char* pic);
	void SetPos(Point pos);
	void SetRect(); 
	void SetRect(Point parentPos);
	void SetVisible(bool x);
	void SetCanUse(bool x);
	void SetUsed(bool x);
	bool GetUsed();
	bool GetCanUse();
	void MoveX(float x); // swipe left/right

	void SetBagPos(Point pos);
	Point GetResetPos();
	void SetTarget(cocos2d::Rect target);
	cocos2d::Rect GetTarget(int num);
	bool GetRetake();
	void SetRetake(); // if the item can be retake (from trigger) when item used

	void SetFeedback(cocos2d::Rect target);
	void SetStagnant(bool x);
	bool GetStagnant();
	const char* GetName();

	void Clear();
	void reset();

	bool touchesBegan(cocos2d::Point inPos);
	bool touchesMoved(cocos2d::Point inPos);
	bool touchesEnded(cocos2d::Point inPos, const char* scene, int num);
};

