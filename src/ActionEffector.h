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

#ifndef ACTIONEFFECTOR_H_
#define ACTIONEFFECTOR_H_

#include "Observer.h"
#include "BasicCommand.h"
#include "ServerParam.h"
#include "PlayerParam.h"
#include <list>
#include <deque>
#include <ostream>

class WorldState;
class PlayerState;
class BallState;

class Agent;


/**
 * ͨ��ʵ��ת��Ƕȼ�����Ҫ��server���͵�turn����Ĳ�����������ת��Ƕ�
 * Calculate angle which should be sent so server as parameter when doing turn action.
 * \param actual_turn_angle angle the player what to turn.
 * \param player_type hetero type of the player.
 * \param player_speed current speed of the player.
 * \return angle which should be sent so server as parameter.
 */
inline AngleDeg GetTurnMoment(const AngleDeg actual_turn_angle, const int player_type, const double player_speed)
{
	return (actual_turn_angle * (1.0 +
			PlayerParam::instance().HeteroPlayer(player_type).inertiaMoment() * player_speed));
}

/**
 * ͨ��ʵ��ת��Ƕȼ�����Ҫ��server���͵�turn����Ĳ�����������ת��Ƕ�
 * Calculate angle the player actually turns from the angle parameter sent to server.
 * \param virtual_turn_angle the angle parameter sent to server.
 * \param player_type hetero type of the player.
 * \param player_speed current speed of the player.
 * \return angle the player actually will turn.
 */
inline AngleDeg GetTurnAngle(const AngleDeg moment, const int player_type, const double player_speed)
{
	return (GetNormalizeMoment(moment) / (1.0 +
			PlayerParam::instance().HeteroPlayer(player_type).inertiaMoment() * player_speed));
}

/**
 * �õ���ǰ״̬�¿���ת������Ƕ�
 * Get maximum angle the player can turn with certain state.
 * \param player_type hetero type of the player.
 * \param player_speed current speed of the player.
 * \return maximum angle the player can turn with certain state.
 */
inline AngleDeg GetMaxTurnAngle(const int player_type, const double player_speed)
{
	return (ServerParam::instance().maxMoment() / (1.0 +
			PlayerParam::instance().HeteroPlayer(player_type).inertiaMoment() * player_speed));
}

/**
 * �õ�˳ʱ��ת���ӵ����Ƕȣ��϶�Ϊ��ֵ�������Ĳ��ӽǶ��������
 * Get maximum angle the player can turn neck by clockwise.
 * \param player_head_angle current neck angle of the player, relative to body angle.
 * \return maximum angle the player can turn neck by.
 */
inline AngleDeg GetMaxTurnNeckAngle(const AngleDeg player_head_angle)
{
	return (ServerParam::instance().maxNeckAngle() - player_head_angle);
}

/**
 * �õ�˳ʱ��ת���ӵ����Ƕȣ��϶�Ϊ��ֵ�������Ĳ��ӽǶ��������
 * Get maximum angle the player can turn neck by counter-clockwise.
 * \param player_head_angle current neck angle of the player, relative to body angle.
 * \return maximum angle the player can turn neck by.
 */
inline AngleDeg GetMinTurnNeckAngle(const AngleDeg player_head_angle)
{
	return (ServerParam::instance().minNeckAngle() - player_head_angle);
}

/**
 * �õ�kick_rate��ע�������ball_2_player�����������Ա��������꣬����Ա���巽��Ϊx������
 * Get kick_rate by relative position of the ball.
 * \param ball_2_player ball position in the player's coordinate system.
 * \param player_type hetero type of the player.
 * \return kick_rate.
 */
inline double GetKickRate(const Vector & ball_2_player, const int player_type)
{
	double dir_diff = fabs(ball_2_player.Dir());
	double dist_ball = ball_2_player.Mod() - PlayerParam::instance().HeteroPlayer(player_type).playerSize() - ServerParam::instance().ballSize();
	return PlayerParam::instance().HeteroPlayer(player_type).kickPowerRate() *
			(1.0 - 0.25 * dir_diff / 180.0 -
					0.25 * dist_ball / PlayerParam::instance().HeteroPlayer(player_type).kickableMargin());
}

/**
 * �õ�kick�������ball_2_player�����溯��һ��Ϊ��ԣ�ball_velΪ��ľ����ٶ�
 * Get maximum random error by relative position and absolute velocity.
 * \param ball_2_player ball position in the player's coordinate system.
 * \param ball_vel velocity of the ball in field coordinate system.
 * \param player_type hetero type of the player.
 * \param kick_power power used by kick action.
 * \return maximum random error in this state.
 */
