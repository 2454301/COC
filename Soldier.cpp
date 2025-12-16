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

	// 野蛮人相关数值（暂定）
	_health = _maxHealth = 150.0f;
	_attack = 10.0f;
	_speed = 250.0f;
	_range = 5.0f;

	_sprite = cocos2d::Sprite::create("Barbarian.png"); // 这里需要图片
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

	// 弓箭手相关数值
	_health = _maxHealth = 100.0f;
	_attack = 10.0f;
	_speed = 250.0f;
	_range = 200.0f;

	_sprite = cocos2d::Sprite::create("Archer.png"); // 这里需要图片
	this->addChild(_sprite);
	_sprite->setPosition(cocos2d::Vec2::ZERO);
	return true;
}

// 寻找攻击目标的逻辑
GameObject* Soldier::findNearestBuilding() {
	if (_availableBuildings.empty()) {
		return nullptr;
	}

	GameObject* nearest = nullptr;
	float minDistance = FLT_MAX;
	cocos2d::Vec2 myPos = this->getPosition();

	// 遍历所有建筑对象，找到离自己最近的作为攻击目标
	// 可能有兵种有优先攻击目标，这一点还未实现
	for (auto building : _availableBuildings) {
		if (building && building->isAlive() && !building->isDestroyed()) {
			float distance = myPos.distance(building->getPosition());
			if (distance < minDistance) {
				minDistance = distance;
				nearest = building;
			}
		}
	}

	return nearest;
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
				_target->onHit(_attack); // 攻击目标
				auto jump = cocos2d::JumpBy::create(0.5, cocos2d::Vec2(0, 0), 100, 1);
				this->runAction(jump); // 随便做的动画
				_attackTimer = 0.0f;
			}
		}
		// 如果在攻击范围外
		else {
			_isMoving = true;

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