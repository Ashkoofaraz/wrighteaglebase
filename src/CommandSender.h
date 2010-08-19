#ifndef __CommandSender_H__
#define __CommandSender_H__

#include "Thread.h"

class Observer;
class Agent;

class CommandSender: public Thread
{
public:
    /**
     * ���캯������������
     */
    CommandSender(Observer *pObserver);
    ~CommandSender();

    /**
     * ��ѭ������
     */
    void StartRoutine();
    void Run(char *msg = 0);
    void RegisterAgent(Agent *agent) { mpAgent = agent; };

private:
    Observer  *mpObserver;
    Agent     *mpAgent;
};

#endif