inline double GetMaxKickRand(const Vector & ball_2_player, const Vector & ball_vel, const int player_type, const double kick_power)
{
	double dir_diff = fabs(ball_2_player.Dir());
	double dist_ball = ball_2_player.Mod() - PlayerParam::instance().HeteroPlayer(player_type).playerSize() - ServerParam::instance().ballSize();

	double pos_rate = 0.5 + 0.25 * (dir_diff / 180.0 +
			dist_ball / PlayerParam::instance().HeteroPlayer(player_type).kickableMargin());

	double speed_rate = 0.5 + 0.5 * (ball_vel.Mod() /
			(ServerParam::instance().ballSpeedMax() * ServerParam::instance().ballDecay()));

	return (PlayerParam::instance().HeteroPlayer(player_type).kickRand() *
			(kick_power / ServerParam::instance().maxPower()) *
			(pos_rate + speed_rate));
}

/**
 * �õ�tackle�ĸ��ʣ�ע�������ball_2_player�����������Ա��������꣬����Ա���巽��Ϊx������
 * Get tackle success probability.
 * \param ball_2_player ball position in the player's coordinate system.
 * \return tackle success probability.
 */
inline double GetTackleProb(const Vector & ball_2_player, const bool foul)
{
	double tackle_dist = (ball_2_player.X() > 0.06 /*this is buffer*/ ?
        ServerParam::instance().tackleDist() :
        ServerParam::instance().tackleBackDist());

	if (fabs(tackle_dist) < FLOAT_EPS) {
		return 0.0;
	}
	else {
		double dx = fabs(ball_2_player.X()) / tackle_dist;
		double dy = fabs(ball_2_player.Y()) / ServerParam::instance().tackleWidth();

		if (dx > 1.0 || dy > 1.0 || ball_2_player.Mod() > ServerParam::instance().maxTackleArea()) {
			return 0.0;
		}

		double exponent = ( foul ) ? ServerParam::instance().foulExponent() : ServerParam::instance().tackleExponent();
		// tackle failure probability
		double prob = pow(dx, exponent) + pow(dy, exponent);
		prob = MinMax(0.0, prob, 1.0);
		return 1.0 - prob;
	}
}

inline double GetMaxTackleRand(const Vector & ball_2_player, const Vector & ball_vel, const int player_type, const bool & foul)
{
	double prob = 1.0 - GetTackleProb(ball_2_player, foul);
	double pos_rate =  0.5 + 0.5 * prob;

	double speed_rate = 0.5 + 0.5 * (ball_vel.Mod() /
			(ServerParam::instance().ballSpeedMax() * ServerParam::instance().ballDecay()));

	return (PlayerParam::instance().HeteroPlayer(player_type).kickRand()
			* ServerParam::instance().tackleRandFactor()
			* (pos_rate + speed_rate));
}

/**
 * ���� max_rand
 * @param prob ����ɹ���
 * @param ball_vel ��ĵ�ǰ�ٶ�
 * @param player_type ��Ա�칹
 * @return
 */
inline double GetMaxTackleRand(const double & prob, const Vector & ball_vel, const int player_type)
{
	double pos_rate =  0.5 + 0.5 * prob;

	double speed_rate = 0.5 + 0.5 * (ball_vel.Mod() /
			(ServerParam::instance().ballSpeedMax() * ServerParam::instance().ballDecay()));

	return (PlayerParam::instance().HeteroPlayer(player_type).kickRand()
			* ServerParam::instance().tackleRandFactor()
			* (pos_rate + speed_rate));
}

/**
 * �õ�tackle�ĸ��ʣ�������Ϊ��������
 * Get tackle success probability with all parameters are absolute values.
 * \param ball_pos.
 * \prram player_pos.
 * \param player_body_dir
 *
 */
inline double GetTackleProb(const Vector & ball_pos, const Vector & player_pos, const AngleDeg & player_body_dir, const bool & foul)
{
    Vector ball_2_player = (ball_pos - player_pos).Rotate(-player_body_dir);
    return GetTackleProb(ball_2_player, foul);
}


