/************************************************************************************
 * WrightEagle (Soccer Simulation League 2D)                                        *
 * BASE SOURCE CODE RELEASE 2010                                                    *
 * Copyright (c) 1998-2010 WrightEagle 2D Soccer Simulation Team,                   *
 *                         Multi-Agent Systems Lab.,                                *
 *                         School of Computer Science and Technology,               *
 *                         University of Science and Technology of China            *
 * All rights reserved.                                                             *
 *                                                                                  *
 * Redistribution and use in source and binary forms, with or without               *
 * modification, are permitted provided that the following conditions are met:      *
 *     * Redistributions of source code must retain the above copyright             *
 *       notice, this list of conditions and the following disclaimer.              *
 *     * Redistributions in binary form must reproduce the above copyright          *
 *       notice, this list of conditions and the following disclaimer in the        *
 *       documentation and/or other materials provided with the distribution.       *
 *     * Neither the name of the WrightEagle 2D Soccer Simulation Team nor the      *
 *       names of its contributors may be used to endorse or promote products       *
 *       derived from this software without specific prior written permission.      *
 *                                                                                  *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND  *
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED    *
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           *
 * DISCLAIMED. IN NO EVENT SHALL WrightEagle 2D Soccer Simulation Team BE LIABLE    *
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL       *
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR       *
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER       *
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,    *
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF *
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                *
 ************************************************************************************/

#ifndef BEHAVIORBASE_H_
#define BEHAVIORBASE_H_

#include <list>
#include <string>
#include "Geometry.h"
#include "ActionEffector.h"
#include "Formation.h"

class WorldState;
class BallState;
class PlayerState;
class InfoState;
class PositionInfo;
class InterceptInfo;
class Strategy;
class Analyser;
class Agent;

enum BehaviorType {
	BT_None,

	BT_Penalty,
	BT_Goalie,
	BT_Setplay,
	BT_Position,
	BT_Dribble,
	BT_Pass,
	BT_Shoot,
	BT_Intercept,
	BT_Formation,
	BT_Block,
	BT_Mark,

	BT_Max
};


enum BehaviorDetailType {
	BDT_None,

	BDT_Intercept_Normal,
    BDT_Pass_Ahead,
    BDT_Pass_Direct,
	BDT_Dribble_Fast,
	BDT_Dribble_Normal,
    BDT_Position_Normal,
    BDT_Position_Rush,
	BDT_Shoot_Tackle,
    BDT_Goalie_Position,
    BDT_Goalie_Catch,
    BDT_Goalie_Tackle,
    BDT_Setplay_Move,
    BDT_Setplay_Scan,
    BDT_Setplay_GetBall
};

class BehaviorExecutable;

class BehaviorFactory {
	BehaviorFactory();
	virtual ~BehaviorFactory();

public:
	typedef BehaviorExecutable * (*BehaviorCreator)(Agent & agent);
	static BehaviorFactory & instance();

	/**
	* ������Ϊ
	* @param agent
	* @param type
	* @return
	*/
	BehaviorExecutable * CreateBehavior(Agent & agent, BehaviorType type);

	/**
	* ע����Ϊ����������ֻ�о���ִ��Ч������Ϊ��Ҫע�ᣬ���� BehaviorAttackPlanner, BehaviorDefensePlanner ����Ҫע��
	* @param type ��Ϊ����
	* @param creator ��������
	* @return
	*/
	bool RegisterBehavior(BehaviorType type, BehaviorCreator creator, const char *behavior_name);

	const char *GetBehaviorName(BehaviorType type) {
		return mNameMap[type].c_str();
	}

private:
	Array<BehaviorCreator, BT_Max, true> mCreatorMap;
	Array<std::string, BT_Max> mNameMap;
};

class ActiveBehavior
{
	/** behavior type and agent pointer */
	BehaviorType mType;
	Agent * mpAgent;

public:

	ActiveBehavior(Agent & agent, BehaviorType type, BehaviorDetailType detail_type = BDT_None) :
		mType(type), mpAgent(&agent), mEvaluation(0.0),
		mKickCycle(0), mAngle(0), mTarget(Vector(0.0, 0.0)),
		mPower(0.0), mDistance(0.0), mKickSpeed(0.0),
		mFoul(false), mDetailType(detail_type),
		mBuffer(0.0)
	{

	}

	void Clear() {
		mEvaluation = 0.0;
	}

	const BehaviorType & GetType() const {
		return mType;
	}
	void SetType(const BehaviorType &type) {
		mType = type;
	}

	Agent & GetAgent() const {
		return *mpAgent;
	}

	bool Execute(); //��������ָ�� behavior ��ָ�룬��Ҫ�� BehaviorType ����
	void SubmitVisualRequest(double plus);

