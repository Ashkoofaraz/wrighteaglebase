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

#ifndef __WORLDSTATE_H__
#define __WORLDSTATE_H__

#include <vector>
#include <cstdlib>
#include "Observer.h"
#include "PlayerState.h"
#include "BallState.h"
#include "CommunicateSystem.h"

class PlayerObserver;
class UnknownPlayerObserver;
class UnknownUnumPlayerObserver;

/**
 * pointҪ������Ƕ� -- һ��Լ��
 * Max point direction.
 */
#define MAX_POINT_DIR 120

class HistoryState;
class BeliefState;

/**
 * WorldState �����Ϣ���Լ��ú����޹أ�����������Ϣ�ö��ѺͶ������֣����Ա�����Ҳ���Լ��ı��޹�
 */
class WorldState {
    friend class WorldStateUpdater;
    WorldState(const WorldState &);

public:
    WorldState(HistoryState * history_state = 0);
    virtual ~WorldState() {}

     /**
     * ��Observer����WorldState
    * @param observer
     */
    void UpdateFromObserver(Observer *observer);

     /**
     * ��world_state���£��õ�һ�����������ʱ�õ�����״̬
    * @param world_state
     */
    void GetReverseFrom(WorldState *world_state);

    const PlayerState & GetPlayer(const Unum & i) const { return i > 0? mTeammate[i]: mOpponent[-i]; }
    const std::vector<PlayerState *> & GetPlayerList() const { return mPlayerList; }

    const BallState & GetBall() const { return mBall; }
    const Time & CurrentTime() const { return mCurrentTime; }
	void  SetCurrentTime(const Time & time) { mCurrentTime = time; }
	KickOffMode GetKickOffMode() const {return mKickOffMode;}

    PlayMode GetPlayMode() const { return mPlayMode; }
	PlayMode GetLastPlayMode() const { return mLastPlayMode;}
    const Time & GetPlayModeTime() const { return mPlayModeTime; }

	BallState & Ball() { return mBall; }
	PlayerState & Player(const Unum & i) {  return i > 0? mTeammate[i]: mOpponent[-i]; }
    PlayerState & Teammate(const Unum & i) {  return mTeammate[i]; }
    PlayerState & Opponent(const Unum & i) { return mOpponent[i]; }

    const PlayerState & GetTeammate(const Unum & i) const { return mTeammate[i]; }
    const PlayerState & GetOpponent(const Unum & i) const {  return mOpponent[i]; }

    Unum GetTeammateGoalieUnum() const { return mTeammateGoalieUnum; }
    Unum GetOpponentGoalieUnum() const { return mOpponentGoalieUnum; }

    int GetTeammateScore() const { return mTeammateScore; }
    int GetOpponentScore() const { return mOpponentScore; }

	/**
	 * get record history
	 * @param i stand for i cycles before ,if i = 0 ,it means that you get current worldstate
	 */
	WorldState * GetHistory(int i) const;

	/**
	 * get time cycle  before current time
	 * warning : this should not be used when you get WorldState from history  for example: GetHistory(1)->GetTimeBeforeCurrent(1) is not correct
	 * @param cycle should be >= 1 and <= HISTORYSIZE
	 */
	Time GetTimeBeforeCurrent(int cycle = 1) const;

	bool IsBallDropped() const { return mIsBallDropped; }

private:
	HistoryState * mpHistory;

    Time mCurrentTime;

	KickOffMode mKickOffMode;
    PlayMode mPlayMode;
	PlayMode mLastPlayMode;
	Time mPlayModeTime;
	bool mIsBallDropped;

    BallState mBall;
    PlayerArray<PlayerState> mTeammate;
    PlayerArray<PlayerState> mOpponent;
    std::vector<PlayerState *> mPlayerList;

    Unum mTeammateGoalieUnum;
    Unum mOpponentGoalieUnum;

	int mTeammateScore;
	int mOpponentScore;

