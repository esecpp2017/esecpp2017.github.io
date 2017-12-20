// State.cpp : �������̨Ӧ�ó������ڵ㡣
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
// ���ص�ʵ�ַ�ʽ��Ҳ���������뵽��ʵ��
// Ҳ�п�����ԭ��ϵͳ��û����ô���ӣ�����ʱ������
// ��������ı䣬�����ݱ�ɵ�ǰ״̬
// ���ڵ������ǣ� 
// 1��Action���ְ������Ӵ󣬰���ά��״̬��ά��״̬Ǩ��
//    (״̬֮��O(n^2)��ϵ)��
//    ά��ÿһ��״̬�£�״̬Ǩ�ƵĹ��������Ӵ��ְ����
//    ��ʵ�Ĵ������Ʊ���һ���ܾ޴�Ĵ�����֮��Ӧ������Ƿ�
//    �ú����ָ��޹أ�����˼������ơ�ά������չ�漰����˼ά
//    �����йأ���˼���ָ��һϵ��С�ĺ�����Ҳͬ������Ϊְ��
//    �����Ӵ�ʹ��ÿһ����չ��ά������Ҫ�ܴ��Ͷ�롣
// 2���ƻ�����ԭ������չ�Կ��С�
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
        // ����Ļ�ζ������������ֺܶ�if + else��䣬
        // ���߳���switch case��䣬�����������д�����չ��
        // �ϰ����⡣
        // ��Ҳ����ζ�ţ�����Կ�����Ĵ����Ƿ���Ҫ��д����
        // ��׼ȷ��˵�����ع���
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
// һ��ΪState Pattern��д��汾

class StateManager; 
// ����״̬������Ľӿ��࣬���ṩһ���ӿڣ��ýӿ�
// �е����ݲ�ͬ״̬���в�ͬ������ְ�𣬽ӿ����֡�����
// �ȵȣ���ȫ���Ը���ϵͳ���������
class State
{
public:
    virtual void action(StateManager *manager, int a, int b) = 0; 

    // ��Ȼ�м̳д��ڣ������������麯������֤�ͷ�ʱ��ȷ����
    virtual ~State() { }
};

// �е�����ǰ״̬��ְ�𣬸���״̬ά����Ǩ��
class StateManager
{
public:
    // �����ṩһ�����ɶ�����״̬ʶ��ʽ
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

    // ֱ�ӵ���State���action������
    // �����ģ�Ҳ��֪�������������ʲô
    // �����ӿڶ���ʵ��
    void action(int a, int b)
    {
        _state->action(this, a, b); 
    }

    void change(int state)
    {
        // ��stateΪ�Ǳ꣬�ı�_stateָ�����ָ��
        // �Ӷ�����StateManager���˽⣬ÿ��State��
        // ��Ӧ��״̬��ʲô��ͬʱҲ��StateManger��ְ��
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

    // ��ѡ�������ṩһ����չ�����ӣ�State�ķ�ʽ
    // Ҳ���Լ��ڹ��캯�����Լ���ӣ���Ҫע��
    // _stateָ��ĳ�ʼָ������
    void install(int state, State *state_impl)
    {
        // _states[state] = state_impl; 

        // ���õķ�ʽ��
        _states[state] = state_impl; 
    }
protected:
private:
    State *_state; 
    
    // State *_states[1024]; 
    // ���õķ�ʽ����map��һ��ӳ�䣬�������գ�
    // 1. �����ṩһ���̶���С�Ŀռ�
    // 2. ����State�ĳ�����ʾ���������Ҵ�0��ʼ
    // 3. ����һ��Ҫ�ó�����ʾ״̬�����������ø��ɶ��ķ�ʽ�����ַ���
    States _states; 
    
};

// �����࣬�����Stateʵ�֣��е�����ְ��
// 1������ǰ��������ǰ��״̬��ϵͳ�Ĳ������Ӷ�ʹ������״̬��
//    �Ĳ���������ģ�����״̬�µ��޸Ļ���չ�������
// 2������ǰ��������ǰ��״̬��״̬��Ǩ�ƣ�Ҳ��ֻ������뵱ǰ
//    ״̬�йص�O(n)����ϵ��
// �Ӷ������Ա�֤�����У��ܹ���ע�������е���ǰ״̬�£�����
// �����ְ����������
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

        // ʵ��ϵͳ�У�������һЩ�������������磬�����˳�
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
    // ��������µ�״̬��
    // 1����ϵͳ���ԣ�������һ��רע�����״̬�ľ����࣬�̳���State
    // 2����Manger���ԣ�ֻ�����installһ���µ�״̬�������������
    // 3��������State��������ԣ�ֻ���޸���Щ������ת������״̬������Щ״̬��ʵ�֡�
    // ���ڵ�3�㣬�Ƿ����������������޸ģ�

    // ��֤��ʼ״̬��ȷ
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

