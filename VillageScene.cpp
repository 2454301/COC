#include "VillageScene.h"
#include "Soldier.h"
#include "Building.h"
#include "GameScene.h"

USING_NS_CC;

class DraggableBuildings;

// 创建场景
Scene* Village::createScene() {
	return Village::create();
}

// 对主村庄初始化
bool Village::init() {
	if (!Scene::init()) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();

	// 加载地图
	auto map = TMXTiledMap::create("COCMap.tmx");
	map->setAnchorPoint(Vec2::ZERO);
	map->setPosition(Vec2::ZERO);
	map->setScale(0.5);
	this->addChild(map, -2);

	// 放置金币和圣水数量的标签
	_goldLabel = Label::createWithTTF("Gold: 1000", "fonts/Marker Felt.ttf", 24);
	_goldLabel->setPosition(Vec2(352, 688));
	this->addChild(_goldLabel, 11);
	_elixirLabel = Label::createWithTTF("Elixir: 1000", "fonts/Marker Felt.ttf", 24);
	_elixirLabel->setPosition(Vec2(352, 656));
	this->addChild(_elixirLabel);

	// 放置建筑大本营
	auto townHall = DraggableBuildings::create("TownHall.png");
	townHall->setPosition(Vec2(352, 352));
	townHall->setScale(0.64);
	townHall->setAnchorPoint(Vec2(0, 0));
	townHall->setSize(64);
	townHall->setVillage(this);
	townHall->setBuildingType(DraggableBuildings::TOWN_HALL);
	addBuilding(townHall);

	// 放置建筑加农炮
	auto cannon = DraggableBuildings::create("Cannon.png");
	cannon->setPosition(Vec2(320, 320));
	cannon->setScale(0.32);
	cannon->setAnchorPoint(Vec2(0, 0));
	cannon->setSize(32);
	cannon->setVillage(this);
	cannon->setBuildingType(DraggableBuildings::CANNON);
	addBuilding(cannon);

	// 放置建筑金矿
	auto goldMine = DraggableBuildings::create("GoldMIne.png");
	goldMine->setPosition(Vec2(320, 352));
	goldMine->setScale(0.32);
	goldMine->setAnchorPoint(Vec2(0, 0));
	goldMine->setSize(32);
	goldMine->setVillage(this);
	goldMine->setBuildingType(DraggableBuildings::GOLD_MINE);
	addBuilding(goldMine);

	auto elixirCollector = DraggableBuildings::create("ElixirCollector.png");
	elixirCollector->setPosition(Vec2(416, 352));
	elixirCollector->setScale(0.32);
	elixirCollector->setAnchorPoint(Vec2(0, 0));
	elixirCollector->setSize(32);
	elixirCollector->setVillage(this);
	elixirCollector->setBuildingType(DraggableBuildings::ELIXIR_COLLECTOR);
	addBuilding(elixirCollector);

	// 关卡系统：一个点击后可选择关卡的按钮、三个关卡进入按钮
	levelItem = MenuItemImage::create("Level.png", "Level.png", CC_CALLBACK_1(Village::levelCallBack, this));
	levelItem_1= MenuItemImage::create("Level_1.png", "Level_1.png", CC_CALLBACK_1(Village::levelCallBack_1, this));
	levelItem_2 = MenuItemImage::create("Level_2.png", "Level_2.png", CC_CALLBACK_1(Village::levelCallBack_2, this));
	levelItem_3 = MenuItemImage::create("Level_3.png", "Level_3.png", CC_CALLBACK_1(Village::levelCallBack_3, this));
	levelItem->setPosition(Vec2(672, 672));
	levelItem_1->setPosition(Vec2(608, 736));
	levelItem_2->setPosition(Vec2(544, 736));
	levelItem_3->setPosition(Vec2(480, 736));
	auto menu = Menu::create(levelItem, levelItem_1, levelItem_2, levelItem_3, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 11);

	return true;
}

// 更新资源数量标签
void Village::updateResourceLabels() {
	_goldLabel->setString(StringUtils::format("Gold: %d", (int)_gold));
	_elixirLabel->setString(StringUtils::format("Elixir: %d", (int)_elixir));
}

// 金币增加
void Village::addGold(float gold) {
	_gold += gold;
	updateResourceLabels();
}

// 圣水增加
void Village::addElixir(float elixir) {
	_elixir += elixir;
	updateResourceLabels();
}

// 金币减少
void Village::reduceGold(float gold) {
	if (!goldIsEnough(gold)) {
		return;
	}

	_gold -= gold;
	updateResourceLabels();
}

// 圣水减少
void Village::reduceElixir(float elixir) {
	if (!elixirIsEnough(elixir)) {
		return;
	}

	_elixir -= elixir;
	updateResourceLabels();
}

