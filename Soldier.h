#ifndef __SOLDIER_H__
#define __SOLDIER_H__

#include "GameObject.h"
#include "Building.h"

// 各函数、成员变量声明
class Soldier : public GameObject {
public:
	static Soldier* createBarbarian();
	static Soldier* createArcher();

	void moveTo(float x, float y);
	void attack(GameObject* target);
	void stop();

	void setBuildings(const std::vector<Building*>& buildings) {
		_availableBuildings = buildings;
	}

	GameObject* findNearestBuilding();

	void updateBehavior(float dt) override;
	void onHit(float damage) override;
	void onDestroy() override;

private:
	bool initBarbarian();
	bool initArcher();

	float _attack = 0.0f;
	float _speed = 0.0f;
	float _range = 0.0f;

	bool _isMoving = false;
	bool _isAttacking = false;
	GameObject* _target = nullptr;
	cocos2d::Vec2 _targetPosition = cocos2d::Vec2::ZERO;
	float _attackTimer = 0.0f;

	std::vector<Building*> _availableBuildings;
};

#endif // __SOLDIER_H__
