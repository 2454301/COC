#include "VillageScene.h"
#include "Soldier.h"
#include "Building.h"
#include "GameScene.h"

USING_NS_CC;

// 类声明
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
	float x = visibleSize.width;
	float y = visibleSize.height;

	// 加载地图
	auto map = TMXTiledMap::create("COCmap1.tmx");
	map->setAnchorPoint(Vec2::ZERO);
	map->setPosition(Vec2::ZERO);
	this->addChild(map, -2);

	// 创建高亮绘制的DrawNode
	_buildingOutlineDrawNode = DrawNode::create();
	this->addChild(_buildingOutlineDrawNode, 10); // 较高的z-order，确保在建筑上方
	_gridAreaDrawNode = DrawNode::create();
	this->addChild(_gridAreaDrawNode, 9); // 稍低的z-order，在建筑边框下方

	// 初始时隐藏高亮
	_buildingOutlineDrawNode->setVisible(false);
	_gridAreaDrawNode->setVisible(false);

	// 放置金币和圣水数量的标签
	_goldLabel = Label::createWithTTF("Gold: 1000 / 3000", "fonts/Marker Felt.ttf", 24);
	_goldLabel->setPosition(Vec2(x / 2, y - 16));
	this->addChild(_goldLabel, 11);
	_elixirLabel = Label::createWithTTF("Elixir: 1000 / 3000", "fonts/Marker Felt.ttf", 24);
	_elixirLabel->setPosition(Vec2(x / 2, y - 48));
	this->addChild(_elixirLabel);

	// 放置兵种数量的标签
	_barbarianLabel = Label::createWithTTF("Barbarian: 0", "fonts/Marker Felt.ttf", 24);
	_barbarianLabel->setPosition(Vec2(96, y - 16));
	this->addChild(_barbarianLabel, 11);
	_archerLabel = Label::createWithTTF("Archer: 0", "fonts/Marker Felt.ttf", 24);
	_archerLabel->setPosition(Vec2(96, y - 48));
	this->addChild(_archerLabel, 11);
	_giantLabel = Label::createWithTTF("Giant: 0", "fonts/Marker Felt.ttf", 24);
	_giantLabel->setPosition(Vec2(96, y - 80));
	this->addChild(_giantLabel, 11);
	_goblinLabel = Label::createWithTTF("Goblin: 0", "fonts/Marker Felt.ttf", 24);
	_goblinLabel->setPosition(Vec2(96, y - 112));
	this->addChild(_goblinLabel, 11);
	_troopLabel = Label::createWithTTF("All Soldier: 0 / 20", "fonts/Marker Felt.ttf", 24);
	_troopLabel->setPosition(Vec2(96, y - 144));
	this->addChild(_troopLabel, 11);

	// 放置建筑大本营
	auto townHall = DraggableBuildings::create("COCTown Hall.png");
	townHall->setPosition(Vec2(1120, 576));
	townHall->setAnchorPoint(Vec2(0, 0));
	townHall->setSize(96);
	townHall->setVillage(this);
	townHall->setBuildingType(DraggableBuildings::TOWN_HALL);
	addBuilding(townHall);

	// 放置建筑加农炮
	auto cannon = DraggableBuildings::create("COCCannon.png");
	cannon->setPosition(Vec2(1120, 672));
	cannon->setAnchorPoint(Vec2(0, 0));
	cannon->setSize(64);
	cannon->setVillage(this);
	cannon->setBuildingType(DraggableBuildings::CANNON);
	addBuilding(cannon);

	// 放置建筑金矿
	auto goldMine = DraggableBuildings::create("COCGold MIne.png");
	goldMine->setPosition(Vec2(928, 640));
	goldMine->setAnchorPoint(Vec2(0, 0));
	goldMine->setSize(64);
	goldMine->setVillage(this);
	goldMine->setBuildingType(DraggableBuildings::GOLD_MINE);
	addBuilding(goldMine);

	// 放置建筑圣水收集器
	auto elixirCollector = DraggableBuildings::create("COCElixir Collector.png");
	elixirCollector->setPosition(Vec2(928, 576));
	elixirCollector->setAnchorPoint(Vec2(0, 0));
	elixirCollector->setSize(64);
	elixirCollector->setVillage(this);
	elixirCollector->setBuildingType(DraggableBuildings::ELIXIR_COLLECTOR);
	addBuilding(elixirCollector);

	// 放置建筑训练营
	auto armyCamp = DraggableBuildings::create("COCArmy Camp.png");
	armyCamp->setPosition(Vec2(1120, 480));
	armyCamp->setScale(0.9);
	armyCamp->setAnchorPoint(Vec2(0, 0));
	armyCamp->setSize(90);
	armyCamp->setVillage(this);
	armyCamp->setBuildingType(DraggableBuildings::ARMY_CAMP);
	addBuilding(armyCamp);

	// 放置建筑兵营
	auto barracks = DraggableBuildings::create("COCBarracks.png");
	barracks->setPosition(Vec2(928, 480));
	barracks->setAnchorPoint(Vec2(0, 0));
	barracks->setSize(96);
	barracks->setVillage(this);
	barracks->setBuildingType(DraggableBuildings::BARRACKS);
	addBuilding(barracks);

	// 放置建筑储金罐
	auto goldStorage = DraggableBuildings::create("COC Gold Storage.png");
	goldStorage->setPosition(Vec2(768, 320));
	goldStorage->setAnchorPoint(Vec2(0, 0));
	goldStorage->setSize(64);
	goldStorage->setVillage(this);
	goldStorage->setBuildingType(DraggableBuildings::GOLD_STORAGE);
	addBuilding(goldStorage);

	// 放置建筑圣水瓶
	auto elixirStorage = DraggableBuildings::create("COC Elixir Collector.png");
	elixirStorage->setPosition(Vec2(1120, 320));
	elixirStorage->setAnchorPoint(Vec2(0, 0));
	elixirStorage->setSize(64);
	elixirStorage->setVillage(this);
	elixirStorage->setBuildingType(DraggableBuildings::ELIXIR_STORAGE);
	addBuilding(elixirStorage);

	// 关卡系统：一个点击后可选择关卡的按钮、三个关卡进入按钮
	levelItem = MenuItemImage::create("Level.png", "Level.png", CC_CALLBACK_1(Village::levelCallBack, this));
	levelItem->setScale(0.3f);
	levelItem_1= MenuItemImage::create("Level_1.png", "Level_1.png", CC_CALLBACK_1(Village::levelCallBack_1, this));
	levelItem_1->setScale(0.5f);
	levelItem_2 = MenuItemImage::create("Level_2.png", "Level_2.png", CC_CALLBACK_1(Village::levelCallBack_2, this));
	levelItem_2->setScale(0.5f);
	levelItem_3 = MenuItemImage::create("Level_3.png", "Level_3.png", CC_CALLBACK_1(Village::levelCallBack_3, this));
	levelItem_3->setScale(0.5f);

	levelItem->setPosition(Vec2(x - 200, y - 200));
	levelItem_1->setPosition(Vec2(x, y + 200));
	levelItem_2->setPosition(Vec2(x, y + 200));
	levelItem_3->setPosition(Vec2(x, y + 200));
	auto menu = Menu::create(levelItem, levelItem_1, levelItem_2, levelItem_3, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 11);	
	this->schedule([this](float dt) {
			for (auto building : _myBuildings) {
				// 只针对资源产出类建筑
				if(building->getBuildingType() == DraggableBuildings::GOLD_MINE ||
						building->getBuildingType() == DraggableBuildings::ELIXIR_COLLECTOR) {
					CCLOG("Updating building resource bar...");
					// 这里的 produceResource 需要你在 DraggableBuildings 类里实现
					building->produceResource(dt);
				}
			}
		},1.0f, "resource_gen_key"
	);
	return true;
}

