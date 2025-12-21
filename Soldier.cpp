#include "Soldier.h"
#include "Building.h"

Soldier* Soldier::createBarbarian() {
	Soldier* soldier = new Soldier();
	if (soldier && soldier->initBarbarian()) {
		soldier->autorelease();
		return soldier;
	}
	delete soldier;
	return nullptr;
}

bool Soldier::initBarbarian() {
	if (!GameObject::init()) {
		return false;
	}

	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("COCBarbarianrun.plist");
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("COCBarbarianattack.plist");
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("COCBarbariandie.plist");

	_runFrames.reserve(4);
	for (int i = 1;i <= 4;i++) {
		std::string frameName = cocos2d::StringUtils::format("barbarian run%d.png", i);
		cocos2d::SpriteFrame* frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
		_runFrames.pushBack(frame);
	}
	_attackFrames.reserve(3);
	for (int i = 1;i <= 3;i++) {
		std::string frameName = cocos2d::StringUtils::format("barbarian attack%d.png", i);
		cocos2d::SpriteFrame* frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);;
		_attackFrames.pushBack(frame);
	}
	_dieFrames.reserve(3);
	for (int i = 1;i <= 3;i++) {
		std::string frameName = cocos2d::StringUtils::format("barbarian die%d.png", i);
		cocos2d::SpriteFrame* frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);;
		_dieFrames.pushBack(frame);
	}

	// 野蛮人相关数值（暂定）
	_health = _maxHealth = 150.0f;
	_attack = 10.0f;
	_speed = 50.0f;
	_range = 30.0f;

	_resourceCost = 50.0f;

	_sprite = cocos2d::Sprite::createWithSpriteFrameName("barbarian run1.png"); // 这里需要图片
	this->addChild(_sprite);
	_sprite->setPosition(cocos2d::Vec2::ZERO);
	return true;
}

Soldier* Soldier::createArcher() {
	Soldier* soldier = new Soldier();
	if (soldier && soldier->initArcher()) {
		soldier->autorelease();
		return soldier;
	}
	delete soldier;
	return nullptr;
}

bool Soldier::initArcher() {
	if (!GameObject::init()) {
		return false;
	}

	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("COCArcher.plist");
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("COCArcherattack.plist");

	_runFrames.reserve(4);
	for (int i = 1;i <= 4;i++) {
		std::string frameName = cocos2d::StringUtils::format("archer1.png");
		cocos2d::SpriteFrame* frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
		_runFrames.pushBack(frame);
	}
	_attackFrames.reserve(4);
	for (int i = 1;i <= 4;i++) {
		std::string frameName = cocos2d::StringUtils::format("archer%d.png", i);
		cocos2d::SpriteFrame* frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
		_runFrames.pushBack(frame);
	}
	_dieFrames.reserve(3);
	for (int i = 1;i <= 4;i++) {
		std::string frameName = cocos2d::StringUtils::format("archer1.png");
		cocos2d::SpriteFrame* frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
		_runFrames.pushBack(frame);
	}

	// 弓箭手相关数值
	_health = _maxHealth = 100.0f;
	_attack = 10.0f;
	_speed = 50.0f;
	_range = 200.0f;

	_resourceCost = 50.0f;

	_sprite = cocos2d::Sprite::createWithSpriteFrameName("archer1.png"); // 这里需要图片
	this->addChild(_sprite);
	_sprite->setPosition(cocos2d::Vec2::ZERO);
	return true;
}

Soldier* Soldier::createGiant() {
	Soldier* soldier = new Soldier();
	if (soldier && soldier->initGiant()) {
		soldier->autorelease();
		return soldier;
	}
	delete soldier;
	return nullptr;
}

bool Soldier::initGiant() {
	if (!GameObject::init()) {
		return false;
	}

	// 巨人相关数值
	_health = _maxHealth = 250.0f;
	_attack = 20.0f;
	_speed = 150.0f;
	_range = 40.0f;

	_resourceCost = 100.0f;

	_priority = BUILDING_DEFENSE;

	_sprite = cocos2d::Sprite::create("Giant.png"); // 这里需要图片
	this->addChild(_sprite);
	_sprite->setPosition(cocos2d::Vec2::ZERO);
	return true;
}

Soldier* Soldier::createGoblin() {
	Soldier* soldier = new Soldier();
	if (soldier && soldier->initGoblin()) {
		soldier->autorelease();
		return soldier;
	}
	delete soldier;
	return nullptr;
}

