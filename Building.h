#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "GameObject.h"

class Soldier;

class Building : public GameObject {
public:
	// 建筑的创建
	static Building* createTownHall();
	static Building* createCannon();

	// 设置攻击目标
	void attack(GameObject* target);
	// 停止
	void stop();

	// 传入兵种集合
	void setSoldiers(const std::vector<Soldier*>& soldiers) {
		_availableSoldiers = soldiers;
	}

	// 寻找第一个进入范围的兵种
	GameObject* findFirstSoldier();

	// 升级系统（未完成）
	void upgrade();
	bool isUpgrading() {
		return _isUpgrading;
	}

	// 行为更新系统
	void updateBehavior(float dt) override;
	// 受攻击和被摧毁
	void onHit(float damage) override;
	void onDestroy() override;

private:
	// 初始化
	bool initTownHall();
	bool initCannon();

	// 等级
	int _level = 1;
	// 攻击力
	float _attack = 0.0f;
	// 攻击范围
	float _range = 0.0f;
	float _goldProduction; // 资源建筑可能用上
	float _defense; // 防御力（？）不确定游戏里有没有这一项，先写着

	// 判断是否在升级
	bool _isUpgrading = false;
	// 判断是否在攻击
	bool _isAttacking = false;
	// 攻击目标
	GameObject* _target = nullptr;
	// 升级时间
	float _upgradeTimer = 0.0f;
	// 攻击时间间隔
	float _attackTimer = 0.0f;

	// 传入的合法兵种集合
	std::vector<Soldier*> _availableSoldiers;
};

#endif // __BUILDING_H__