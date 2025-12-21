#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "GameObject.h"

class Soldier;

class Building : public GameObject {
public:
	// 建筑的创建
	static Building* createTownHall();
	static Building* createCannon();
	static Building* createGoldMine();
	static Building* createElixirCollector();
	static Building* createArmyCamp(); // 训练营
	static Building* createBarracks(); // 兵营

	// 设置攻击目标
	void attack(GameObject* target);
	// 停止
	void stop();

	BuildingType getType() {
		return _type;
	}

	// 传入兵种集合
	void setSoldiers(const std::vector<Soldier*>& soldiers) {
		_availableSoldiers = soldiers;
	}

	// 寻找第一个进入范围的兵种
	GameObject* findFirstSoldier();

	// 行为更新系统
	void updateBehavior(float dt) override;
	// 受攻击和被摧毁
	void onHit(float damage) override;
	void onDestroy() override;

private:
	// 初始化
	bool initTownHall();
	bool initCannon();
	bool initGoldMine();
	bool initElixirCollector();
	bool initArmyCamp();
	bool initBarracks();


	// 等级
	int _level = 1;
	// 攻击力
	float _attack = 0.0f;
	// 攻击范围
	float _range = 0.0f;
	// 建筑占地
	int _size = 0;
	// 建筑种类
	BuildingType _type = BUILDING_NORMAL;
	// 资源类建筑生产效率
	float _resourceProduction = 0.0f;
	// 建造消耗资源
	float _resourceCost = 0.0f;

	// 判断是否在攻击
	bool _isAttacking = false;
	// 攻击目标
	GameObject* _target = nullptr;
	// 攻击时间间隔
	float _attackTimer = 0.0f;

	// 传入的合法兵种集合
	std::vector<Soldier*> _availableSoldiers;
};

#endif // __BUILDING_H__