#include "GameScene.h"
#include "Soldier.h"
#include "Building.h"

USING_NS_CC;

// 这里是游戏主场景，现在还没有背景，会有兵种选择菜单和初始出来的建筑
// 可选择兵种并放置，兵种会按照逻辑自动攻击

// 创建场景（后续可添加更多关卡）
Scene* Game::createScene() {
    return Game::create();
}

Scene* Level_1::createScene() {
    return Level_1::create();
}

// 初始化
bool Game::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    float x = visibleSize.width;
    float y = visibleSize.height;

    // 添加地图
    auto map = TMXTiledMap::create("COCmap1.tmx");
    map->setAnchorPoint(Vec2::ZERO);
    map->setPosition(Vec2::ZERO);
    this->addChild(map, -2);

    // 初始化建筑
    createInitialBuildings();
    // 创建兵种菜单
    createSoldierMenu();
    // 注册触摸事件
    setupTouchEvents();

    // 设置兵种数量标签
    _barbarianLabel = Label::createWithTTF("Barbarian: 0", "fonts/Marker Felt.ttf", 18);
    _barbarianLabel->setPosition(Vec2(64, y - 16));
    this->addChild(_barbarianLabel, 11);
    _archerLabel = Label::createWithTTF("Arhcer: 0", "fonts/Marker Felt.ttf", 18);
    _archerLabel->setPosition(Vec2(64, y - 48));
    this->addChild(_archerLabel, 11);
    _giantLabel = Label::createWithTTF("Giant: 0", "fonts/Marker Felt.ttf", 18);
    _giantLabel->setPosition(Vec2(64, y - 80));
    this->addChild(_giantLabel, 11);
    _goblinLabel = Label::createWithTTF("Goblin: 0", "fonts/Marker Felt.ttf", 18);
    _goblinLabel->setPosition(Vec2(64, y - 112));
    this->addChild(_goblinLabel, 11);
    // 在 Game::init() 靠近末尾的地方添加
    _availableBarbarians = 10; // 强行给你10个野蛮人
    _availableArchers = 10;
    _availableGiants = 5;   
    _availableGoblins = 5;
    _isPlacingSoldier = false;  // 初始设为 false
    _selectedSoldierType = SOLDIER_NONE;

    updateTroopLabels(); // 更新一下左上角的数字显示

    // 启用逐帧更新
    this->scheduleUpdate();

    return true;
}

void Game::update(float dt) {
    // 对合法兵种更新行为
    for (auto soldier : _soldiers) {
        if (soldier && soldier->isAlive() && !soldier->isDestroyed()) {
            soldier->updateBehavior(dt);
        }
    }
    // 对合法建筑更新行为
    for (auto building : _buildings) {
        if (building && building->isAlive() && !building->isDestroyed()) {
            building->setSoldiers(_soldiers);
            building->updateBehavior(dt);
        }
    }
    // 检测：把死亡的兵种从集合中移除
    auto s_it = _soldiers.begin();
    while (s_it != _soldiers.end()) {
        auto soldier = *s_it;
        if (!soldier || soldier->isDestroyed() || !soldier->isAlive()) {
            s_it = _soldiers.erase(s_it);
        }
        else {
            ++s_it;
        }
    }
    // 检测：把摧毁的建筑从集合中移除
    auto b_it = _buildings.begin();
    while (b_it != _buildings.end()) {
        auto building = *b_it;
        if (!building || building->isDestroyed() || !building->isAlive()) {
            if (building) {
                building->removeFromParent();
            }
            b_it = _buildings.erase(b_it);
        }
        else {
            ++b_it;
        }
    }

    // 胜利判断逻辑
    if (_buildings.empty()) { // 场上建筑均被摧毁
        auto victory = Sprite::create("Victory.png");
        victory->setPosition(Vec2(1000, 500));
        this->addChild(victory, 11);

        updateVillageTroopCounts();

        auto delay = DelayTime::create(1.0f);
        this->runAction(delay);
        auto popScene = CallFunc::create([]() {
            Director::getInstance()->popScene();
            });
        this->runAction(Sequence::create(delay, popScene, nullptr)); // 播放胜利CG
    }
    
    // 失败判断逻辑
    bool hasAliveSoldiers = false;
    for (auto soldier : _soldiers) {
        if (soldier && soldier->isAlive() && !soldier->isDestroyed()) {
            hasAliveSoldiers = true;
            break;
        }
    }

    bool hasAvailableTroops = (_availableBarbarians > 0 || _availableArchers > 0 ||
        _availableGiants > 0 || _availableGoblins > 0);

    if (!hasAliveSoldiers && !hasAvailableTroops) { // 场上兵种均阵亡，且可使用兵种已耗尽
        auto defeat = Sprite::create("Defeat.png");
        defeat->setPosition(Vec2(1000, 500));
        this->addChild(defeat, 11);

        updateVillageTroopCounts();

        auto delay = DelayTime::create(1.0f);
        auto popScene = CallFunc::create([]() {
            Director::getInstance()->popScene();
            });
        this->runAction(Sequence::create(delay, popScene, nullptr)); // 播放战败CG
    }
}

