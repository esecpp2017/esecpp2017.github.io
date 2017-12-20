// State.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <cassert>
#include <map>

using std::cout; 
using std::cin; 
using std::endl; 
using std::string; 
using std::map; 

#if 0
// 朴素的实现方式，也是最容易想到的实现
// 也有可能是原本系统并没有这么复杂，随着时间增加
// 需求持续改变，代码演变成当前状态
// 存在的问题是： 
// 1、Action类的职责过于庞大，包括维护状态，维护状态迁移
//    (状态之间O(n^2)关系)，
//    维护每一个状态下，状态迁移的规则，这种庞大的职责在
//    真实的代码中势必有一个很巨大的代码与之对应。这和是否
//    用函数分割无关，而与思考、设计、维护、扩展涉及到的思维
//    负担有关，因此即便分割成一系列小的函数，也同样会因为职责
//    过于庞大，使得每一次扩展或维护，需要很大的投入。
// 2、破坏开闭原则，无扩展性可研。
class Action
{
public:
    enum {
        STATE_STATIC = 0,  
        STATE_PROMOVE, 
        STATE_RUN, 
        STATE_BRAKE 
    };

public:
    Action() : _state(STATE_STATIC) { }
    ~Action() { }
protected:
	void action(int a, int b)
    {
        // 代码的坏味道——例如出现很多if + else语句，
        // 或者出现switch case语句，都表明代码中存在扩展性
        // 障碍问题。
        // 这也就意味着，你可以考虑你的代码是否需要重写——
        // 更准确的说，是重构。
        switch (_state)
        {
        case STATE_STATIC:
            cout << "STATE_STATIC" << endl; 
            if (a == 3)
            {
                _state = STATE_PROMOVE; 
            }
            else if (b == 2)
            {
                _state = STATE_RUN; 
            }
            break; 
        case STATE_PROMOVE:
            cout << "STATE_PROMOVE" << endl; 
            if ((a & 1) == 1)
            {
                _state = STATE_BRAKE; 
            }
            else if (a == 0 && (b & 1) == 0)
            {
                _state = STATE_RUN; 
            }
            break; 
        case STATE_RUN:
            cout << "STATE_RUN" << endl; 
            if (a == 3 && b == 5)
            {
                _state = STATE_BRAKE; 
            }
            break; 
        case STATE_BRAKE:
            cout << "STATE_BRAKE" << endl; 
            if (b == 1)
            {
                _state = STATE_STATIC; 
            }
            break; 
        }
    }
private:
    int _state; 
};
#endif

//////////////////////////////////////////////////////////////////////////
// 一下为State Pattern改写后版本

class StateManager; 
// 所有状态抽象出的接口类，其提供一个接口，该接口
// 承担根据不同状态进行不同操作的职责，接口名字、参数
// 等等，完全可以根据系统需求决定。
class State
{
public:
    virtual void action(StateManager *manager, int a, int b) = 0; 

    // 既然有继承存在，析构函数用虚函数，保证释放时正确析构
    virtual ~State() { }
};

// 承担管理当前状态的职责，负责状态维护及迁移
class StateManager
{
public:
    // 对外提供一个“可读”的状态识别方式
    enum {
        STATE_STATIC = 0,  
        STATE_PROMOVE, 
        STATE_RUN, 
        STATE_BRAKE 
    };

private:
    typedef std::map<int, State *> States; 

public:
    StateManager() 
    {
    }

    // 直接调用State类的action操作，
    // 不关心（也不知道）具体操作是什么
    // 依赖接口而非实现
    void action(int a, int b)
    {
        _state->action(this, a, b); 
    }

    void change(int state)
    {
        // 以state为角标，改变_state指针具体指向
        // 从而无需StateManager类了解，每个State类
        // 对应的状态是什么，同时也简化StateManger的职责
        _state = _states[state]; 
#if 0
        switch (state)
        {
        case STATE_STATIC:
            _state = _state_static; 
            break; 
        case STATE_PROMOVE:
            _state = _state_prmove; 
            break; 
        }
#endif
    }

    // 可选，对外提供一个扩展（增加）State的方式
    // 也可以简单在构造函数中自己添加，但要注意
    // _state指针的初始指向问题
    void install(int state, State *state_impl)
    {
        // _states[state] = state_impl; 

        // 更好的方式是
        _states[state] = state_impl; 
    }
protected:
private:
    State *_state; 
    
    // State *_states[1024]; 
    // 更好的方式是用map做一个映射，这样最终：
    // 1. 无需提供一个固定大小的空间
    // 2. 无需State的常数表示必须连续且从0开始
    // 3. 无需一定要用常数表示状态，甚至可以用更可读的方式——字符串
    States _states; 
    
};

// 具象类，具体的State实现，承担两个职责：
// 1、“当前”（仅当前）状态下系统的操作，从而使得其他状态下
//    的操作无需关心，其他状态下的修改或扩展无需理会
// 2、“当前”（仅当前）状态下状态的迁移，也即只需关心与当前
//    状态有关的O(n)个关系。
// 从而，可以保证开发中，能够将注意力集中到当前状态下，避免
// 造成因职责过多而犯错。
class StateStatic : public State
{
public:
    virtual void action(StateManager *manager, int a, int b)
    {
        cout << "STATE_STATIC" << endl; 

        if (a == 3)
        {
            manager->change(StateManager::STATE_PROMOVE); 
        }
        else if (b == 2)
        {
            manager->change(StateManager::STATE_RUN); 
        }
    }
};

class StatePromove : public State
{
public:
    virtual void action(StateManager *manager, int a, int b)
    {
        cout << "STATE_PROMOVE" << endl; 

        if ((a & 1) == 1)
        {
            manager->change(StateManager::STATE_BRAKE); 
        }
        else if (a == 0 && (b & 1) == 0)
        {
            manager->change(StateManager::STATE_RUN); 
        }
    }
};


class StateRun : public State
{
public:
    virtual void action(StateManager *manager, int a, int b)
    {
        cout << "STATE_RUN" << endl; 

        if (a == 3 && b == 5)
        {
            manager->change(StateManager::STATE_BRAKE); 
        }
    }
};

class StateBrake : public State
{
public:
    virtual void action(StateManager *manager, int a, int b)
    {
        cout << "STATE_BRAKE" << endl; 

        // 实际系统中，可能有一些其他操作，例如，程序退出
        exit(0); 

        if (b == 1)
        {
   
            manager->change(StateManager::STATE_STATIC); 
        }

    }
};


int _tmain(int argc, _TCHAR* argv[])
{
    // Action action; 
    // action.action(3, 2); 
    // action.action(1, 5); 
    // action.action(0, 3); 
 
    StateManager manager; 
    manager.install(StateManager::STATE_STATIC,  new StateStatic); 
    manager.install(StateManager::STATE_PROMOVE, new StatePromove); 
    manager.install(StateManager::STATE_RUN,     new StateRun); 
    manager.install(StateManager::STATE_BRAKE,   new StateBrake); 
    // 如需添加新的状态：
    // 1、对系统而言，需增加一个专注于这个状态的具象类，继承自State
    // 2、对Manger而言，只需给他install一个新的状态，其他无需关心
    // 3、对其他State具象类而言，只需修改那些可能跳转到“新状态”的那些状态的实现。
    // 关于第3点，是否有其他方法避免修改？

    // 保证初始状态正确
    manager.change(StateManager::STATE_STATIC); 

    //
    while (1)
    {
        int a, b; 
        cin >> a >> b; 
        manager.action(a, b); 
    }
    
    system("PAUSE"); 
	return 0;
}