	bool operator>(const ActiveBehavior & ab) const {
		return this->mEvaluation > ab.mEvaluation;
	}
	bool operator ==(const ActiveBehavior & ab) const {
		return this->mEvaluation == ab.mEvaluation;
	}
	bool operator <(const ActiveBehavior & ab) const {
		return this->mEvaluation < ab.mEvaluation;
	}

	double mEvaluation;

	//behavior detail
	int mKickCycle; // ��������ڣ����ܶ������
	AngleDeg mAngle; // �Ƕ�
	Vector mTarget; // Ŀ����λ�ã������ǰ����ߵ��õ���Լ��ܵ��õ��
	double mPower; // dash��power
	double mDistance; // Ŀ��㵽�Լ��ľ���
	double mKickSpeed; // kick������
	bool mFoul; //�Ƿ�Ҫ���ⷸ�����

	KeyPlayerInfo mKeyTm; // teammate
	KeyPlayerInfo mKeyOpp; // opponent
	KeyPlayerInfo mKeyOppGB; // opponent to get ball
	KeyPlayerInfo mKeyOppGT; // opponent to go through

	BehaviorDetailType mDetailType; //ϸ������
	double mBuffer; //��Щ��Ϊִ��ʱ��buffer����planʱ��õģ�Ҫ�ȴ浽���������
};

class BehaviorAttackData {
public:
	BehaviorAttackData(Agent & agent);
	virtual ~BehaviorAttackData();

	Agent & mAgent;

	const WorldState & mWorldState;

	const BallState & mBallState;
	const PlayerState & mSelfState;

	PositionInfo & mPositionInfo;
	InterceptInfo & mInterceptInfo;

	Strategy & mStrategy;

    Formation & mFormation;
};

class BehaviorDefenseData: public BehaviorAttackData {
public:
	BehaviorDefenseData(Agent & agent);
	virtual ~BehaviorDefenseData();

	Analyser & mAnalyser;
};

template < class BehaviorDataType >
class BehaviorPlannerBase: public BehaviorDataType {
	BehaviorPlannerBase(const BehaviorPlannerBase &);

public:
	BehaviorPlannerBase(Agent & agent): BehaviorDataType(agent) {}
	virtual ~BehaviorPlannerBase() {}

	/**
	* �����ߣ�������õ�ActiveBehavior���浽behavior_list����
	*/
	virtual void Plan(std::list<ActiveBehavior> & behavior_list) = 0;

public:
	const std::list<ActiveBehavior> & GetActiveBehaviorList() {
		return mActiveBehaviorList;
	}

protected:
	std::list<ActiveBehavior> mActiveBehaviorList; // record the active behaviors for each high level behavior
};

class BehaviorExecutable {
	BehaviorExecutable(const BehaviorExecutable&);

public:
	BehaviorExecutable() {}
	virtual ~BehaviorExecutable() {}

	/**
	* ִ�� ActiveBehavior
	* @param act ActiveBehavior
	* @return
	*/
	virtual bool Execute(const ActiveBehavior & act_bhv) = 0; //ÿ����ֻ��һ����Ϊ��ִ��
	virtual void SubmitVisualRequest(const ActiveBehavior & act_bhv, double plus = 0.0) { //�������ж����Ϊ�ύ�Ӿ�����
		(void) act_bhv;
		(void) plus;
	}

	/// behavior factory interfaces
	template <class BehaviorDerived >
	static BehaviorExecutable* Creator(Agent & agent) { return new BehaviorDerived(agent); }

	template<class BehaviorDerived>
	static bool AutoRegister() {
		std::string behavior_name;

#ifndef WIN32
		std::string function_name = __PRETTY_FUNCTION__;

		size_t end = function_name.find("Executer");
		if (end != std::string::npos) {
			size_t begin = end;
			while (begin && isalpha(function_name[begin--])) ;
			begin += 2;
			if (begin < end) {
				behavior_name = function_name.substr(begin, end - begin);
			}
		}
#endif

		return BehaviorFactory::instance().RegisterBehavior(
				BehaviorDerived::BEHAVIOR_TYPE, Creator<BehaviorDerived>, behavior_name.c_str());
	}
};

template < class BehaviorDataType >
class BehaviorExecuterBase: public BehaviorDataType, public BehaviorExecutable {
	BehaviorExecuterBase(const BehaviorExecuterBase &);

public:
	BehaviorExecuterBase(Agent & agent): BehaviorDataType(agent) {}
	virtual ~BehaviorExecuterBase() {}
};


typedef std::list<ActiveBehavior> ActiveBehaviorList;
typedef std::list<ActiveBehavior>::iterator ActiveBehaviorPtr;

#define TeammateFormationTactic(TacticName) (*(FormationTactic##TacticName *)mFormation.GetTeammateTactic(FTT_##TacticName))
#define OpponentFormationTactic(TacticName) (*(FormationTactic##TacticName *)mFormation.GetOpponentTactic(FTT_##TacticName))

#endif /* BEHAVIORBASE_H_ */
