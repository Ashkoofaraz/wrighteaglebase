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

#ifndef __Strategy_H__
#define __Strategy_H__

#include <vector>
#include "Types.h"
#include "Geometry.h"
#include "InterceptInfo.h"
#include "BehaviorBase.h"
#include "DecisionData.h"

class Agent;
class PlayerState;

/**
 * ���� Strategy �����Ϣ����ʱ��Ҫ�õ��Լ��ú��룬���Բ��ܷ��� InfoState ���棬�����Ƶ� Agent ��
 */
class Strategy: public DecisionData
{
public:
	Strategy(Agent & agent);
	virtual ~Strategy();

	void UpdateRoutine();

private:
	void StrategyAnalyze();
	void BallPossessionAnalyse();
	bool SetPlayAnalyse();
	void SituationAnalyse();
    void StateAnalyse();

public:
    /** Situation based strategy position **/
	Vector GetTeammateSBSPPosition(Unum t_num,const Vector& ballpos);

public:
	bool IsBallActuralKickable() const { return mIsBallActuralKickable;}
	bool IsBallFree() const { return mIsBallFree; }
	int GetBallFreeCycleLeft() const { return mBallFreeCycleLeft; }
	bool IsTmKickable() const;

	Unum GetController() const { return mController;  }
	void SetController(Unum controller) { mController = controller; }
    bool IsMyControl() const;
	bool IsTmControl() const { return mController > 0; }
	bool IsOppControl() const { return mController < 0; }
	bool IsBallOutOfControl() const { return mController == 0; }

    bool IsLastMyControl() const;
	bool IsLastTmControl() const { return mLastController > 0; }
	bool IsLastOppControl() const { return mLastController < 0; }
    bool IsLastBallFree() const { return mIsLastBallFree; }
    Unum GetLastController() const { return mLastController; }
    Unum GetLastChallenger() const { return mLastChallenger; }

    const Time & GetLastBallFreeTime() const { return mLastBallFreeTime; }

	const Situation & GetSituation() const { return mSituation; }
	void SetSituation(Situation S){mSituation = S;}

	const Vector & GetBallInterPos() const { return mBallInterPos; }

	int GetBallOutCycle() const { return mBallOutCycle; }

	int GetMyInterCycle() const { return mMyInterCycle; }
	void SetMyInterCycle(int cycle) { mMyInterCycle = cycle; }

	Vector GetMyInterPos();

	int GetMinOppInterCycle() const { return mMinOppInterCycle; }
	int GetMinTmInterCycle() const { return mMinTmInterCycle; }
	Unum GetFastestTm() const { return mFastestTm; }
	Unum GetFastestOpp() const { return mFastestOpp; }

	int GetSureOppInterCycle() const { return mSureOppInterCycle; }
	int GetSureTmInterCycle() const { return mSureTmInterCycle; }
	int GetSureInterCycle() const {return mSureInterCycle;}
	Unum GetSureTm() const { return mSureTm; }
	Unum GetSureOpp() const { return mSureOpp; }

private:
	bool mIsBallActuralKickable;//�Ƿ�ʵ�ʿ���
	bool mIsBallFree;
	int mBallFreeCycleLeft; //ball_free���ܳ���������

	Unum mController; //+��ʾ�Լ��˿���-��ʾ�Է�����0��ʾû�˿������˿��������free����һ���£���free��ָ��ǰ���˿����򣬵�������˲�һ��Ҫ�����򣬿��������ص�����ˣ�
	Unum mChallenger; //�Է����ߵ��� +

	bool mIsLastBallFree;
    Time mLastBallFreeTime;
    Unum mLastController; //��¼�ϴο�����ˣ��������ϸ����ڣ������ڸ���㣩��Ҳ�������ϸ��׶εģ�������new��infostate���
    Unum mLastChallenger;

