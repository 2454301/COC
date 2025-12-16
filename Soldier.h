#ifndef __SOLDIER_H__
#define __SOLDIER_H__

#include "GameObject.h"

class Building;

// 各函数、成员变量声明
class Soldier : public GameObject {
public:
	// 兵种创建
	static Soldier* createBarbarian();
	static Soldier* createArcher();

	// 设置攻击目标
	void attack(GameObject* target);
	// 停止
	void stop();

	// 传入建筑集合
	void setBuildings(const std::vector<Building*>& buildings) {
		_availableBuildings = buildings;
	}

	// 寻找最近建筑
	GameObject* findNearestBuilding();

	// 更新行为
	void updateBehavior(float dt) override;
	// 受攻击
	void onHit(float damage) override;
	// 被摧毁
	void onDestroy() override;

private:
	// 兵种初始化
	bool initBarbarian();
	bool initArcher();

	// 伤害
	float _attack = 0.0f;
	// 移速
	float _speed = 0.0f;
	// 攻击范围
	float _range = 0.0f;

	// 判断是否移动
	bool _isMoving = false;
	// 判断是否攻击
	bool _isAttacking = false;
	// 攻击目标（这里用的类是GameObject，可能出现兵种互相攻击的情况，以后修复）
	GameObject* _target = nullptr;
	// 目标坐标
	cocos2d::Vec2 _targetPosition = cocos2d::Vec2::ZERO;
	// 攻击时间间隔
	float _attackTimer = 0.0f;

	// 传入的合法建筑集合
	std::vector<Building*> _availableBuildings;
};

#endif // __SOLDIER_H__
