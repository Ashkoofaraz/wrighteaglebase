#ifndef AGENT_H_
#define AGENT_H_

#include "ActionEffector.h"
#include "WorldState.h"
#include "InfoState.h"
#include "PositionInfo.h"
#include "Strategy.h"
#include "Analyser.h"
#include "Formation.h"

class WorldModel;

/**
 * Identifies an agent.
 */
struct AgentID {
	Unum     mAgentUnum; //����Ϊ��
	Time     mCurrentTime;
	bool     mReverse;

	AgentID(Unum unum = 0, Time time = Time(-3, 0), bool reverse = false): mAgentUnum(unum), mCurrentTime(time), mReverse(reverse){}
	bool operator == (const AgentID & o){
		return mCurrentTime == o.mCurrentTime && mAgentUnum == o.mAgentUnum && mReverse == o.mReverse;
	}
	bool operator != (const AgentID & o){
		return !(*this == o);
	}

	friend std::ostream & operator<<(std::ostream & os, const AgentID & o) {
		if (o.mReverse) {
			return os << '-' << o.mAgentUnum;
		}
		else {
			return os << '+' << o.mAgentUnum;
		}
	}
};

/**
 * ��������Աͨ��
 */
class Agent
{
	friend class Client;

	Agent(Agent &);
	Agent(Unum unum, WorldModel *world_model, bool reverse);

public:
	virtual ~Agent();

	/**
	 * Interface to create an agent which represents a team mate.
	 */
	Agent * CreateTeammateAgent(Unum unum); ///�������

	/**
	 * Interface to create an agent which represents an opponent.
	 */
	Agent * CreateOpponentAgent(Unum unum); ///�������

	/**
	 * Interfaces to get the agent's world state.
	 */
	WorldState       & World() { return *mpWorldState; }
	const WorldState & GetWorldState() const { return *mpWorldState; }

	/**
	 * Interfaces to get the agent's info state.
	 */
	InfoState        & Info() {	return *mpInfoState; }
	const InfoState  & GetInfoState() const { return *mpInfoState; }

	/**
	 * �Լ���صĽӿ�
	 * Interfaces to get information about the agent it self.
	 */
	AgentID             GetAgentID() const { return AgentID(mSelfUnum, GetWorldState().CurrentTime(), mReverse); }
	Unum                GetSelfUnum() const { return mSelfUnum; }

	const PlayerState & GetSelf() const {
		if (!mSelfUnum) {
			static PlayerState coach; //dummy player state for coach
			return coach;
		}

		return GetWorldState().GetTeammate(mSelfUnum);
	}

	PlayerState & Self() {
		if (!mSelfUnum) {
			static PlayerState coach; //dummy player state for coach
			return coach;
		}

		return World().Teammate(mSelfUnum);
	}

	/**
	 * Get state information after queued actions are executed.
	 */
	Vector GetSelfPosWithQueuedActions() { return GetActionEffector().GetSelfPosWithQueuedActions(); }
	Vector GetSelfVelWithQueuedActions() { return GetActionEffector().GetSelfVelWithQueuedActions(); }
	AngleDeg GetSelfBodyDirWithQueuedActions() { return GetActionEffector().GetSelfBodyDirWithQueuedActions(); }
	Vector GetBallPosWithQueuedActions() { return GetActionEffector().GetBallPosWithQueuedActions(); }
	Vector GetBallVelWithQueuedActions() { return GetActionEffector().GetBallVelWithQueuedActions(); }

