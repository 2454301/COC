#ifndef __VILLAGE_SCENE_H__
#define __VILLAGE_SCENE_H__

#include "cocos2d.h"

// 类声明
class Soldier;
class DraggableBuildings;

// 主存庄类
class Village : public cocos2d::Scene {
public:
	static cocos2d::Scene* createScene();
	CREATE_FUNC(Village);

	virtual bool init();

	// 资源处理相关函数
	void addGold(float gold);
	void addElixir(float elixir);
	void reduceGold(float gold);
	void reduceElixir(float elixir);
	bool goldIsEnough(float gold);
	bool elixirIsEnough(float elixir);
	bool soldierIsFull() {
		return _numOfBarbarians + _numOfArchers + _numOfGiants + _numOfGoblins == _maxSoldier;
	}

	// 容量扩充相关函数
	void promoteGoldVolume() {
		_maxGold += 1000;
	}
	void promoteElixirVolume() {
		_maxElixir += 1000;
	}
	void promoteSoldierVolume() {
		_maxSoldier += 40;
	}

	// 建筑处理相关函数
	void addBuilding(DraggableBuildings* building);
	bool isTouching(DraggableBuildings* pointBuilding);

	// 更新资源标签
	void updateResourceLabels();

	// 更新兵种标签
	void updateTroopLabels();

	// 高亮显示相关函数
	void showBuildingOutlines();
	void hideBuildingOutlines();
	void showGridArea();
	void hideGridArea();

	virtual void onEnter() override;

	// 训练的兵种数量
	int _numOfBarbarians = 0;
	int _numOfArchers = 0;
	int _numOfGiants = 0;
	int _numOfGoblins = 0;

	// 场上的建筑集合
	std::vector<DraggableBuildings*> _myBuildings;

private:
	// 初始金币、圣水
	float _gold = 1000.0f;
	float _elixir = 1000.0f;

	float _maxGold = 3000.0f;
	float _maxElixir = 3000.0f;
	int _maxSoldier = 20.0f;

	// 金币、圣水数量标签
	cocos2d::Label* _goldLabel = nullptr;
	cocos2d::Label* _elixirLabel = nullptr;

	// 兵种数量标签
	cocos2d::Label* _barbarianLabel = nullptr;
	cocos2d::Label* _archerLabel = nullptr;
	cocos2d::Label* _giantLabel = nullptr;
	cocos2d::Label* _goblinLabel = nullptr;
	cocos2d::Label* _troopLabel = nullptr;

	// 高亮显示相关
	cocos2d::DrawNode* _buildingOutlineDrawNode = nullptr;
	cocos2d::DrawNode* _gridAreaDrawNode = nullptr;

	// 关卡系统相关菜单按钮及回调函数
	cocos2d::MenuItemImage* levelItem;
	cocos2d::MenuItemImage* levelItem_1;
	cocos2d::MenuItemImage* levelItem_2;
	cocos2d::MenuItemImage* levelItem_3;
	void levelCallBack(cocos2d::Ref* psender);
	void levelCallBack_1(cocos2d::Ref* psender);
	void levelCallBack_2(cocos2d::Ref* psender);
	void levelCallBack_3(cocos2d::Ref* psender);
};

// 可拖动建筑类（主村庄和关卡中的建筑需要实现的功能不一样，故新写一个类）
class DraggableBuildings : public cocos2d::Sprite {
public:
	// 枚举建筑种类
	enum BuildingType {
		NONE,
		TOWN_HALL,
		CANNON,
		GOLD_MINE,
		ELIXIR_COLLECTOR,
		ARMY_CAMP,
		BARRACKS,
		GOLD_STORAGE,
		ELIXIR_STORAGE
	};

	static DraggableBuildings* create(const std::string& filename);
	virtual bool init(const std::string& filename);

	// 初始化相关设置函数
	void setDraggable(bool draggable);
	void setVillage(Village* village);
	bool isDragging() {
		return _isDragging;
	}
	void setSize(int size) {
		_size = size;
	}

	// 升级函数
	void upgrade();

	// 设置和获取建筑种类
	void setBuildingType(BuildingType type) {
		_buildingType = type;
	}
	BuildingType getBuildingType() {
		return _buildingType;
	}

	// 点击回调函数
	void onBuildingClicked();

	// 菜单栏相关函数
	void initInfoPanel();
	void showInfoPanel();
	void hideInfoPanel();
	void hideOtherBuildingPanels();

private:
	// 判断拖动相关
	bool _isDragging = false;
	bool _hasMoved = false;

	// 坐标设置相关
	cocos2d::Vec2 _dragOffset;
	cocos2d::Vec2 _originalPos;
	cocos2d::Vec2 _touchBeganPos;
	int _size = 0;
	
	// 等级（初始1级）
	int _level = 1;

	// 建筑种类
	BuildingType _buildingType = NONE;

	// 和主村庄关联
	Village* _village = nullptr;

	// 各种菜单项
	cocos2d::Label* _levelLabel = nullptr;
	cocos2d::MenuItemImage* _upgradeButton = nullptr;
	cocos2d::MenuItemImage* _collectButton = nullptr;

	cocos2d::MenuItemImage* _trainBarbarianButton = nullptr;
	cocos2d::MenuItemImage* _trainArcherButton = nullptr;
	cocos2d::MenuItemImage* _trainGiantButton = nullptr;
	cocos2d::MenuItemImage* _trainGoblinButton = nullptr;

	cocos2d::Menu* _infoMenu = nullptr;
	bool _infoPanelVisible = false;

	int _originalZOrder = 0;

	// 触摸事件回调
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	// 升级、收集回调
	void onUpgradeClicked(cocos2d::Ref* sender);
	void onCollectClicked(cocos2d::Ref* sender);

	// 训练回调
	void onTrainBarbarianClicked(cocos2d::Ref* sender);
	void onTrainArcherClicked(cocos2d::Ref* sender);
	void onTrainGiantClicked(cocos2d::Ref* sender);
	void onTrainGoblinClicked(cocos2d::Ref* sender);

	// 更新菜单
	void updateInfoPanel();
};

#endif // __VILLAGE_SCENE_H__