	bool mIsCycleStopped;
};

/**
 * ����WorldState�ĸ���
 */
class WorldStateUpdater {
    WorldStateUpdater(const WorldStateUpdater &);

public:
    WorldStateUpdater(Observer *observer, WorldState *world_state):
    	mpObserver( observer ),
    	mpWorldState( world_state ),
    	mSelfSide( mpObserver? mpObserver->OurSide(): '?' ),
    	mSelfUnum( mpObserver? mpObserver->MyUnum(): 0 )
    {
		mBallConf = 1;
		mPlayerConf = 1;
		mSightDelay = 0;
		mIsHearBallPos = false;
		mIsHearBallVel = false;
    }

     /**
      * ��������
      */
    void Run();

private:
     /** һЩֻ�ڸ���ʱ�õĽӿ� */
    inline BallState   & Ball();
    inline PlayerState & Teammate(Unum i);
    inline PlayerState & Opponent(Unum i);
    inline PlayerState & SelfState();

	/**methods*/
    inline const BallState & GetBall();
    inline const PlayerState & GetTeammate(Unum i);
    inline const PlayerState & GetOpponent(Unum i);
    inline const PlayerState & GetSelf();
	inline AngleDeg  GetNeckGlobalDirFromSightDelay(int sight_delay = 0);
	inline Vector    GetSelfVelFromSightDelay(int sight_delay = 0);
    inline Unum GetSelfUnum();
    inline char GetSelfSide();

private:
    /** ���ָ��·��� */
	void UpdateWorldState();

private:
    /**
     * ��õ�ǰ�ɼ�������Ķ���
     * û���򷵻صĺ���Ϊ0
     */
    Unum GetSeenClosestTeammate();

    /**
     * ��õ�ǰ�ɼ�������Ķ���
     * û���򷵻صĺ���Ϊ0
     */
    Unum GetSeenClosestOpponent();

	/**
	 * get closest player
	 *  just for EvaluatePlayer
	 */
	Unum GetClosestOpponent();

	/**
	 * get closest player to ball
	 * @return is teammate ? > 0 : < 0
	 */
	Unum GetClosestPlayerToBall();


//=======================================�ڲ����µĽӿ�================================================
public:
    /** ����Action��Ҫ����Ϣ */
    void UpdateActionInfo();

private:
	/**update sight delay */
	void UpdateSightDelay();

    /** �����Լ���Ϣ */
    void UpdateSelfInfo();

    /** �����Լ���sense */
    void UpdateSelfSense();

    /** �������еĶ�Ա����Ϣ */
    void UpdateKnownPlayers();

    /** ����δ֪��Ա����Ϣ */
    void UpdateUnknownPlayers();

	bool UpdateMostSimilarPlayer(const Vector & pos ,int index);

    /** ����ĳһ���ض��Ķ�Ա */
    void UpdateSpecificPlayer(const PlayerObserver& player , Unum unum , bool is_teammate);

    /** ����ĳ���ض���δ֪����Ա */
    void UpdateSpecificUnknownPlayer(const UnknownPlayerObserver& player , Unum num , bool is_teammate);

    /** ���������Ϣ */
    void UpdateBallInfo();

    /** �Զ���delay��1 */
    void AutoDelay(int delay_add);

    /** �����ض���λ�ô��ض�״̬ */
    void UpdateInfoFromPlayMode();

    /** ���³��ϵ���Ϣ */
    void UpdateFieldInfo();

    /** ������������ */
    void UpdateFromAudio();

    /** ����collide���� */
    void UpdateInfoWhenCollided();
//=========================================���º�Ԥ��ļ��㹤��=============================================
private:
    /**�����Լ�ͷ�ĽǶ�*/
    bool ComputeSelfDir(double& angle);

    /**�����Լ���λ��*/
    bool ComputeSelfPos(Vector& vec , double& eps);

    /**������һ������*/
    bool ComputeNextCycle(MobileState& ms , double decay);

