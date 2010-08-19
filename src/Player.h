#ifndef __Player_H__
#define __Player_H__

#include "Client.h"

class DecisionTree;
class  BeliefState;

class Player: public Client
{
	DecisionTree *mpDecisionTree;

public:
    /**
     * ���캯������������
     */
    Player();
    ~Player();

    void Run();
    void SendOptionToServer();
};

#endif