// 显示所有建筑边框高亮
void Village::showBuildingOutlines() {
	_buildingOutlineDrawNode->clear();
	_buildingOutlineDrawNode->setVisible(true);

	// 设置边框颜色为绿色
	Color4F outlineColor(0.0f, 1.0f, 0.0f, 0.8f);
	float lineWidth = 2.0f;

	// 遍历所有建筑，绘制边框
	for (auto building : _myBuildings) {
		if (building) {
			// 获取建筑在世界坐标系中的边界框
			Rect rect = building->getBoundingBox();

			// 将世界坐标转换为DrawNode的本地坐标
			// 因为 DrawNode 是直接添加到场景中的，所以需要将世界坐标转换为场景坐标
			Vec2 bottomLeft = this->convertToNodeSpace(rect.origin);
			Vec2 bottomRight = this->convertToNodeSpace(Vec2(rect.getMaxX(), rect.getMinY()));
			Vec2 topRight = this->convertToNodeSpace(Vec2(rect.getMaxX(), rect.getMaxY()));
			Vec2 topLeft = this->convertToNodeSpace(Vec2(rect.getMinX(), rect.getMaxY()));

			// 绘制矩形边框
			_buildingOutlineDrawNode->drawLine(bottomLeft, bottomRight, outlineColor);
			_buildingOutlineDrawNode->drawLine(bottomRight, topRight, outlineColor);
			_buildingOutlineDrawNode->drawLine(topRight, topLeft, outlineColor);
			_buildingOutlineDrawNode->drawLine(topLeft, bottomLeft, outlineColor);
		}
	}
}

