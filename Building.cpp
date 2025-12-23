#include "Soldier.h"
#include "Building.h"

Building* Building::createTownHall() {
	Building* building = new Building();
	if (building && building->initTownHall()) {
		building->autorelease();
		return building;
	}
	delete building;
	return nullptr;
}

// 大本营初始化
bool Building::initTownHall() {
	if (!GameObject::init()) {
		return false;
	}

	_health = _maxHealth = 500.0f; // 500血量
	_type = BUILDING_RESOURCE; // 大本营是资源类建筑

	_sprite = cocos2d::Sprite::create("COCTown Hall.png"); // 这里需要一个图片文件
	// 添加进场景并调整一些参数
	this->addChild(_sprite);
	_sprite->setPosition(cocos2d::Vec2::ZERO);
	return true;
}

Building* Building::createCannon() {
	Building* building = new Building();
	if (building && building->initCannon()) {
		building->autorelease();
		return building;
	}
	delete building;
	return nullptr;
}

bool Building::initCannon() {
	if (!GameObject::init()) {
		return false;
	}

	_health = _maxHealth = 300.0f;
	_type = BUILDING_DEFENSE; // 加农炮是防御类建筑

	_attack = 40.0f;
	_range = 250.0f;

	_resourceCost = 100.0f;

	_sprite = cocos2d::Sprite::create("COCCannon.png");
	this->addChild(_sprite);
	_sprite->setPosition(cocos2d::Vec2::ZERO);
	return true;
}

Building* Building::createArmyCamp() {
	Building* building = new Building();
	if (building && building->initArmyCamp()) {
		building->autorelease();
		return building;
	}
	delete building;
	return nullptr;
}

bool Building::initArmyCamp() {
	if (!GameObject::init()) {
		return false;
	}

	_health = _maxHealth = 300.0f;

	_resourceCost = 150.0f;

	_sprite = cocos2d::Sprite::create("ArmyCamp.png");
	this->addChild(_sprite);
	_sprite->setPosition(cocos2d::Vec2::ZERO);
	return true;
}

Building* Building::createBarracks() {
	Building* building = new Building();
	if (building && building->initBarracks()) {
		building->autorelease();
		return building;
	}
	delete building;
	return nullptr;
}

bool Building::initBarracks() {
	if (!GameObject::init()) {
		return false;
	}

	_health = _maxHealth = 300.0f;

	_resourceCost = 300.0f;

	_sprite = cocos2d::Sprite::create("COCBarracks.png");
	this->addChild(_sprite);
	_sprite->setPosition(cocos2d::Vec2::ZERO);
	return true;
}

Building* Building::createGoldMine() {
	Building* building = new Building();
	if (building && building->initGoldMine()) {
		building->autorelease();
		return building;
	}
	delete building;
	return nullptr;
}

bool Building::initGoldMine() {
	if (!GameObject::init()) {
		return false;
	}

	_health = _maxHealth = 300.0f;
	_type = BUILDING_RESOURCE;

	_resourceCost = 100.0f;
	_resourceProduction = 100.0f;

	_sprite = cocos2d::Sprite::create("COCGold Mine.png");
	this->addChild(_sprite);
	_sprite->setPosition(cocos2d::Vec2::ZERO);
	return true;
}

Building* Building::createElixirCollector() {
	Building* building = new Building();
	if (building && building->initElixirCollector()) {
		building->autorelease();
		return building;
	}
	delete building;
	return nullptr;
}

bool Building::initElixirCollector() {
	if (!GameObject::init()) {
		return false;
	}

	_health = _maxHealth = 300.0f;
	_type = BUILDING_RESOURCE;

	_resourceCost = 100.0f;
	_resourceProduction = 100.0f;

	_sprite = cocos2d::Sprite::create("COCElixir Collector.png");
	this->addChild(_sprite);
	_sprite->setPosition(cocos2d::Vec2::ZERO);
	return true;
}

// 寻找第一个进入攻击范围的兵种
GameObject* Building::findFirstSoldier() {
	if (_availableSoldiers.empty() || _type != BUILDING_DEFENSE) {
		return nullptr;
	}

	GameObject* first = nullptr;
	cocos2d::Vec2 myPos = this->getPosition();

	for (auto soldier : _availableSoldiers) {
		if (soldier && soldier->isAlive() && !soldier->isDestroyed()) {
			float distance = myPos.distance(soldier->getPosition());
			if (distance <= _range) {
				first = soldier; // 指向目标兵种
				return first;
			}
		}
	}
	return nullptr;
}

// 设置攻击目标
void Building::attack(GameObject* target) {
	if (target == nullptr || !target->isAlive()) {
		return;
	}

	_target = target;
	_isAttacking = true;
	_attackTimer = 0.0f;
}

void Building::stop() {
	_isAttacking = false;
	_target = nullptr;
}

// 行为更新系统
void Building::updateBehavior(float dt) {
	if (!isAlive()) {
		return;
	}

	if (_target == nullptr || !_target->isAlive()) {
		_target = findFirstSoldier();
		if (_target) {
			attack(_target);
		}
		else {
			stop();
			return;
		}
	}

	if (_isAttacking && _target && _target->isAlive()) {
		_attackTimer += dt;
		if (_attackTimer >= 1.0f) {
			_target->onHit(_attack);
			_attackTimer = 0.0f;
		}
	}
}

// 受攻击判定
void Building::onHit(float damage) {
	_health -= damage;
	if (!isAlive()) {
		onDestroy();
	}
}

// 死了就移除掉
void Building::onDestroy() {
	GameObject::onDestroy();

	auto removeAction = cocos2d::CallFunc::create([this]() {
		if (this && this->getParent()) {
			this->removeFromParent();
		}
		});

	auto sequence = cocos2d::Sequence::create(
		cocos2d::DelayTime::create(0.1f), // 延迟0.1秒
		removeAction,
		nullptr
	);
	this->runAction(sequence);
}