void Game::createInitialBuildings() {
    // 基类为空实现，由子类重写
}

// 更新兵种数量标签
void Game::updateTroopLabels() {
    _barbarianLabel->setString(StringUtils::format("Barbarian: %d", _availableBarbarians));
    _archerLabel->setString(StringUtils::format("Archer: %d", _availableArchers));
    _giantLabel->setString(StringUtils::format("Giant: %d", _availableGiants));
    _goblinLabel->setString(StringUtils::format("Goblin: %d", _availableGoblins));
}

// 初始化建筑
// 第一关的建筑布局
void Level_1::createInitialBuildings() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float x = visibleSize.width;
    float y = visibleSize.height;

    auto townHall = Building::createTownHall();
    townHall->setPosition(Vec2(1040, 592));
    this->addChild(townHall);
    _buildings.push_back(townHall);

    auto cannon = Building::createCannon();
    cannon->setPosition(Vec2(1120, 672));
    this->addChild(cannon);
    _buildings.push_back(cannon);
}

// 创建兵种选择菜单
void Game::createSoldierMenu() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 野蛮人兵种选择按钮
    auto barbarianBtn = MenuItemImage::create("Barbarian.png", "Barbarian.png", CC_CALLBACK_1(Game::onBarbarianClicked, this));
    barbarianBtn->setPosition(visibleSize.width * 0.2, 50);
    barbarianBtn->setScale(1.5);

    // 弓箭手兵种选择按钮
    auto archerBtn = MenuItemImage::create("Archer.png", "Archer.png", CC_CALLBACK_1(Game::onArcherClicked, this));
    archerBtn->setPosition(visibleSize.width * 0.4, 50);
    archerBtn->setScale(1.5);

    // 巨人兵种选择按钮
    auto giantBtn = MenuItemImage::create("Giant.png", "Giant.png", CC_CALLBACK_1(Game::onGiantClicked, this));
    giantBtn->setPosition(visibleSize.width * 0.6, 50);
    giantBtn->setScale(1.5);

    // 哥布林兵种选择按钮
    auto goblinBtn = MenuItemImage::create("Goblin.png", "Goblin.png", CC_CALLBACK_1(Game::onGoblinClicked, this));
    goblinBtn->setPosition(visibleSize.width * 0.8, 50);
    goblinBtn->setScale(1.5);

    // 全部添加到菜单
    auto menu = Menu::create(barbarianBtn, archerBtn, giantBtn, goblinBtn, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 11);
}

