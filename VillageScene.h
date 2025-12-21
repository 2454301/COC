#ifndef __VILLAGE_SCENE_H__
#define __VILLAGE_SCENE_H__

#include "cocos2d.h"

class Soldier;
class DraggableBuildings;

class Village : public cocos2d::Scene {
public:
	static cocos2d::Scene* createScene();
	CREATE_FUNC(Village);

	virtual bool init();

	void addGold(float gold);
	void addElixir(float elixir);
	void reduceGold(float gold);
	void reduceElixir(float elixir);
	bool goldIsEnough(float gold);
	bool elixirIsEnough(float elixir);

	void addBuilding(DraggableBuildings* building);
	bool isTouching(DraggableBuildings* pointBuilding);

	void updateResourceLabels();

	std::vector<Soldier*> _mySoldiers;
	std::vector<DraggableBuildings*> _myBuildings;

private:
	float _gold = 1000.0f;
	float _elixir = 1000.0f;

	cocos2d::Label* _goldLabel = nullptr;
	cocos2d::Label* _elixirLabel = nullptr;

	cocos2d::MenuItemImage* levelItem;
	cocos2d::MenuItemImage* levelItem_1;
	cocos2d::MenuItemImage* levelItem_2;
	cocos2d::MenuItemImage* levelItem_3;
	void levelCallBack(cocos2d::Ref* psender);
	void levelCallBack_1(cocos2d::Ref* psender);
	void levelCallBack_2(cocos2d::Ref* psender);
	void levelCallBack_3(cocos2d::Ref* psender);
};

class DraggableBuildings : public cocos2d::Sprite {
public:
	enum BuildingType {
		NONE,
		TOWN_HALL,
		CANNON,
		GOLD_MINE,
		ELIXIR_COLLECTOR,
		ARMY_CAMP,
		BARRACKS
	};

	static DraggableBuildings* create(const std::string& filename);
	virtual bool init(const std::string& filename);

	void setDraggable(bool draggable);
	void setVillage(Village* village);
	bool isDragging() {
		return _isDragging;
	}
	void setSize(int size) {
		_size = size;
	}
	void upgrade();

	void setBuildingType(BuildingType type) {
		_buildingType = type;
	}
	BuildingType getBuildingType() {
		return _buildingType;
	}

	void onBuildingClicked();

	void initInfoPanel();
	void showInfoPanel();
	void hideInfoPanel();
	void hideOtherBuildingPanels();

private:
	bool _isDragging = false;
	bool _hasMoved = false;
	cocos2d::Vec2 _dragOffset;
	cocos2d::Vec2 _originalPos;
	cocos2d::Vec2 _touchBeganPos;
	int _size = 0;
	int _level = 1;

	BuildingType _buildingType = NONE;
	Village* _village;

	cocos2d::Label* _levelLabel = nullptr;
	cocos2d::MenuItemImage* _upgradeButton = nullptr;
	cocos2d::MenuItemImage* _collectButton = nullptr;
	cocos2d::Menu* _infoMenu = nullptr;
	bool _infoPanelVisible = false;

	// 触摸事件回调
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	void onUpgradeClicked(cocos2d::Ref* sender);
	void onCollectClicked(cocos2d::Ref* sender);
	void updateInfoPanel();
};

#endif // __VILLAGE_SCENE_H__