bool Soldier::initGoblin() {
	if (!GameObject::init()) {
		return false;
	}

	// 哥布林相关数值
	_health = _maxHealth = 80.0f;
	_attack = 10.0f;
	_speed = 300.0f;
	_range = 20.0f;

	_resourceCost = 50.0f;

	_priority = BUILDING_RESOURCE;

	_sprite = cocos2d::Sprite::create("Goblin.png"); // 这里需要图片
	this->addChild(_sprite);
	_sprite->setPosition(cocos2d::Vec2::ZERO);
	return true;
}

// 寻找攻击目标的逻辑
GameObject* Soldier::findNearestBuilding() {
	if (_availableBuildings.empty()) {
		return nullptr;
	}

	GameObject* nearest_1 = nullptr, * nearest_2 = nullptr;
	float minDistance_1 = FLT_MAX, minDistance_2 = FLT_MAX;
	cocos2d::Vec2 myPos = this->getPosition();

	// 遍历所有建筑对象，找到符合要求的为当前目标
	for (auto building : _availableBuildings) {
		if (building && building->isAlive() && !building->isDestroyed()) {
			float distance = myPos.distance(building->getPosition());
			if (building->getType() == _priority) {
				if (distance < minDistance_1) {
					minDistance_1 = distance;
					nearest_1 = building;
				}
			}
			else {
				if (distance < minDistance_2) {
					minDistance_2 = distance;
					nearest_2 = building;
				}
			}
		}
	}

	return nearest_1 != nullptr ? nearest_1 : nearest_2;
}

// 设置自己的攻击目标
void Soldier::attack(GameObject* target) {
	if (target == nullptr || !target->isAlive()) {
		return;
	}
	
	_target = target;
	_isAttacking = true;
	_isMoving = false;
	_attackTimer = 0.0f;
}

// 停止行为
void Soldier::stop() {
	_isMoving = false;
	_isAttacking = false;
	_target = nullptr;
}

// 行为更新系统（由系统逐帧调用）
void Soldier::updateBehavior(float dt) {
	if (!isAlive()) {
		return;
	}

	// 如果还没有攻击目标
	if (_target == nullptr || !_target->isAlive()) {
		_target = findNearestBuilding();
		if (_target) {
			attack(_target);
		}
		else {
			stop();
			return;
		}
	}

	// 如果确定攻击目标且目标存活
	if (_isAttacking && _target && _target->isAlive()) {
		float distance = this->getPosition().distance(_target->getPosition());

		// 如果目标在攻击范围内
		if (distance <= _range) {
			_isMoving = false;

			_attackTimer += dt;
			if (_attackTimer >= 1.0f) { // 设置攻速1秒
				_sprite->stopAllActions();
				_target->onHit(_attack); // 攻击目标
				auto animation = cocos2d::Animation::createWithSpriteFrames(_attackFrames, 0.1f);
				auto animate = cocos2d::Animate::create(animation);
				_sprite->runAction(animate);
				_attackTimer = 0.0f;
			}
		}
		// 如果在攻击范围外
		else {
			_isMoving = true;

			_runTimer += dt;
			if (_runTimer >= 0.6f) {
				_sprite->stopAllActions();
				auto animation = cocos2d::Animation::createWithSpriteFrames(_runFrames, 0.1f);
				auto animate = cocos2d::Animate::create(animation);
				_sprite->runAction(animate);
				_runTimer = 0.0f;
			}

			cocos2d::Vec2 currentPos = this->getPosition();
			cocos2d::Vec2 targetPos = _target->getPosition();
			cocos2d::Vec2 direction = targetPos - currentPos;

			direction.normalize(); // 标准化

			float moveDistance = _speed * dt; // 每帧需要移动

			cocos2d::Vec2 newPos = currentPos + direction * moveDistance;

			this->setPosition(newPos); // 设置新位置，因为是逐帧调用所以不需要用动作

			if (_sprite) {
				_sprite->setPosition(cocos2d::Vec2::ZERO);
			}
		}
	}


}

void Soldier::onHit(float damage) {
	_health -= damage;
	if (!isAlive()) {
		onDestroy();
	}
}

// 死了就移除掉
void Soldier::onDestroy() {
	GameObject::onDestroy();

	_sprite->stopAllActions();

	auto removeAction = cocos2d::CallFunc::create([this]() {
		if (this && this->getParent()) {
			this->removeFromParent();
		}
	});

	auto animation = cocos2d::Animation::createWithSpriteFrames(_dieFrames, 0.1f);
	auto animate = cocos2d::Animate::create(animation);
	_sprite->runAction(animate);

	auto sequence = cocos2d::Sequence::create(
		cocos2d::DelayTime::create(0.4f),
		removeAction,
		nullptr
	);
	this->runAction(sequence);
}