inline double GetDashDirRate(double dir)
{
	dir = GetNormalizeAngleDeg(dir);

	if ( ServerParam::instance().dashAngleStep() < FLOAT_EPS ) {
		// players can dash in any direction.
	}
	else {
		// The dash direction is discretized by server::dash_angle_step
		dir = ServerParam::instance().dashAngleStep() * Rint( dir / ServerParam::instance().dashAngleStep() );
	}

	double dir_rate = ( std::fabs( dir ) > 90.0
			? ServerParam::instance().backDashRate() - ( ( ServerParam::instance().backDashRate() - ServerParam::instance().sideDashRate() )
					* ( 1.0 - ( std::fabs( dir ) - 90.0 ) / 90.0 ) )
					: ServerParam::instance().sideDashRate() + ( ( 1.0 - ServerParam::instance().sideDashRate() )
							* ( 1.0 - std::fabs( dir ) / 90.0 ) )
	);
	dir_rate = MinMax( 0.0, dir_rate, 1.0 );

	return dir_rate;
}

/**
 * ���ø�ʡ�����ĵ�Ч��dash��ʽ
 * @param dash_power
 * @param dash_dir
 */
inline void TransformDash(double & dash_power, AngleDeg & dash_dir)
{
    if (dash_power < 0.0) {
    	double anti_dash_power = -dash_power * GetDashDirRate(dash_dir) / GetDashDirRate(dash_dir + 180.0);
    	if (anti_dash_power < ServerParam::instance().maxDashPower()) {
    		dash_dir = GetNormalizeAngleDeg(dash_dir + 180.0);
    		dash_power = anti_dash_power;
    	}
    }
}

/**
 * Action������
 * Input of an Action.
 */
struct ActionInput
{
	int     mPlayerType;
	Vector  mPlayerPos;
	Vector  mPlayerVel;
	double  mPlayerBodyDir;
	Vector  mBallPos;
	Vector  mBallVel;
};

/**
 * ����ԭ�Ӷ���
 * Describes an atomic action.
 */
struct AtomicAction
{
	bool        mSucceed;
	CommandType mType;

	Vector      mKickVel;
	AngleDeg    mTurnAngle;
	double      mDashPower;
    AngleDeg    mDashDir;

	AtomicAction(bool succeed = false):
		mSucceed (succeed),
		mType (CT_None),
		mTurnAngle (0.0),
		mDashPower (0.0),
        mDashDir (0.0)
	{
	}

	bool Execute(Agent & agent) const;

	void Clear() {
		mSucceed = false;
		mType = CT_None;
		mKickVel = Vector(0, 0);
		mTurnAngle = 0.0;
		mDashPower = 0.0;
		mDashDir = 0.0;
	}

	friend std::ostream & operator<<(std::ostream & os, const AtomicAction & act) {
		switch (act.mType) {
		case CT_Turn: return os << "(turn <" << act.mTurnAngle << ">)";
		case CT_Dash: return os << "(dash " << act.mDashPower << " " << act.mDashDir << ")";
		case CT_Kick: return os << "(kick " << act.mKickVel.Mod() << " " << act.mKickVel.Dir() << ")";
		case CT_None: return os << "(none)";
		default: return os << "";
		}
	}
};

/**
 * �ƻ�ִ�е�Action����Ϣ
 * Stores a queue of atomic actions planed to be executed.
 */
struct ActionPlan
{
	bool    mSucceed;
	int     mCycle;

	std::deque<AtomicAction> mActionQueue;

	ActionPlan() : mSucceed(false), mCycle(0) {}
};

/**
 * Action effector.
 * This class maintains a queue of commands which will be sent to the server. It provides low level implementations
 * of atomic actions such as kick, dash, and so on.
 */
class ActionEffector {
public:
	ActionEffector(Agent & agent);
	virtual ~ActionEffector() {}

	/**
	 * ��������ڷ���server�������������WorldState�ĸ���
	 * Check commands sent to server in last cycle. Help to update WorldState.
	 */
	void CheckCommandQueue(Observer *observer);

	/**
	 * ��������ڵ������Ƿ�©����ÿ���ڻ����Ϣ�󱻵���
	 * Check commands sent to server in last cycle, and see if there are any commands were lost.
	 * Called after sense information was fetched.
	 */
	void CheckCommands(Observer *observer);

	/**
	 * ��server������������е�����
	 * Send commands in queue to server.
	 */
	void SendCommands(char *msg = 0);

	void Reset();
	void ResetForScan();