// 判断金币是否足够
bool Village::goldIsEnough(float gold) {
	return _gold >= gold;
}

// 判断圣水是否足够
bool Village::elixirIsEnough(float elixir) {
	return _elixir >= elixir;
}

// 按照逻辑添加建筑
void Village::addBuilding(DraggableBuildings* building) {
	if (building) {
		_myBuildings.push_back(building); // 把建筑放到集合中
		this->addChild(building); // 在场景中显示建筑
	}
}

// 判断建筑是否重叠
bool Village::isTouching(DraggableBuildings* pointBuilding) {
	for (auto building : _myBuildings) { // 遍历集合中的所有建筑
		if (building == pointBuilding) { // 如果建筑是自身就跳过
			continue;
		}

		// 获取两个建筑的边界框
		Rect rect1 = pointBuilding->getBoundingBox();
		Rect rect2 = building->getBoundingBox();

		// 收缩边界框，允许边界重叠
		float shrinkAmount = 2.0f; // 收缩量，2像素对于格子的32像素无影响
		rect1.origin.x += shrinkAmount;
		rect1.origin.y += shrinkAmount;
		rect1.size.width -= 2 * shrinkAmount;
		rect1.size.height -= 2 * shrinkAmount;

		rect2.origin.x += shrinkAmount;
		rect2.origin.y += shrinkAmount;
		rect2.size.height -= 2 * shrinkAmount;
		rect2.size.width -= 2 * shrinkAmount;

		// 使用收缩后的矩形检测重叠
		if (rect1.intersectsRect(rect2)) {
			return true;
		}
	}
	return false;
}