// 注册触摸事件
void Game::setupTouchEvents() {
    auto listener = EventListenerTouchOneByOne::create();

    listener->onTouchBegan = CC_CALLBACK_2(Game::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(Game::onTouchEnded, this);

    listener->setSwallowTouches(true); // 阻止事件传递

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

// 判断点击位置是否合法
bool Game::isPointInDiamond(const cocos2d::Vec2& point) {
    // 菱形四个顶点
    const cocos2d::Vec2 A(320, 480);
    const cocos2d::Vec2 B(1024, 1024);
    const cocos2d::Vec2 C(1728, 480);
    const cocos2d::Vec2 D(1024, -64);

    // 使用向量叉积法判断点是否在凸多边形内
    // 对于凸多边形，如果点P在每条边的同一侧（都左侧或都右侧），则在多边形内

    // 计算向量
    cocos2d::Vec2 AB = B - A;
    cocos2d::Vec2 AP = point - A;

    cocos2d::Vec2 BC = C - B;
    cocos2d::Vec2 BP = point - B;

    cocos2d::Vec2 CD = D - C;
    cocos2d::Vec2 CP = point - C;

    cocos2d::Vec2 DA = A - D;
    cocos2d::Vec2 DP = point - D;

    // 计算叉积
    float cross1 = AB.x * AP.y - AB.y * AP.x;
    float cross2 = BC.x * BP.y - BC.y * BP.x;
    float cross3 = CD.x * CP.y - CD.y * CP.x;
    float cross4 = DA.x * DP.y - DA.y * DP.x;

    // 如果所有叉积同号，则点在菱形内
    // 取逆时针顺序，所以所有叉积应该都大于0
    return (cross1 >= 0 && cross2 >= 0 && cross3 >= 0 && cross4 >= 0) ||
        (cross1 <= 0 && cross2 <= 0 && cross3 <= 0 && cross4 <= 0);
}

// 点击开始
bool Game::onTouchBegan(Touch* touch, Event* event) {
    return true; // 必须返回true，onTouchEnded才会被调用
}

// 放置兵种会调用此处函数
void Game::onTouchEnded(Touch* touch, Event* event) {
    if (!_isPlacingSoldier || _selectedSoldierType == SOLDIER_NONE) {
        return;
    }

    Vec2 touchLocation = touch->getLocation();

    if (!isPointInDiamond(touchLocation)) {
        return;
    }

    Soldier* newSoldier = nullptr;
    bool canPlace = false;

    switch (_selectedSoldierType) {
        case SOLDIER_BARBARIAN:
            if (_availableBarbarians > 0) {
                newSoldier = Soldier::createBarbarian();
                canPlace = true;
                if (newSoldier) _availableBarbarians--; // 只有创建成功才减数量
            }
            break;
        case SOLDIER_ARCHER:
            if (_availableArchers > 0) {
                newSoldier = Soldier::createArcher();
                canPlace = true;
                if (newSoldier) _availableArchers--;
            }
            break;
        case SOLDIER_GIANT:
            if (_availableGiants > 0) {
                newSoldier = Soldier::createGiant(); // 如果这里返回空，下面会拦截
                canPlace = true;
                if (newSoldier) _availableGiants--;
            }
            break;
        case SOLDIER_GOBLIN:
            if (_availableGoblins > 0) {
                newSoldier = Soldier::createGoblin();
                canPlace = true;
                if (newSoldier) _availableGoblins--;
            }
            break;
        default:
            return;
    }

    // ✨ 修改重点：必须同时满足 newSoldier 不为空且可以放置
    if (newSoldier != nullptr && canPlace) {
        newSoldier->setPosition(touchLocation);
        newSoldier->setBuildings(_buildings);

        this->addChild(newSoldier);

        // 只有在这里确保 newSoldier != nullptr，push_back 才不会触发 CCASSERT 崩溃
        _soldiers.push_back(newSoldier);

        updateTroopLabels();
        updateVillageTroopCounts(); // 统一在放置成功后更新
    }
    else if (canPlace) {
        // 调试用：如果数量够但没创建出来，说明资源或 create 函数有问题
        CCLOG("Error: Failed to create soldier instance for type: %d", _selectedSoldierType);
    }
}

// 以下代码重复较多，以后优化，现在先跑起来

// 点击野蛮人按钮后调用
void Game::onBarbarianClicked(Ref* sender) {
    auto button = dynamic_cast<MenuItemImage*>(sender);
    if (!button) return;

    // 如果已经选在野蛮人按钮上，就取消选择，图标向下移动一点
    if (_selectedSoldierButton == button) {
        _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
                                                          _selectedSoldierButton->getPositionY() - 50));
        _isPlacingSoldier = false;
        _selectedSoldierType = SOLDIER_NONE;
        _selectedSoldierButton = nullptr;
    }
    // 否则更新到这个按钮，图标向上移动一点
    else {
        if (_selectedSoldierButton != nullptr) {
            _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
                                                              _selectedSoldierButton->getPositionY() - 50));
        }
        _isPlacingSoldier = true;
        _selectedSoldierType = SOLDIER_BARBARIAN;
        _selectedSoldierButton = button;
        _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
                                                          _selectedSoldierButton->getPositionY() + 50));
    }
}

// 弓箭手按钮，逻辑同上
void Game::onArcherClicked(Ref* sender) {
    auto button = dynamic_cast<MenuItemImage*>(sender);
    if (!button) return;

    if (_selectedSoldierButton == button) {
        _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
                                                          _selectedSoldierButton->getPositionY() - 50));
        _isPlacingSoldier = false;
        _selectedSoldierType = SOLDIER_NONE;
        _selectedSoldierButton = nullptr;
    }
    else {
        if (_selectedSoldierButton != nullptr) {
            _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
                                                              _selectedSoldierButton->getPositionY() - 50));
        }
        _isPlacingSoldier = true;
        _selectedSoldierType = SOLDIER_ARCHER;
        _selectedSoldierButton = button;
        _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
                                                          _selectedSoldierButton->getPositionY() + 50));
    }
}