	/**
	 * Action interfaces
	 */
	bool Turn(AngleDeg angle) { return GetActionEffector().SetTurnAction(angle); }
	bool Dash(double power, AngleDeg dir) { return GetActionEffector().SetDashAction(power, dir); }
	bool TurnNeck(AngleDeg angle) { return GetActionEffector().SetTurnNeckAction(angle); }
	bool Say(std::string msg) { return GetActionEffector().SetSayAction(msg); }
	bool Attentionto(Unum num) { return GetActionEffector().SetAttentiontoAction(num); }
	bool AttentiontoOff() { return GetActionEffector().SetAttentiontoOffAction(); }
	bool Kick(double power, AngleDeg angle) { return GetActionEffector().SetKickAction(power, angle); }
	bool Tackle(AngleDeg angle, const bool foul) { return GetActionEffector().SetTackleAction(angle, foul); }
	bool Pointto(double dist, AngleDeg angle) { return GetActionEffector().SetPointtoAction(dist, angle); }
	bool PointtoOff() { return GetActionEffector().SetPointtoOffAction(); }
	bool Catch(AngleDeg angle) { return GetActionEffector().SetCatchAction(angle); }
	bool Move(Vector pos) { return GetActionEffector().SetMoveAction(pos); }
	bool ChangeView(ViewWidth view_width) { return GetActionEffector().SetChangeViewAction(view_width); }
	bool Compression(int level) { return GetActionEffector().SetCompressionAction(level); }
	bool SenseBody() { return GetActionEffector().SetSenseBodyAction(); }
	bool Score() { return GetActionEffector().SetScoreAction(); }
	bool Bye() { return GetActionEffector().SetByeAction(); }
	bool Done() { return GetActionEffector().SetDoneAction(); }
	bool Clang(int min_ver, int max_ver) { return GetActionEffector().SetClangAction(min_ver, max_ver); }
	bool EarOn(bool our_side, EarMode ear_mode = EM_All) { return GetActionEffector().SetEarOnAction(our_side, ear_mode); }
	bool EarOff(bool our_side, EarMode ear_mode = EM_All) { return GetActionEffector().SetEarOffAction(our_side, ear_mode); }
	bool SynchSee() { return GetActionEffector().SetSynchSeeAction(); }
	bool ChangePlayerType(Unum num, int player_type) { return GetActionEffector().SetChangePlayerTypeAction(num, player_type); }

	/**
	 * Get ActionEffector of this agent.
	 */
	ActionEffector & GetActionEffector() {
		if (mpActionEffector == 0){
			mpActionEffector = new ActionEffector(*this);
		}
		return *mpActionEffector;
	}

	/**
	 * Get Formation of this agent.
	 */
	Formation & GetFormation()
    {
        if (mpFormation == 0)
        {
            mpFormation = new Formation(*this);
        }
		return *mpFormation;
	}


private:
	/**
	 * Interface template for getting certain type of decision data.
	 */
	template <class DecisionDataDerived>
	DecisionDataDerived & GetDecisionData(DecisionDataDerived ** xptr)
	{
		if (*xptr == 0) {
			*xptr = new DecisionDataDerived(*this);
		}
		(*xptr)->Update();
		return **xptr;
	}

public:
	/**
	 * ÿ�ε��õ�ʱ�򣬼���Ƿ���Ҫ���£�����ʹ��ʱ������ȸ�ֵ�������������ȽϿ�
	 * Each the following methods will check if an update should be performed first when it is called.
	 * It's better to store the returned value in a variable when multiple uses are needed.
	 */
	Strategy & GetStrategy() { return GetDecisionData(& mpStrategy); }
	Analyser & GetAnalyser() { return GetDecisionData(& mpAnalyser); }

public:
    /**
     * Check commands sent to server, based on ActionEffector::CheckCommands. Will update IsNewSight and BallSeenTime.
     */
	void CheckCommands(Observer *observer) { mIsNewSight = observer->IsNewSight();mBallSeenTime = observer->Ball().GetDist().time(); GetActionEffector().CheckCommands(observer); }

	/**
	 * Interface to ActionEffector::SendCommands.
	 */
	void SendCommands(char *msg) { GetActionEffector().SendCommands(msg); }

	/**
	 * If there's a new sight arrived in current cycle.
	 */
	bool IsNewSight() const { return mIsNewSight; }

	/**
	 * Get last time the agent saw the ball.
	 */
	Time GetBallSeenTime() const { return mBallSeenTime; }

public:
	/**
	 * If this agent is created for reverse calculation.
	 */
	bool IsReverse() const { return mReverse; }

private:
	const Unum mSelfUnum; //Agent�ĺ�������Ϊ��
	const bool mReverse;  //������Agent�Ƿ������
	WorldModel * const mpWorldModel;
	WorldState * const mpWorldState;

	/** ���ϱ����� Agent �������������ǲ����ģ�������ʽ�ķ��㣨����������ѺͶ��֣���Ҫ new һ�� Agent */

	InfoState  * mpInfoState;

	bool mIsNewSight;
	Time mBallSeenTime;

	/** ���±����ڵ�һ��ʹ��ʱ����ָ��ʵ������ÿ�ε���ʱ����Ƿ���Ҫ���� */
	Strategy * mpStrategy;
	Analyser * mpAnalyser;

    ActionEffector * mpActionEffector;
    Formation * mpFormation;
};

#endif /* AGENT_H_ */