    double ComputePlayerMaxDist(const PlayerState& state);

    /**compute the player may see or not. just for update unknown player */
    bool ComputePlayerMaySeeOrNot(const PlayerState& state);

	/**compute conf from cycle*/
	double ComputeConf(double delay , int cycle);

	/** estimate to now*/
	void EstimateToNow();

      /** �����˶����ɺ�������ִ�еĶ�����ģ��һ���� */
    void EstimateWorld(bool is_estimate_to_now = false , int cycle = 0);

      /** Ԥ���Լ�����Ϣ */
    void EstimateSelf(bool is_estimate_to_now = false , int cycle = 0);

      /** Ԥ�������Ϣ */
    void EstimateBall(bool is_estimate_to_now = false , int cycle = 0);

      /** Ԥ�������Լ���������Ա����Ϣ */
    void EstimatePlayers();

	/**
	 * recompute conf
	 */
	void EvaluateConf();

	/**
	 * Evaluate ball forgotten
	 */
	void EvaluateBall();

	/**
	 * evaluate forget ball
	 */
	void EvaluateForgetBall(bool use_memory);

	/**
	 * evaluate player
	 */
	void EvaluatePlayer(PlayerState& player);

	bool ShouldSee(const Vector & pos); //��Ӧ�ÿ���/��֪����

	Vector GetNearSidePos(const Vector & pos, const Vector *expected_pos = 0);

	/**
	 * evaluate to forget player
	 */
	void EvaluateForgetPlayer(PlayerState& player);

	/**
	 * calc player go to point
	 */
	double CalcPlayerGotoPoint(PlayerState& player ,double dist);

	/**
	 * tell whether can see or not
	 */
	bool ComputeShouldSeeOrNot(Vector pos);

	void MaintainPlayerStamina();

	void MaintainConsistency();

	void UpdateOtherKick();
private:
    Observer * const mpObserver;
    WorldState * const mpWorldState;

    char mSelfSide;
    Unum mSelfUnum;

	double mPlayerConf;
	double mBallConf;
	int mSightDelay;

public:
	static const double KICKABLE_BUFFER;
	static const double CATCHABLE_BUFFER;

private:
	bool  mIsOtherKick;
	int   mOtherKickUnum; // >0 : teammate , < 0 opponent;

	bool  mIsOtherMayKick;

	bool mIsHearBallPos;
	bool mIsHearBallVel;

public:
    /** ������Ա�Ĳ���ɹ��ʣ���Ϊ��������£����Է�������ȽϺ� */
    double ComputeTackleProb(const Unum & unum, bool foul = false);
};

/**
 * ��������worldstate������ݣ��Թ�������ѻ���֣�������������������ָ�״̬
 */
/**
 * EXAMPLE:
 * void Behavior*Planner::Plan() {
 *      ActiveBehavior teammate_behavior;
 *
 *      { //����ָ��������ڵĴ��뷶Χ����ʹ�ָ���������
 *      	WorldStateSetter setter(mWorldState);
 *      	setter.Ball().UpdatePos(Vector(0, 0));
 *      	//... ...
 *          setter.IncStopTime(); //���Կ�ʼ������
 *      	Agent * agent = mAgent.CreateTeammateAgent(mStrategy.GetSureTm());
 *          ActiveBehaviorList bhv_list;
 *			{ //���ڱ�ʾPlanner�������������������Planner����������
 *			  //��˰�������Ϊ�����ȳ���Planner�ٳ���Agent
 *      		BehaviorPassPlanner(*agent).Plan(bhv_list);
 *      	}
 *          teammate_behavior = bhv_list.front();
 *          delete agent;
 *          //��������setter�����������ָ�����״̬
 *      }
 *
 *      //do_some_thing_about teammate_behavior
 *      //... ...
 * }
 *
 */