	/**
	 * ������Щ�ӿ���server�ṩ����������Ľӿڣ��߲�������
	 * ��Щ�ӿ����ϸ���server�Ƿ��ִ�����жϣ�server�϶�����ִ�еĲŷ���true
	 * Each of the following methods provides an interface of a server command for high level decisions.
	 * Will return true if and only if command can be executed absolutely by server.
	 */
	bool SetTurnAction(AngleDeg angle); ///����� angle ������ת��ĽǶȣ����Ƿ��� server �ĽǶ�
	bool SetDashAction(double power, AngleDeg dir);
	bool SetTurnNeckAction(AngleDeg angle);
	bool SetSayAction(std::string msg);
	bool SetAttentiontoAction(Unum num); //�����ʵ�ֳɻ��⶯��
	bool SetAttentiontoOffAction();      //�����ʵ�ֳɻ��⶯��
	bool SetKickAction(double power, AngleDeg angle);
	bool SetTackleAction(AngleDeg angle, const bool foul);
	bool SetPointtoAction(double dist, AngleDeg angle);
	bool SetPointtoOffAction();
	bool SetCatchAction(AngleDeg angle);
	bool SetMoveAction(Vector pos);
	bool SetChangeViewAction(ViewWidth view_width);
	bool SetCompressionAction(int level);
	bool SetSenseBodyAction();
	bool SetScoreAction();
	bool SetByeAction();
	bool SetDoneAction();
	bool SetClangAction(int min_ver, int max_ver);
	bool SetEarOnAction(bool our_side, EarMode ear_mode = EM_All);
	bool SetEarOffAction(bool our_side, EarMode ear_mode = EM_All);
	bool SetSynchSeeAction();
	bool SetChangePlayerTypeAction(Unum num, int player_type);

	/**
	 * Each of the following methods provides an interface to get the number of times a certain action
	 * has been executed.
	 */
	int GetTurnCount() const { return mTurnCount; }
	int GetDashCount() const { return mDashCount; }
	int GetTurnNeckCount() const { return mTurnNeckCount; }
	int GetSayCount() const { return mSayCount; }
	int GetAttentiontoCount() const { return mAttentiontoCount; }
	int GetKickCount() const { return mKickCount; }
	int GetTackleCount() const { return mTackleCount; }
	int GetPointtoCount() const { return mPointtoCount; }
	int GetCatchCount() const { return mCatchCount; }
	int GetMoveCount() const { return mMoveCount; }
	int GetChangeViewCount() const { return mChangeViewCount; }
	int GetCompressionCount() const { return mCompressionCount; }
	int GetSenseBodyCount() const { return mSenseBodyCount; }
	int GetScoreCount() const { return mScoreCount; }
	int GetByeCount() const { return mByeCount; }
	int GetDoneCount() const { return mDoneCount; }
	int GetClangCount() const { return mClangCount; }
	int GetEarCount() const { return mEarCount; }
	int GetSynchSeeCount() const { return mSynchSeeCount; }

	/**
	 * If the commands queue includes turn, dash, kick, tackle, catch or move.
	 */
	bool IsMutex() const { return mIsMutex; }

	/**
	 * Each of the following methods checks if the commands queue includes a certain type of command.
	 */
	bool IsTurn() const { return mIsTurn; }
	bool IsDash() const { return mIsDash; }
	bool IsTurnNeck() const { return mIsTurnNeck; }
	bool IsSay() const { return mIsSay; }
	bool IsAttentionto() const { return mIsAttentionto; }
	bool IsKick() const { return mIsKick; }
	bool IsTackle() const { return mIsTackle; }
	bool IsPointto() const { return mIsPointto; }
	bool IsCatch() const { return mIsCatch; }
	bool IsMove() const { return mIsMove; }
	bool IsChangeView() const { return mIsChangeView; }
	bool IsCompression() const { return mIsCompression; }
	bool IsSenseBody() const { return mIsSenseBody; }
	bool IsScore() const { return mIsScore; }
	bool IsBye() const { return mIsBye; }
	bool IsDone() const { return mIsDone; }
	bool IsClang() const { return mIsClang; }
	bool IsEar() const { return mIsEar; }
	bool IsSynchSee() const { return mIsSynchSee; }
	bool IsChangePlayerType() const { return mIsChangePlayerType; }

	bool IsSayMissed() const { return mIsSayMissed; }
    CommandType GetLastCommandType() const { return mLastCommandType; }
	
    /** ���ݵ�ǰ�������������ݣ�Ԥ����Ϣ*/
	/**
	 * �õ�����ִ�к���ӽǿ��
	 * Get view width after queued actions.
	 */
	ViewWidth GetSelfViewWidthWithQueuedActions();

	/**
	 * Get self position after queued actions.
	 */
	Vector GetSelfPosWithQueuedActions();

