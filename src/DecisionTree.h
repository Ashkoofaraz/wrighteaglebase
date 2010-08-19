#ifndef DECISIONTREE_H_
#define DECISIONTREE_H_

#include <list>
#include "BehaviorBase.h"

class Agent;

class DecisionTree {
public:
	DecisionTree() {}
	virtual ~DecisionTree() {}

	/**
	 * ������
	 * @param agent
	 */
	bool Decision(Agent & agent);

private:
	/**
	* ��������������ɶԣ�״̬��������������
	* @param agent ��ǰ�ڵ�ľֲ�����
	* @param step ��ǰ�ڵ����ȣ���������Ƿ����������أ�step = 1�����һ�㣬step = max_step �ǵ�һ�㣩
	* @return
	*/
	ActiveBehavior Search(Agent & agent, int step);

	ActiveBehavior GetBestActiveBehavior(Agent & agent, std::list<ActiveBehavior> & behavior_list);

	template <typename BehaviorDerived>
	bool MutexPlan(Agent & agent, std::list<ActiveBehavior> & active_behavior_list){
		BehaviorDerived(agent).Plan(active_behavior_list);
		return !active_behavior_list.empty();
	}
};

#endif /* DECISIONTREE_H_ */