// 隐藏建筑边框高亮
void Village::hideBuildingOutlines() {
	_buildingOutlineDrawNode->setVisible(false);
}

// 显示网格区域高亮
void Village::showGridArea() {
	_gridAreaDrawNode->clear();
	_gridAreaDrawNode->setVisible(true);

	// 设置网格区域颜色为蓝色
	Color4F gridColor(0.0f, 0.0f, 1.0f, 0.6f);

	// 定义矩形范围的四个顶点
	Vec2 vertices[4] = {
		Vec2(640, 192),
		Vec2(640, 800),
		Vec2(1408, 800),
		Vec2(1408, 192)
	};

	// 绘制矩形边框
	for (int i = 0; i < 4; i++) {
		_gridAreaDrawNode->drawLine(vertices[i], vertices[(i + 1) % 4], gridColor);
	}

	// 填充矩形内部（半透明）
	_gridAreaDrawNode->drawSolidRect(vertices[0], vertices[2], Color4F(0.0f, 0.0f, 1.0f, 0.2f));
}

// 隐藏网格区域高亮
void Village::hideGridArea() {
	_gridAreaDrawNode->setVisible(false);
}

// 更新资源数量标签
void Village::updateResourceLabels() {
	_goldLabel->setString(StringUtils::format("Gold: %d / %d", (int)_gold, (int)_maxGold));
	_elixirLabel->setString(StringUtils::format("Elixir: %d / %d", (int)_elixir, (int)_maxElixir));
}

// 更新兵种数量标签
void Village::updateTroopLabels() {
	_barbarianLabel->setString(StringUtils::format("Barbarian: %d", _numOfBarbarians));
	_archerLabel->setString(StringUtils::format("Archer: %d", _numOfArchers));
	_giantLabel->setString(StringUtils::format("Giant: %d", _numOfGiants));
	_goblinLabel->setString(StringUtils::format("Goblin: %d", _numOfGoblins));
	_troopLabel->setString(StringUtils::format("All Soldier: %d / %d", _numOfBarbarians + _numOfArchers + _numOfGiants + _numOfGoblins, _maxSoldier));
}

// 金币增加
void Village::addGold(float gold) {
	if (_maxGold - _gold < gold) {
		_gold = _maxGold;
	}
	else {
		_gold += gold;
	}
	updateResourceLabels();
}

// 圣水增加
void Village::addElixir(float elixir) {
	if (_maxElixir - _elixir < elixir) {
		_elixir = _maxElixir;
	}
	else {
		_elixir += elixir;
	}
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
	_healthBar = ui::LoadingBar::create("Healthchar.png");
	_healthBar->setDirection(ui::LoadingBar::Direction::LEFT);
	_healthBar->setPercent(100); // 初始满血

	// 设置位置：位于建筑图片的顶部中心，再往上偏移 10 像素
	Size size =this->getContentSize();
	_healthBar->setPosition(Vec2(size.width / 2, size.height + 10));

	// 如果图片太长，可以手动设置尺寸
	_healthBar->ignoreContentAdaptWithSize(false);
	_healthBar->setContentSize(Size(60, 10)); // 宽度60，高度10，你可以根据资源微调

	this->addChild(_healthBar, 101); // 确保在最上层
	_healthBar->setVisible(true);    // 保持一直可见
	return true;
}

void DraggableBuildings::setBuildingType(BuildingType type) {
	_buildingType = type;

	if (_buildingType == GOLD_MINE || _buildingType == ELIXIR_COLLECTOR) {
		if (!_productionNode) {
			_productionNode = Node::create();
			// 1. 位置稍微往建筑中心再调调，高度可以设为 20，让它离地稍微高点
			_productionNode->setPosition(Vec2(this->getContentSize().width / 2, 20));
			this->addChild(_productionNode, 100);

			_productionBar = ui::LoadingBar::create("redchar.png");
			_productionBar->setDirection(ui::LoadingBar::Direction::LEFT);
			_productionBar->setPercent(0);

			// 2. 【核心修改】把 60 改成 100（更长），把 8 改成 15（更粗）
			_productionBar->ignoreContentAdaptWithSize(false);
			_productionBar->setContentSize(Size(100, 30));

			_productionNode->addChild(_productionBar);
			_productionNode->setVisible(false);
		}
	}
}