void Game::onGiantClicked(Ref* sender) {
    auto button = dynamic_cast<MenuItemImage*>(sender);
    if (!button) return;

    if (_selectedSoldierButton == button) {
        _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
            _selectedSoldierButton->getPositionY() - 50));
        _isPlacingSoldier = false;
        _selectedSoldierType = SOLDIER_NONE;
        _selectedSoldierButton = nullptr;
    }
    else {
        if (_selectedSoldierButton != nullptr) {
            _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
                _selectedSoldierButton->getPositionY() - 50));
        }
        _isPlacingSoldier = true;
        _selectedSoldierType = SOLDIER_GIANT;
        _selectedSoldierButton = button;
        _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
            _selectedSoldierButton->getPositionY() + 50));
    }
}

void Game::onGoblinClicked(Ref* sender) {
    auto button = dynamic_cast<MenuItemImage*>(sender);
    if (!button) return;

    if (_selectedSoldierButton == button) {
        _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
            _selectedSoldierButton->getPositionY() - 50));
        _isPlacingSoldier = false;
        _selectedSoldierType = SOLDIER_NONE;
        _selectedSoldierButton = nullptr;
    }
    else {
        if (_selectedSoldierButton != nullptr) {
            _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
                _selectedSoldierButton->getPositionY() - 50));
        }
        _isPlacingSoldier = true;
        _selectedSoldierType = SOLDIER_GOBLIN;
        _selectedSoldierButton = button;
        _selectedSoldierButton->setPosition(cocos2d::Vec2(_selectedSoldierButton->getPositionX(),
            _selectedSoldierButton->getPositionY() + 50));
    }
}

// --- Level_2 实现 ---
Scene* Level_2::createScene() {
    return Level_2::create();
}

void Level_2::createInitialBuildings() {
    // 1. 核心大本营（两个）
    auto th1 = Building::createTownHall();
    th1->setPosition(Vec2(1024, 600)); // 中心
    this->addChild(th1);
    _buildings.push_back(th1);

    auto th2 = Building::createTownHall();
    th2->setPosition(Vec2(1150, 500)); // 侧翼
    this->addChild(th2);
    _buildings.push_back(th2);

    // 2. 防御塔阵列（四个加农炮，形成交叉火力）
    Vec2 cannonPos[] = {
        Vec2(900, 700),  // 左上
        Vec2(1200, 700), // 右上
        Vec2(900, 400),  // 左下
        Vec2(1200, 400)  // 右下
    };

    for (int i = 0; i < 4; i++) {
        auto cn = Building::createCannon();
        cn->setPosition(cannonPos[i]);
        this->addChild(cn);
        _buildings.push_back(cn);
    }

    // 3. 增加一些“资源罐”作为干扰建筑，消耗士兵火力
    for (int i = 0; i < 3; i++) {
        auto storage = Building::createTownHall(); // 暂时用大本营逻辑代替，或者你有Storage类
        storage->setScale(0.6f); // 缩小一点作为装饰性目标
        storage->setPosition(Vec2(800 + i * 200, 300));
        this->addChild(storage);
        _buildings.push_back(storage);
    }
}

// --- Level_3 实现 ---
Scene* Level_3::createScene() {
    return Level_3::create();
}

void Level_3::createInitialBuildings() {
    auto center = Vec2(1024, 550); // 地图大致中心

    // 1. 核心大本营群（3个，形成品字形）
    Vec2 thPositions[] = { center, center + Vec2(100, -80), center + Vec2(-100, -80) };
    for (auto pos : thPositions) {
        auto th = Building::createTownHall();
        th->setPosition(pos);
        this->addChild(th);
        _buildings.push_back(th);
    }

    // 2. 防御塔林（6个加农炮，环绕护卫）
    for (int i = 0; i < 6; i++) {
        // 使用简单的三角函数让炮台绕中心排布
        float angle = i * (M_PI * 2 / 6);
        float radius = 250.0f;
        Vec2 cannonPos = center + Vec2(cos(angle) * radius, sin(angle) * radius);

        auto cn = Building::createCannon();
        cn->setPosition(cannonPos);
        // 提高第三关难度：让第三关的炮台缩放稍微大一点，看起来更有威慑力
        cn->setScale(1.2f);
        this->addChild(cn);
        _buildings.push_back(cn);
    }

    // 3. 外围障碍物/资源罐（8个，作为第一层防线消耗士兵数量）
    for (int i = 0; i < 8; i++) {
        float angle = i * (M_PI * 2 / 8);
        float radius = 450.0f; // 最外圈
        Vec2 storagePos = center + Vec2(cos(angle) * radius, sin(angle) * radius);

        auto storage = Building::createTownHall(); // 暂代资源罐
        storage->setScale(0.5f);
        storage->setPosition(storagePos);
        storage->setOpacity(180); // 稍微透明一点，区分于主基地
        this->addChild(storage);
        _buildings.push_back(storage);
    }
}