	//�����ǿ���cycle_delay����С�������ڣ�������Ϊcycle_delayʱ�����Ѿ�������
	int mMyInterCycle;
	int mMinTmInterCycle; /*����Ķ��Ѳ������Լ�*/
	int mMinOppInterCycle;
	int mMinIntercCycle;
	Unum mFastestTm;
	Unum mFastestOpp;

	//�������ۺϿ�����������п��ܵĽ�������
	int mSureTmInterCycle;
	int mSureOppInterCycle;
	int mSureInterCycle;
	Unum mSureTm;
	Unum mSureOpp;

	Vector mBallInterPos; //�����Ƚ�ס�ĵط����������Լ��˺ͶԷ�ȥ�ص�
	int mBallOutCycle; //��������������

	Situation mSituation;	//��ǰ�Ĺ�������

private:
	bool mForbiddenDribble; //setplay����ʱ���ܴ���

public:
	bool IsForbidenDribble() const { return mForbiddenDribble; }
	void SetForbidenDribble(const bool & forbiden) { mForbiddenDribble = forbiden; }

/**
 * ����last behavior�Ľӿ�
 * ע��: ���õĽӿڶ�����������õģ�behavior*���治�ù�
 **/
public:
	/**
	 * �õ������ڱ����activebehavior��û����Ϊ��
	 * @param type
	 * @return
	 */
	ActiveBehavior *GetLastActiveBehavior(BehaviorType type) const;

	/**
	 * �õ�������ʵ��ִ�е�activebehavior
	 * @return
	 */
	ActiveBehavior *GetLastActiveBehaviorInAct() const { return mLastActiveBehavior[0]; }

	/**
	 * ���ر�����ִ�е���Ϊ -- ���Ҫ��DecisionTree::Decision���غ��������
	 * @return
	 */
	ActiveBehavior *GetActiveBehaviorInAct() const { return mActiveBehavior[0]; }

	/**
	 * �����������Ƿ�ִ����type���͵�activebehavior
	 * @param type
	 * @return
	 */
	bool IsLastActiveBehaviorInActOf(BehaviorType type) const {
		return GetLastActiveBehaviorInAct() && GetLastActiveBehaviorInAct()->GetType() == type;
	}

private:
	/**
	 * ����behavior*���߳�������activebehavior -- plan����ʱ����
	 * @param type
	 */
	void SaveActiveBehavior(const ActiveBehavior & beh);

	friend class DecisionTree;

	void SaveActiveBehaviorList(const std::list<ActiveBehavior> & behavior_list);

	/**
	 * ���ñ�����ʵ��ִ�е�activebehavior -- excuteʱ����
	 * @param type
	 */
	void SetActiveBehaviorInAct(BehaviorType type);

	void ResetSavedActiveBehavior();

private:
	Array<ActiveBehavior*, BT_Max, true> mActiveBehavior;
	Array<ActiveBehavior*, BT_Max, true> mLastActiveBehavior;

public:
    PlayerInterceptInfo mMyTackleInfo; //�����Լ��Ŀ��߷�Χ���󵽲������Ľ�����Ϣ

    Vector AdjustTargetForSetplay(Vector target);

private:
	class VirtualSelf: public PlayerState {
	public:
		VirtualSelf(const PlayerState & player): PlayerState(player) {}
		const double & GetKickableArea() const { return ServerParam::instance().maxTackleArea(); }
	};

/** ���ڵ���Ľӿں����� */
public:
	void SetPenaltyFirstStep(bool flag)    { mIsPenaltyFirstStep = flag; }
	bool IsPenaltyFirstStep() const        { return mIsPenaltyFirstStep; }
	Unum GetPenaltyTaker() const           { return mPenaltyTaker; }
	bool IsMyPenaltyTaken() const;

private:
	bool IsPenaltyPlayMode() const;
	void PenaltyAnalyze();

private:
	bool mIsPenaltyFirstStep; // ������setupʱ�õ�����
	int mPenaltySetupTime; // �ҷ��ڼ��η�����
	Unum mPenaltyTaker; // ��������ˣ����ַ�ʱΪ-1
};

#endif