void DraggableBuildings::produceResource(float dt) {
	// 1. 计算资源产出
	float productionRate = (float)_level * 5.0f;
	_currentInternalResource += productionRate * dt;

	if (_currentInternalResource > _maxCapacity) {
		_currentInternalResource = _maxCapacity;
	}

	// 2. ✨ 更新红色进度条
	if (_productionBar && _productionNode) {
		float percent = (_currentInternalResource / _maxCapacity) * 100.0f;
		_productionBar->setPercent(percent);

		// 超过 5% 才显示进度条
		_productionNode->setVisible(percent > 5.0f);

		// 如果满了（100%），可以加个变色效果，比如变黄或者变亮
		if (percent >= 100.0f) {
			_productionBar->setColor(Color3B::YELLOW); // 满了变黄色提醒
		}
		else {
			_productionBar->setColor(Color3B::WHITE); // 平时保持原色
		}
	}
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

		if (_village) {
			_village->showBuildingOutlines(); // 显示建筑边框高亮
			_village->showGridArea(); // 显示网格区域高亮
			_village->showBuildingOutlines();  // 更新建筑边框
			_village->showGridArea(); // 更新网格区域
		}
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

			if (_village) {
				_village->hideBuildingOutlines();
				_village->hideGridArea();
			}

			return;
		}
		_hasMoved = false;

		hideInfoPanel();
	}

	// 否则触发以下的位置调整逻辑
	Vec2 bestPos;
	float distance = FLT_MAX;

	Vec2 nowPos = this->getPosition();

	for (int x = 640;x <= 1408 - this->_size;x += 32) {
		for (int y = 192;y <= 800 - this->_size;y += 32) {
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

	if (_village) {
		_village->hideBuildingOutlines();
		_village->hideGridArea();
	}
}

// 升级逻辑
void DraggableBuildings::upgrade() {
	// 1. 如果正在升级中，直接跳过，防止重复点击
	if (_isUpgrading) return;

	if (_village) {
		int cost = 200 * _level;
		if (_level < 3 && _village->goldIsEnough(cost)) {
			// 2. 扣除金币
			_village->reduceGold(cost);

			// 3. 进入“施工状态”
			_isUpgrading = true;

			// 设定升级时间：1级升2级需10秒，2级升3级需20秒（你可以自行调整）
			_totalUpgradeTime = (float)_level * 10.0f;
			_upgradeTimer = _totalUpgradeTime;

			// 4. UI 表现：隐藏按钮，显示进度条
			_upgradeButton->setVisible(false);
			_progressNode->setVisible(true);
			_progressBar->setPercent(0);

			// 5. 启动每帧更新函数 update(float dt)
			// 这是让计时器跑起来的关键指令
			this->scheduleUpdate();
			// --- 震动特效启动 ---
			_sakikoSprite = Sprite::create("Sakiko.png");
			if
				(_sakikoSprite) {
				// 设置在建筑上方一点的位置
				_sakikoSprite->setPosition(Vec2(
					this->getContentSize().width / 2, this->getContentSize().height + 40));
				_sakikoSprite->setScale(
					0.5f); // 根据你的图片大小调整缩放
				this->addChild(_sakikoSprite, 15); // 加在建筑上，它会跟着建筑一起抖动

				// 给 Sakiko 加一个简单的淡入效果
				_sakikoSprite->setOpacity(0);
				_sakikoSprite->runAction(FadeIn::create(0.3f));
			}
			auto moveLeft = MoveBy::create(0.05f, Vec2(-2, 0));
			auto moveRight = MoveBy::create(0.05f, Vec2(2, 0));
			auto shakeSeq = Sequence::create(moveLeft, moveRight, nullptr);

			// 给震动起个 Tag（比如 99），方便后面精准停止它
			auto shakeAction = RepeatForever::create(shakeSeq);
			shakeAction->setTag(99);
			this->runAction(shakeAction);

			// 播放一个小音效或建筑抖动反馈
			this->runAction(Sequence::create(ScaleTo::create(0.1f, 1.05f), ScaleTo::create(0.1f, 1.0f), nullptr));
		}
		else {
			// 钱不够时的反馈
			this->showFullTip("Not Enough Gold!");
		}
	}
}

void DraggableBuildings::update(float dt)
{
	if(_isUpgrading) {
		_upgradeTimer -= dt;

		float percent = ((_totalUpgradeTime - _upgradeTimer) / _totalUpgradeTime) * 100.0f;
		if (_progressBar) { // 加个安全检查，防止崩溃
			_progressBar->setPercent(percent);
		}

		if (_upgradeTimer <= 0) {
			_isUpgrading =false;
			this->unscheduleUpdate();
			if (_progressNode) _progressNode->setVisible(false);
			this->stopActionByTag(99);
			this->setPosition(_originalPos); // 确保建筑不会停在歪掉的位置
			if (_sakikoSprite) {
				// 消失动画：边缩小边变透明，然后移除
				auto fadeOut = FadeOut::create(0.2f);
				auto scaleToZero = ScaleTo::create(0.2f, 0.0f);
				auto remove = RemoveSelf::create();
				_sakikoSprite->runAction(Sequence::create(Spawn::create(fadeOut, scaleToZero, nullptr), remove, nullptr));
				_sakikoSprite = nullptr; // 重置指针
			}

			auto mortis = Sprite::create("Mortis.png");
			if(mortis) {
				// 将 Mortis 放在建筑中心上方一点
				mortis->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height + 50));
				mortis->setScale(0.1f); // 初始极小
				mortis->setOpacity(0);   // 初始透明
				this->addChild(mortis, 30);

				// 模拟心脏跳动动作：猛地放大 -> 略微收缩 -> 再次放大并消失
				auto fadeIn = FadeTo::create(0.1f, 255);
				auto beat1 = ScaleTo::create(0.15f, 1.2f); // 第一次猛烈跳动
				auto beat2 = ScaleTo::create(0.1f, 0.9f);  // 快速收缩
				auto beat3 = ScaleTo::create(0.15f, 1.1f); // 第二次轻微跳动
				auto fadeOut = FadeOut::create(0.3f);      // 消失

				// 动作序列：跳动完后自动从内存删除
				auto
					seq = Sequence::create(Spawn::create(fadeIn, beat1,nullptr),beat2,beat3,fadeOut,RemoveSelf::create(),nullptr);
				mortis->runAction(seq);
			}

			_level++;

			auto scaleDown = ScaleTo::create(0.1f, 0.9f);
			auto scaleUp = ScaleTo::create(0.1f, 1.2f);
			auto scaleBack = ScaleTo::create(0.2f, 1.0f);
			this->runAction(Sequence::create(scaleDown, scaleUp, scaleBack, nullptr));

			this->showFullTip("UPGRADED!");

			if(_village) {
				// 记得把你之前的扩容逻辑写在这里
				if (this->_buildingType == GOLD_STORAGE) _village->promoteGoldVolume();
				else if (this->_buildingType == ELIXIR_STORAGE) _village->promoteElixirVolume();

				_village->updateResourceLabels();
			}

			updateInfoPanel();
			this->showFullTip("Level Up!");
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

// 初始化建筑相关菜单
void DraggableBuildings::initInfoPanel() {
	_levelLabel = Label::createWithTTF("Lv.1", "fonts/Marker Felt.ttf", 18);
	_levelLabel->setPosition(Vec2(-60, 10));
	_levelLabel->setVisible(false);
	this->addChild(_levelLabel, 11);

	// --- 修改升级按钮部分 ---
	_upgradeButton = MenuItemImage::create("UpgradeButton.png", "UpgradeButton.png", CC_CALLBACK_1(DraggableBuildings::onUpgradeClicked, this));

	// 重点 1：统一缩放。如果图片太大，我们把它缩到和收集按钮类似的体感大小
	// 假设你的图片很大，我们先尝试设置一个较小的缩放比例（如 0.2 或 0.1）
	_upgradeButton->setScale(0.05f);

	if (_upgradeButton->getContentSize().width <= 0) {
		auto label = Label::createWithTTF("UPGRADE", "fonts/Marker Felt.ttf", 16);
		label->setColor(Color3B::GREEN);
		// 重点 2：如果图片不存在，ContentSize 是 0，label 的位置要设为 Vec2::ZERO 才能居中
		label->setPosition(Vec2::ZERO);
		_upgradeButton->addChild(label);
		// 如果是纯文字，缩放设回 1.0 否则字会太小
		_upgradeButton->setScale(1.0f);
	}

	// 重点 3：位置微调。既然变小了，坐标可以稍微往右挪一点
	_upgradeButton->setPosition(Vec2(-20, 20));
	_upgradeButton->setVisible(false);

	_collectButton = MenuItemImage::create("CollectButton.png", "CollectButton.png", CC_CALLBACK_1(DraggableBuildings::onCollectClicked, this));
	_collectButton->setScale(0.08f);
	_collectButton->setPosition(Vec2(0, 80));
	_collectButton->setVisible(false);

	// 训练按钮组保持你原来的逻辑
	_trainBarbarianButton = MenuItemImage::create("BarbarianTrain.png", "BarbarianTrain.png", CC_CALLBACK_1(DraggableBuildings::onTrainBarbarianClicked, this));
	_trainBarbarianButton->setPosition(Vec2(0, -10));
	_trainBarbarianButton->setVisible(false);

	_trainArcherButton = MenuItemImage::create("ArcherTrain.png", "ArcherTrain.png", CC_CALLBACK_1(DraggableBuildings::onTrainArcherClicked, this));
	_trainArcherButton->setPosition(Vec2(40, -10));
	_trainArcherButton->setVisible(false);

	_trainGiantButton = MenuItemImage::create("GiantTrain.png", "GiantGiantTrain.png", CC_CALLBACK_1(DraggableBuildings::onTrainGiantClicked, this));
	_trainGiantButton->setPosition(Vec2(80, -10));
	_trainGiantButton->setVisible(false);

	_trainGoblinButton = MenuItemImage::create("GoblinTrain.png", "GoblinTrain.png", CC_CALLBACK_1(DraggableBuildings::onTrainGoblinClicked, this));
	_trainGoblinButton->setPosition(Vec2(120, -10));
	_trainGoblinButton->setVisible(false);

	_infoMenu = Menu::create(_upgradeButton, _collectButton, _trainBarbarianButton, _trainArcherButton, _trainGiantButton, _trainGoblinButton, nullptr);
	_infoMenu->setPosition(Vec2::ZERO);
	_infoMenu->setVisible(false);
	this->addChild(_infoMenu, 11);
	// --- 进度条 UI 创建 ---
	_progressNode = Node::create();
	_progressNode->setPosition(Vec2(0, 110));
	_progressNode->setVisible(false);
	this->addChild(_progressNode, 20);

	// 绿色填充条
	_progressBar = ui::LoadingBar::create("ProgressBarFill.png");
	_progressBar->setDirection(ui::LoadingBar::Direction::LEFT);
	_progressBar->setPercent(0);
	_progressBar->setPosition(Vec2::ZERO);
	_progressNode->addChild(_progressBar);

	
}
// 显示建筑相关菜单
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
	// 生产类建筑设置收集按钮为可见
	if
		(_buildingType == GOLD_MINE || _buildingType == ELIXIR_COLLECTOR) {
		_collectButton->setVisible(true);

		// 1. 先重置缩放（防止多次点击导致缩放错乱）
		_collectButton->setScale(0.08f);

		// 2. 清除之前的动作
		_collectButton->stopAllActions();

		// 3. 创建并运行呼吸动画
		auto scaleUp = ScaleTo::create(0.8f, 0.1f);
		auto scaleDown = ScaleTo::create(0.8f, 0.05f);
		auto seq = Sequence::create(scaleUp, scaleDown, nullptr);
		_collectButton->runAction(RepeatForever::create(seq));
	}
	else
	{
		_collectButton->setVisible(false);
		_collectButton->stopAllActions();
		// 隐藏时停止动画，节省性能
	}

	// 训练营设置训练按钮可见
	if (_buildingType == ARMY_CAMP) {
		_trainBarbarianButton->setVisible(true);
		_trainArcherButton->setVisible(true);
		_trainGiantButton->setVisible(true);
		_trainGoblinButton->setVisible(true);
	}
}

// 隐藏建筑相关菜单
void DraggableBuildings::hideInfoPanel() {
	_levelLabel->setVisible(false);
	_infoMenu->setVisible(false);
	_infoPanelVisible = false;
}

// 隐藏其他建筑相关菜单
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

// 更新建筑相关菜单
void DraggableBuildings::updateInfoPanel() {
	// 更新等级显示
	std::string levelText = "Lv." + std::to_string(_level);
	_levelLabel->setString(levelText);
}

// 升级按钮回调函数
void DraggableBuildings::onUpgradeClicked(cocos2d::Ref* sender) {
	upgrade();
	updateInfoPanel();
}

// 收集按钮回调函数
void DraggableBuildings::onCollectClicked(cocos2d::Ref* sender) {
	if (!_village) return;

	// 收集逻辑...
	if (_buildingType == GOLD_MINE) _village->addGold(_currentInternalResource);
	else if (_buildingType == ELIXIR_COLLECTOR) _village->addElixir(_currentInternalResource);

	_currentInternalResource = 0.0f; // 清空内部存储

	// 进度条归零并隐藏
	if (_productionBar) _productionBar->setPercent(0);
	if (_productionNode) _productionNode->setVisible(false);

	hideInfoPanel();
}

// 训练野蛮人
void DraggableBuildings::onTrainBarbarianClicked(cocos2d::Ref* sender) {
	if (_village) {
		if (_village->elixirIsEnough(50) && !_village->soldierIsFull()) {
			_village->reduceElixir(50);
			_village->_numOfBarbarians++;

			_village->updateTroopLabels();
		}
	}
}

// 训练弓箭手
void DraggableBuildings::onTrainArcherClicked(cocos2d::Ref* sender) {
	if (_village) {
		if (_village->elixirIsEnough(50) && !_village->soldierIsFull()) {
			_village->reduceElixir(50);
			_village->_numOfArchers++;

			_village->updateTroopLabels();
		}
	}
}

// 训练巨人
void DraggableBuildings::onTrainGiantClicked(cocos2d::Ref* sender) {
	if (_village) {
		if (_village->elixirIsEnough(100) && !_village->soldierIsFull()) {
			_village->reduceElixir(100);
			_village->_numOfGiants++;

			_village->updateTroopLabels();
		}
	}
}

// 训练哥布林
void DraggableBuildings::onTrainGoblinClicked(cocos2d::Ref* sender) {
	if (_village) {
		if (_village->elixirIsEnough(50) && !_village->soldierIsFull()) {
			_village->reduceElixir(50);
			_village->_numOfGoblins++;

			_village->updateTroopLabels();
		}
	}
}

void Village::playCialloEffect(Vec2 pos) {
	auto ciallo = Sprite::create("Ciallo.png");
	if (ciallo) {
		ciallo->setPosition(pos);
		ciallo->setScale(0.1f); // 初始很小
		ciallo->setOpacity(255);
		this->addChild(ciallo, 200); // 确保在最上层 (Z-order 设高一些)

		// 动画组合：向上移动、稍微放大、最后淡出
		auto moveUp = MoveBy::create(0.8f, Vec2(0, 120));
		auto scaleTo = ScaleTo::create(0.4f, 0.5f); // 放大到 0.5 倍
		auto fadeOut = FadeOut::create(0.8f);

		// 使用 Spawn 让动作同时进行，EaseExponentialOut 让弹出感更强
		auto action = Spawn::create(
			EaseExponentialOut::create(moveUp),
			EaseBackOut::create(scaleTo),
			fadeOut,
			nullptr
		);

		// 动画结束后自动从内存删除
		ciallo->runAction(Sequence::create(action, RemoveSelf::create(), nullptr));
	}
}

// 以下都是关卡系统相关回调函数

void Village::levelCallBack(Ref* psender) {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	float x = visibleSize.width;
	float y = visibleSize.height;

	// 1. 主按钮点击反馈
	levelItem->runAction(Sequence::create(ScaleTo::create(0.1f, 0.4f), ScaleTo::create(0.1f, 0.3f), nullptr));

	// 2. 特效：Bangdream.png 随机散开 (保持不变)
	int bubbleCount = 8; 
	for (int i = 0; i < bubbleCount; i++) {
		auto sprite = Sprite::create("Bangdream.png");
		if (sprite) {
			sprite->setPosition(levelItem->getPosition());
			sprite->setScale(0.1f);
			this->addChild(sprite, 100);
			float randX = CCRANDOM_MINUS1_1() * 180.0f;
			float randY = 50.0f + CCRANDOM_0_1() * 150.0f;
			float randRotate = CCRANDOM_MINUS1_1() * 360.0f;
			auto moveAction = MoveBy::create(0.8f, Vec2(randX, randY));
			auto rotateAction = RotateBy::create(0.8f, randRotate);
			auto fadeOut = FadeOut::create(0.8f);
			auto scaleTo = ScaleTo::create(0.4f, 0.3f);
			auto spawn = Spawn::create(moveAction, rotateAction, fadeOut, scaleTo, nullptr);
			auto easeAction = EaseQuadraticActionOut::create(spawn);
			sprite->runAction(Sequence::create(easeAction, RemoveSelf::create(), nullptr));
		}
	}

	// 3. ✨ 关卡按钮弹出逻辑（Level_1, 2, 3）
	static bool valid = true; 
	if (valid) { 
		// --- 弹出状态：依次从上方弹入到 y-32 的位置 ---
		
		// Level 1
		levelItem_1->stopAllActions();
		levelItem_1->setPosition(Vec2(x - 96, y + 100)); // 先重置到屏幕上方边缘外
		levelItem_1->runAction(EaseBackOut::create(MoveTo::create(0.4f, Vec2(x - 300, y - 400))));
		
		// Level 2 (延迟0.1秒弹出，增加层次感)
		levelItem_2->stopAllActions();
		levelItem_2->setPosition(Vec2(x - 160, y + 100));
		levelItem_2->runAction(Sequence::create(DelayTime::create(0.1f), EaseBackOut::create(MoveTo::create(0.4f, Vec2(x - 300, y - 600))), nullptr));
		
		// Level 3 (延迟0.2秒弹出)
		levelItem_3->stopAllActions();
		levelItem_3->setPosition(Vec2(x - 224, y + 100));
		levelItem_3->runAction(Sequence::create(DelayTime::create(0.2f), EaseBackOut::create(MoveTo::create(0.4f, Vec2(x - 300, y - 780))), nullptr));

		valid = false;
	}
	else { 
		// --- 缩回状态：平滑收回屏幕上方 y+64 的位置 ---
		levelItem_1->runAction(EaseExponentialIn::create(MoveTo::create(0.3f, Vec2(x - 96, y + 64))));
		levelItem_2->runAction(EaseExponentialIn::create(MoveTo::create(0.3f, Vec2(x - 160, y + 64))));
		levelItem_3->runAction(EaseExponentialIn::create(MoveTo::create(0.3f, Vec2(x - 224, y + 64))));
		
		valid = true;
	}
}

void Village::levelCallBack_1(Ref* psender) {
	auto item = dynamic_cast<MenuItemImage*>(psender);
	if (item) {
		// --- 新增：调用 Ciallo 特效 ---
		playCialloEffect(item->getPosition());

		// 1. 点击反馈动画
		auto scaleDown = ScaleTo::create(0.05f, 0.4f);
		auto scaleUp = ScaleTo::create(0.05f, 0.5f);

		auto jumpToLevel = CallFunc::create([this]() {
			if (_numOfBarbarians == 0 && _numOfArchers == 0 && _numOfGiants == 0 && _numOfGoblins == 0) {
				return;
			}
			auto levelscene = Level_1::createScene();
			auto level = dynamic_cast<Level_1*>(levelscene);
			if (level) {
				level->setTroopCounts(_numOfBarbarians, _numOfArchers, _numOfGiants, _numOfGoblins);
				level->setVillage(this);
			}
			Director::getInstance()->pushScene(levelscene);
			});

		item->runAction(Sequence::create(scaleDown, scaleUp, jumpToLevel, nullptr));
	}
}

void DraggableBuildings::showFullTip(const std::string& msg) {
	auto label = Label::createWithTTF(msg, "fonts/Marker Felt.ttf", 24);
	if (!label) return;

	label->setPosition(Vec2(0, 120)); // 在建筑上方弹出
	label->setColor(Color3B::RED);
	this->addChild(label, 20);

	// 向上漂浮并消失的动画
	auto move = MoveBy::create(1.0f, Vec2(0, 50));
	auto fade = FadeOut::create(1.0f);
	auto spawn = Spawn::create(move, fade, nullptr);
	label->runAction(Sequence::create(spawn, RemoveSelf::create(), nullptr));
}

void Village::levelCallBack_2(Ref* psender) {
	auto item = dynamic_cast<MenuItemImage*>(psender);
	if (item) {
		playCialloEffect(item->getPosition());

		// 如果没有兵，不让进
		if (_numOfBarbarians == 0 && _numOfArchers == 0 && _numOfGiants == 0 && _numOfGoblins == 0) {
			return;
		}

		auto level2Scene = Level_2::createScene();
		auto level = dynamic_cast<Level_2*>(level2Scene);
		if (level) {
			// 传递兵力数据
			level->setTroopCounts(_numOfBarbarians, _numOfArchers, _numOfGiants, _numOfGoblins);
			level->setVillage(this);
		}
		Director::getInstance()->pushScene(level2Scene);
	}
}

void Village::levelCallBack_3(Ref* psender) {
	auto item = dynamic_cast<MenuItemImage*>(psender);
	if (item) {
		playCialloEffect(item->getPosition());

		if (_numOfBarbarians == 0 && _numOfArchers == 0 && _numOfGiants == 0 && _numOfGoblins == 0) {
			return;
		}

		auto level3Scene = Level_3::createScene();
		auto level = dynamic_cast<Level_3*>(level3Scene);
		if (level) {
			level->setTroopCounts(_numOfBarbarians, _numOfArchers, _numOfGiants, _numOfGoblins);
			level->setVillage(this);
		}
		Director::getInstance()->pushScene(level3Scene);
	}
}
