/************************************************************************************
 * WrightEagle (Soccer Simulation League 2D)                                        *
 * BASE SOURCE CODE RELEASE 2010                                                    *
 * Copyright (C) 1998-2010 WrightEagle 2D Soccer Simulation Team,                   *
 *                         Multi-Agent Systems Lab.,                                *
 *                         School of Computer Science and Technology,               *
 *                         University of Science and Technology of China, China.    *
 *                                                                                  *
 * This program is free software; you can redistribute it and/or                    *
 * modify it under the terms of the GNU General Public License                      *
 * as published by the Free Software Foundation; either version 2                   *
 * of the License, or (at your option) any later version.                           *
 *                                                                                  *
 * This program is distributed in the hope that it will be useful,                  *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    *
 * GNU General Public License for more details.                                     *
 *                                                                                  *
 * You should have received a copy of the GNU General Public License                *
 * along with this program; if not, write to the Free Software                      *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.  *
 ************************************************************************************/

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
