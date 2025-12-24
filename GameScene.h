#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Soldier.h"
#include "Building.h"
#include "VillageScene.h"

// 显示关卡主场景
class Game : public cocos2d::Scene {
public:
    // 场景创建
    static cocos2d::Scene* createScene();
    CREATE_FUNC(Game);

    // 初始化界面
    virtual bool init();

    // 枚举类，便于选择
    enum SoldierType {
        SOLDIER_NONE,
        SOLDIER_BARBARIAN,
        SOLDIER_ARCHER,
        SOLDIER_GIANT,
        SOLDIER_GOBLIN
    };

    // 用于在关卡创建处将训练的兵种读取至关卡中
    void setTroopCounts(int barbarians, int archers, int giants, int goblins) {
        _availableBarbarians = barbarians;
        _availableArchers = archers;
        _availableGiants = giants;
        _availableGoblins = goblins;
        updateTroopLabels();
    }

    // 与主村庄建立联系
    void setVillage(Village* village) {
        _village = village;
    }

    // 与主村庄关联：战斗结束后将剩余兵种反馈回主村庄
    void updateVillageTroopCounts() {
        if (_village) {
            _village->_numOfBarbarians = _availableBarbarians;
            _village->_numOfArchers = _availableArchers;
            _village->_numOfGiants = _availableGiants;
            _village->_numOfGoblins = _availableGoblins;
        }
    }

protected:
    // 触摸开始和结束检测函数
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    // 判断点是否在菱形范围内
    bool isPointInDiamond(const cocos2d::Vec2& point);

    // 创建初始建筑（虚函数，由子类重写）
    virtual void createInitialBuildings();
    // 创建兵种菜单
    void createSoldierMenu();
    // 注册触摸事件
    void setupTouchEvents();
    // 多兵种点击检测
    void onBarbarianClicked(cocos2d::Ref* sender);
    void onArcherClicked(cocos2d::Ref* sender);
    void onGiantClicked(cocos2d::Ref* sender);
    void onGoblinClicked(cocos2d::Ref* sender);

    // 更新兵种数量标签
    void updateTroopLabels();

    // 建筑集合
    std::vector<Building*> _buildings;
    // 兵种集合
    std::vector<Soldier*> _soldiers;
    // 兵种选择按钮
    cocos2d::MenuItemImage* _selectedSoldierButton = nullptr;
    // 确定是否放置兵种
    bool _isPlacingSoldier = false;
    // 确定放置的是哪种兵种
    SoldierType _selectedSoldierType = SOLDIER_NONE;

    // 兵种数量
    int _availableBarbarians = 0;
    int _availableArchers = 0;
    int _availableGiants = 0;
    int _availableGoblins = 0;

    // 标签
    cocos2d::Label* _barbarianLabel = nullptr;
    cocos2d::Label* _archerLabel = nullptr;
    cocos2d::Label* _giantLabel = nullptr;
    cocos2d::Label* _goblinLabel = nullptr;

    // 逐帧调用，更新行为
    void update(float dt) override;

    // 与主村庄联系
    Village* _village = nullptr;
};

// 继承自Game类的关卡，后续可添加2、3关
class Level_1 : public Game {
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(Level_1);

protected:
    void createInitialBuildings() override; // 重写初始建筑布局
};

#endif // __GAME_SCENE_H__