	/**
	 * Get self velocity after queued actions.
	 */
	Vector GetSelfVelWithQueuedActions();

	/**
	 * Get self body direction after queued actions.
	 * btw, neck direction don't consider, said baj.
	 */
	AngleDeg GetSelfBodyDirWithQueuedActions();

	/**
	 * Get ball position after queued actions.
	 */
	Vector GetBallPosWithQueuedActions();

	/**
	 * Get ball velocity after queued actions.
	 */
	Vector GetBallVelWithQueuedActions();

public:
	/**
	 * ͨ������kick�Ĳ���������kick�����λ�ú��ٶ�
	 * Calculate ball position and velocity after a kick action.
	 */
	static void ComputeInfoAfterKick(const double kick_power, const double kick_angle,
			const PlayerState &player_state, const BallState &ball_state, Vector &ball_pos, Vector &ball_vel, bool is_self = true);

	/**
	 * ͨ������dash�Ĳ���������dash����Ա��λ�ú��ٶ�
	 * Calculate player position and velocity after a dash action.
	 */
	static void ComputeInfoAfterDash(const double dash_power, double dash_dir,
			const PlayerState &player_state, Vector &player_pos, Vector &player_vel);

	/**
	 * ͨ������move�Ĳ���������move����Ա��λ�ú��ٶ�
	 * Calculate player position and velocity after a move action.
	 */
	static void ComputeInfoAfterMove(const Vector & move_pos,
			Vector &player_pos, Vector &player_vel);

	/**
	 * ͨ������turn�Ĳ���������turn����Ա�����峯��Ͳ��ӳ���
	 * Calculate player body direction after a turn action.
	 */
	static void ComputeInfoAfterTurn(const AngleDeg turn_angle,
			const PlayerState &player_state, AngleDeg &body_dir);

	/**
	 * ͨ������turn_neck�Ĳ���������turn_neck����Ա�Ĳ��ӳ���
	 * Calculate player neck direction after a turn_neck action.
	 */
	static void ComputeInfoAfterTurnNeck(const AngleDeg turn_neck_angle,
			const PlayerState &player_state, AngleDeg &neck_dir);

private:
	const Agent       & mAgent;

	const WorldState  & mWorldState;
	const BallState   & mBallState;
    const PlayerState & mSelfState;

	std::list<CommandInfo> mCommandQueue;

public:
	static ThreadMutex CMD_QUEUE_MUTEX;

	Turn        mTurn;
	Dash        mDash;
	TurnNeck    mTurnNeck;
	Say         mSay;
	Attentionto mAttentionto;
	Kick        mKick;
	Tackle      mTackle;
	Pointto     mPointto;
	Catch       mCatch;
	Move        mMove;
	ChangeView  mChangeView;
	Compression mCompression;
	SenseBody   mSenseBody;
	Score       mScore;
	Bye         mBye;
	Done        mDone;
	Clang       mClang;
	Ear         mEar;
	SynchSee    mSynchSee;
	ChangePlayerType mChangePlayerType;

	int         mTurnCount;
	int         mDashCount;
	int         mTurnNeckCount;
	int         mSayCount;
	int         mAttentiontoCount;
	int         mKickCount;
	int         mTackleCount;
	int         mPointtoCount;
	int         mCatchCount;
	int         mMoveCount;
	int         mChangeViewCount;
	int         mCompressionCount;
	int         mSenseBodyCount;
	int         mScoreCount;
	int         mByeCount;
	int         mDoneCount;
	int         mClangCount;
	int         mEarCount;
	int         mSynchSeeCount;
	int         mChangePlayerTypeCount;

	bool        mIsMutex;

	bool        mIsTurn;
	bool        mIsDash;
	bool        mIsTurnNeck;
	bool        mIsSay;
	bool        mIsAttentionto;
	bool        mIsKick;
	bool        mIsTackle;
	bool        mIsPointto;
	bool        mIsCatch;
	bool        mIsMove;
	bool        mIsChangeView;
	bool        mIsCompression;
	bool        mIsSenseBody;
	bool        mIsScore;
	bool        mIsBye;
	bool        mIsDone;
	bool        mIsClang;
	bool        mIsEar;
	bool        mIsSynchSee;
	bool        mIsChangePlayerType;

	bool		mIsSayMissed;
    CommandType mLastCommandType; // ������ִ�еĻ���ԭ�Ӷ������ͣ���ʱֻ����kick, dash, move, turn
};

#endif /* ACTIONEFFECTOR_H_ */