// 主村庄中建筑的创建
DraggableBuildings* DraggableBuildings::create(const std::string& filename)
{
	DraggableBuildings* sprite = new (std::nothrow) DraggableBuildings();
	if (sprite && sprite->init(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

// 主村庄中建筑的初始化
bool DraggableBuildings::init(const std::string& filename)
{
	if (!Sprite::initWithFile(filename))
		return false;

	_isDragging = false;
	_village = nullptr;
	this->setDraggable(true);

	initInfoPanel();

	return true;
}

// 确保DraggableBuildings类可以通过_village来访问到Village类
void DraggableBuildings::setVillage(Village* village) {
	_village = village;
}

// 赋予建筑“可拖动”的性质
void DraggableBuildings::setDraggable(bool draggable)
{
	auto eventDispatcher = Director::getInstance()->getEventDispatcher();

	if (draggable)
	{
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);

		listener->onTouchBegan = CC_CALLBACK_2(DraggableBuildings::onTouchBegan, this);
		listener->onTouchMoved = CC_CALLBACK_2(DraggableBuildings::onTouchMoved, this);
		listener->onTouchEnded = CC_CALLBACK_2(DraggableBuildings::onTouchEnded, this);

		eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	}
	else
	{
		eventDispatcher->removeEventListenersForTarget(this);
	}
}

// 记录点击在某个建筑的开始瞬间
bool DraggableBuildings::onTouchBegan(Touch* touch, Event* event)
{
	_originalPos = this->getPosition();
	
	Vec2 touchLocation = this->convertTouchToNodeSpace(touch);
	Size size = this->getContentSize();
	Rect rect(0, 0, size.width, size.height);

	if (rect.containsPoint(touchLocation))
	{
		_isDragging = true;
		_dragOffset = this->convertToWorldSpace(touchLocation) - this->getPosition();

		_touchBeganPos = touch->getLocation();

		return true;
	}

	return false;
}

// 鼠标移动可以带动建筑一起移动，实现“可拖动”
void DraggableBuildings::onTouchMoved(Touch* touch, Event* event)
{
	if (_isDragging)
	{
		Vec2 newPosition = touch->getLocation() - _dragOffset;
		this->setPosition(newPosition);

		_hasMoved = true;
	}
}

// 点击结束，根据不同情况作出处理
void DraggableBuildings::onTouchEnded(Touch* touch, Event* event)
{
	if (_isDragging)
	{
		_isDragging = false;

		Vec2 touchEndedPos = touch->getLocation();
		float moveDistance = _touchBeganPos.distance(touchEndedPos);
		if (moveDistance <= 10.0f && !_hasMoved) { // 如果移动很小（视作点击而非拖动）
			onBuildingClicked(); // 则触发点击对应的回调函数
			return;
		}
		_hasMoved = false;

		hideInfoPanel();
	}

	// 否则触发以下的位置调整逻辑
	Vec2 bestPos;
	float distance = FLT_MAX;

	Vec2 nowPos = this->getPosition();

	for (int x = 0;x <= 704 - this->_size;x += 32) {
		for (int y = 0;y <= 640 - this->_size;y += 32) {
			float dis = sqrt((x - nowPos.x) * (x - nowPos.x) + (y - nowPos.y) * (y - nowPos.y));
			if (dis < distance) {
				distance = dis;
				bestPos.x = x;
				bestPos.y = y;
			}
		}
	}
	this->setPosition(bestPos); // 这样建筑就会自动吸附网格，而不是随意摆放

	bool isOverlapping = false;
	if (_village) {
		isOverlapping = _village->isTouching(this); // 如果和其他建筑重叠
	}
	if (isOverlapping) {
		this->setPosition(_originalPos); // 就把该建筑放回原位置，表示移动失败
	}
}

// 升级逻辑
void DraggableBuildings::upgrade() {
	if (_village) {
		if (_level < 3 && _village->goldIsEnough(200)) { // 最高3级，且每次升级消耗200金币
			_level++;
			_village->reduceGold(200);
		}
	}
}

// 点击建筑后的回调函数
void DraggableBuildings::onBuildingClicked() {
	if (_infoPanelVisible) {
		hideInfoPanel();
	}
	else {
		showInfoPanel();
	}
}

void DraggableBuildings::initInfoPanel() {
	_levelLabel = Label::createWithTTF("Lv.1", "fonts/Marker Felt.ttf", 18);
	_levelLabel->setPosition(Vec2(0, 110));
	_levelLabel->setVisible(false);
	this->addChild(_levelLabel, 11);

	_upgradeButton = MenuItemImage::create("UpgradeButton.png", "UpgradeButton.png", CC_CALLBACK_1(DraggableBuildings::onUpgradeClicked, this));
	_upgradeButton->setPosition(Vec2(40, 110));
	_upgradeButton->setVisible(false);

	_collectButton = MenuItemImage::create("CollectButton.png", "CollectButton.png", CC_CALLBACK_1(DraggableBuildings::onCollectClicked, this));
	_collectButton->setPosition(Vec2(80, 110));
	_collectButton->setVisible(false);

	_infoMenu = Menu::create(_upgradeButton, _collectButton, nullptr);
	_infoMenu->setPosition(Vec2::ZERO);
	_infoMenu->setVisible(false);
	this->addChild(_infoMenu, 11);
}

void DraggableBuildings::showInfoPanel() {
	if (!_village) {
		return;
	}

	hideOtherBuildingPanels();
	updateInfoPanel();

	_levelLabel->setVisible(true);
	_infoMenu->setVisible(true);
	_upgradeButton->setVisible(true);
	_infoPanelVisible = true;
	if (_buildingType == GOLD_MINE || _buildingType == ELIXIR_COLLECTOR) {
		_collectButton->setVisible(true);
	}
	else {
		_collectButton->setVisible(false);
	}
}

void DraggableBuildings::hideInfoPanel() {
	_levelLabel->setVisible(false);
	_infoMenu->setVisible(false);
	_infoPanelVisible = false;
}

void DraggableBuildings::hideOtherBuildingPanels() {
	if (!_village) {
		return;
	}

	for (auto building : _village->_myBuildings) {
		if (building != this) {
			building->hideInfoPanel();
		}
	}
}

void DraggableBuildings::updateInfoPanel() {
	// 更新等级显示
	std::string levelText = "Lv." + std::to_string(_level);
	_levelLabel->setString(levelText);
}

void DraggableBuildings::onUpgradeClicked(cocos2d::Ref* sender) {
	upgrade();
	updateInfoPanel();
}

void DraggableBuildings::onCollectClicked(cocos2d::Ref* sender) {
	if (!_village) {
		return;
	}

	if (_buildingType == GOLD_MINE) {
		// 收集100金币
		_village->addGold(100);
	}
	else if (_buildingType == ELIXIR_COLLECTOR) {
		// 收集100圣水
		_village->addElixir(100);
	}
}

// 以下都是关卡系统相关回调函数

void Village::levelCallBack(Ref* psender) {
	static bool valid = true;
	if (valid) {
		levelItem_1->setPosition(Vec2(608, 672));
		levelItem_2->setPosition(Vec2(544, 672));
		levelItem_3->setPosition(Vec2(480, 672));
		valid = !valid;
	}
	else {
		levelItem_1->setPosition(Vec2(608, 736));
		levelItem_2->setPosition(Vec2(544, 736));
		levelItem_3->setPosition(Vec2(480, 736));
		valid = !valid;
	}
}

void Village::levelCallBack_1(Ref* psender) {
	auto levelscene = Level_1::createScene();
	Director::getInstance()->pushScene(levelscene);
}

void Village::levelCallBack_2(Ref* psender) {

}

void Village::levelCallBack_3(Ref* psender) {

}