class WorldStateSetter {
public:
	WorldStateSetter (WorldState & world_state):
		mWorldState(world_state),
		mpBackupBallState(0),
		mBackupTime(mWorldState.CurrentTime())
	{
	}

	~WorldStateSetter() {
		mWorldState.SetCurrentTime(mBackupTime);
		if (mpBackupBallState != 0) {
			mWorldState.Ball() = *mpBackupBallState;
			delete mpBackupBallState;
		}
		for (int i = 0; i < TEAMSIZE * 2 + 1; ++i) {
			if (mpBackupPlayerState[i] != 0) { //������ݹ��ˣ����ڻָ���
				Assert(i != 0);
				if (i <= TEAMSIZE) {
					mWorldState.Teammate(i) = *mpBackupPlayerState[i];
					delete mpBackupPlayerState[i];
				}
				else {
					mWorldState.Opponent(i - TEAMSIZE) = *mpBackupPlayerState[i];
					delete mpBackupPlayerState[i];
				}
			}
		}
	}

	/**
	 * Ҫ������Ľӿ�����state�����ݣ������ܱ���
	 * @return
	 */
	BallState   & Ball() { if (pBall() == 0) pBall() = new BallState(mWorldState.Ball()); return mWorldState.Ball(); }
	PlayerState & Teammate(Unum i) { if (pTeammate(i) == 0) pTeammate(i) = new PlayerState(mWorldState.Teammate(i)); return mWorldState.Teammate(i); }
	PlayerState & Opponent(Unum i) { if (pOpponent(i) == 0) pOpponent(i) = new PlayerState(mWorldState.Opponent(i)); return mWorldState.Opponent(i); }

	void SetBallInfo(const Vector & pos, const Vector & vel) {
		Ball().UpdatePos(pos);
		Ball().UpdateVel(vel);
	}

	void SetTeammateInfo(const Unum & num, const Vector & pos, const AngleDeg & body_dir, const Vector & vel) {
		Teammate(num).UpdatePos(pos);
		Teammate(num).UpdateBodyDir(body_dir);
		Teammate(num).UpdateVel(vel);
	}

	void SetOpponentInfo(const Unum & num, const Vector & pos, const AngleDeg & body_dir, const Vector & vel) {
		Opponent(num).UpdatePos(pos);
		Opponent(num).UpdateBodyDir(body_dir);
		Opponent(num).UpdateVel(vel);
	}
	void IncStopTime() { WorldStateUpdater(0, & mWorldState).UpdateActionInfo(); mWorldState.SetCurrentTime(Time(mWorldState.CurrentTime().T(), mWorldState.CurrentTime().S() + 1)); }

private:
	BallState   *& pBall() { return mpBackupBallState; }
	PlayerState *& pTeammate(Unum i) { Assert(i >= 1 && i <= TEAMSIZE); return mpBackupPlayerState[i]; }
	PlayerState *& pOpponent(Unum i) { Assert(i >= 1 && i <= TEAMSIZE); return mpBackupPlayerState[TEAMSIZE + i]; }

private:
	WorldState & mWorldState;

	BallState   * mpBackupBallState;
	Array<PlayerState *, 1 + TEAMSIZE * 2, true> mpBackupPlayerState;
	Time mBackupTime;
};

/**��¼StateWorld��ʷ��Ϣ*/
class HistoryState
{
public:
    HistoryState(): mNum(0) {
    }

    enum {
    	HISTORY_SIZE = 10
    };

    /**����ǰ����������
     * @param ��ʵ���������
     */
    void UpdateHistory(const WorldState & world);

    /**���֮ǰ������
     * @param ȡֵ��ΧΪ1~HISTORYSIZE����������µ���ǰ������
     */
    WorldState *GetHistory(int num);

private:
    /**��¼StateWorld������*/
    Array<WorldState, HISTORY_SIZE> mRecord;

    /**��¼���鵱ǰ���ö�ǰһ���հ�*/
    int mNum;
};

#endif /* WORLDSTATE_H_ */
