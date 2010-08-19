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
	int  GetBallFreeCycleLeft() const { return mBallFreeCycleLeft; }
	bool IsTmKickable() const;

	bool IsMyPenaltyTaken() const;

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
	void	SetSituation(Situation S){mSituation = S;}

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
	Unum mController; //+��ʾ�Լ��˿���-��ʾ�Է�����0��ʾû�˿������˿��������free����һ���£���free��ָ��ǰ���˿����򣬵�������˲�һ��Ҫ�����򣬿��������ص�����ˣ�
	Unum mChallenger; //�Է����ߵ��� +

    bool mIsLastBallFree;
    Time mLastBallFreeTime;
    Unum mLastController; //��¼�ϴο�����ˣ��������ϸ����ڣ������ڸ���㣩��Ҳ�������ϸ��׶εģ�������new��infostate���
    Unum mLastChallenger;

	///�����ǿ���cycle_delay����С�������ڣ�������Ϊcycle_delayʱ�����Ѿ�������
	int mMyInterCycle;
	int mMinTmInterCycle; /*����ö��Ѳ������Լ�*/
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
	int mBallFreeCycleLeft; //ball_free���ܳ���������
	int mBallOutCycle; //��������������

	Situation mSituation;	//��ǰ�Ĺ�������

public:
	/** ����last behavior�Ľӿ� */
	/**
	 * ע��: ���õĽӿڶ�����������õģ�behavior*���治�ù�
	 * */

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

private:
	class VirtualSelf: public PlayerState {
	public:
		VirtualSelf(const PlayerState & player): PlayerState(player) {}
		const double & GetKickableArea() const { return ServerParam::instance().maxTackleArea(); }
	};
};

#endif
