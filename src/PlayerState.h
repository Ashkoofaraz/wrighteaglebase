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

#ifndef __PLAYERSTATE_H__
#define __PLAYERSTATE_H__

#include "BaseState.h"
#include "PlayerParam.h"
#include "BallState.h"

class PlayerState : public MobileState
{
public:
	/**�ֱ�ָ�������� ע���Լ���Ա���Լ���õ���Ϣ��һ����
	* ��ʱ���Լ��ɻ�õ���ϢΪ��׼�����࣬������Աֻ�ܻ����Ϣ��һ����*/
	class ArmPoint
	{
	public:
		ArmPoint(int = 0):
			mMovableBan(0),
			mExpireBan(0),
			mTargetDist(0),
			mTargetDir(0)
		{
		}

		/**���´��ֱ��ܶ���ʣ��������*/
		int mMovableBan;

		/**ֱ���ֱ۶���ʧЧʣ���������*/
		int mExpireBan;

		/**ָ��Ŀ��ľ���*/
		double mTargetDist;

		/**ָ��Ŀ��ķ���*/
		AngleDeg mTargetDir;
	};

	/**ע�Ӷ������ ע����ʱֻ���Լ����Ի��ע�ӵ���Ϣ������ֻ��Ϊ�Ժ����ӿ�*/
	class FocusOn
	{
	public:
		FocusOn(int = 0):
			mFocusSide('?'),
			mFocusNum(0)
		{
		}

		/**ע�ӵĶ���Ϊ��һ��*/
		char mFocusSide;

		/**ע�ӵ���Ա��*/
		Unum mFocusNum;
	};

public:
	PlayerState();
	virtual ~PlayerState() {}

	/**��������
	* @param ����
	* @param ʱ��
	* @param ���Ŷ�*/
	void UpdateStamina(double stamina);

	/**��õ�ǰ��ǰ���ε�����
	*/
	const double & GetStamina() const { return mStamina; }

	/**����effort
	* @param ����
	* @param ʱ��
	* @param ���Ŷ�*/
	void UpdateEffort(double effort);
	void UpdateCapacity(double capacity);

	/**��õ�ǰ��ǰ���ε�effort
	*/
	const double & GetEffort() const { return mEffort; }

	const double & GetCapacity() const { return mCapacity; }

	/**
	 * Update recovery
	 * @param recovery
	 */
	void UpdateRecovery(double recovery) { mRecovery = recovery;}

	/** get recovery*/
	const double & GetRecovery() const { return mRecovery;}

	/**���²��ӳ���
	* @param ����
	* @param ʱ��
	* @param ���Ŷ�*/
	void UpdateNeckDir(double dir , int delay = 0, double conf = 1);

	/**��õ�ǰ��ǰ���εĲ���ȫ�ֳ���
	*/
	double GetNeckGlobalDir() const { return GetNormalizeAngleDeg(GetNeckDir() + GetBodyDir()); }

	/**��õ�ǰ�������������ĽǶ�
	* ע��������GetGlobalNeckDir����
	*/
	double GetNeckDir() const { return GetNormalizeNeckAngle(mNeckDir.mValue); }

	/**��õ�ǰ��ǰ���εĲ��ӳ����ʱ��
	*/
	int GetNeckDirDelay() const { return mNeckDir.mCycleDelay; }

	/**��õ�ǰ��ǰ���εĲ��ӳ�������Ŷ�
	*/
	double GetNeckDirConf() const { return mNeckDir.mConf; }

	/**�������峯��
	* @param ����
	* @param ʱ��
	* @param ���Ŷ�*/
	void UpdateBodyDir(double dir , int delay = 0, double conf = 1);

	/**��õ�ǰ��ǰ���ε����峯��
	*/
    const double & GetBodyDir() const { return mBodyDir.mValue; }

	/**��õ�ǰ��ǰ���ε����峯���ʱ��
	*/
	int GetBodyDirDelay() const { return mBodyDir.mCycleDelay; }

	/**��õ�ǰ��ǰ���ε����峯������Ŷ�
	*/
	double GetBodyDirConf() const { return mBodyDir.mConf; }

	void UpdateTackleBan(int ban);

	int GetTackleBan() const { return mTackleBan; }

	/**���¿����������
	* @param ��ʣ�������ڿ�������
	*/
	void UpdateCatchBan(int ban);

	/**��õ�ǰ���ڻ��ж������ڿ������� ��ʱ�޷���*/
	int GetCatchBan() const { return mCatchBan; }

	/**������ָ����
	* @param �ֳ���
	* @param ʱ��
	* @param ���Ŷ�
	* @param ����
	* @param �´οɶ�ʱ��
	* @param ��Чʱ��ʣ��*/
	void UpdateArmPoint(AngleDeg dir , int delay = 0, double conf = 1 , double  dist = 0 , int move_ban = -1 , int expire_ban = -1);

	/**�Ƿ���ָ*/
	bool IsPointing() const {return mIsPointing;}
	void SetIsPointing(bool is_pointing) {mIsPointing = is_pointing;}

	/**�����ָ��ĳ���*/
	AngleDeg GetArmPointDir() const { return GetNormalizeAngleDeg(mArmPoint.mValue.mTargetDir); }

	/**�����ָ��ľ��� ��ʱ���Լ�������*/
	double  GetArmPointDist() const { return mArmPoint.mValue.mTargetDist; }

	/**����ֻ�ʣ����ʱ��ſɶ� ��ʱ���Լ�������*/
	int GetArmPointMovableBan() const { return mArmPoint.mValue.mMovableBan; }

	/**����ֶ�����Чʱ��ʣ�� ��ʱ���Լ�������*/
	int GetArmPointExpireBan() const { return mArmPoint.mValue.mExpireBan; }

	/**�����ָ���ʱ���ӳ�*/
	int GetArmPointDelay() const { return mArmPoint.mCycleDelay; }

	/**�����ָ������Ŷ�*/
	double GetArmPointConf() const { return mArmPoint.mConf; }

	/**����ע��Ŀ��
	* @param ע���ķ�
	* @param ע�Ӷ�Ա��
	* @param ʱ��
	* @param ���Ŷ�*/
	void UpdateFocusOn(char side , Unum num , int delay = 0 , double conf = 1);

	/**���ע�ӵĶ�Ա��λ*/
	char GetFocusOnSide() const { return mFocusOn.mValue.mFocusSide; }

	/**���ע�ӵĶ�Ա����*/
	Unum GetFocusOnUnum() const { return mFocusOn.mValue.mFocusNum; }

	/**���ע�ӵ�ʱ���ӳ�*/
	int GetFocusOnDelay() const { return mFocusOn.mCycleDelay; }

	/**���ע�ӵ����Ŷ�*/
	double GetFocusOnConf() const { return mFocusOn.mConf; }

	/**�����Ƿ���������*/
	void UpdateKicked(bool is_kicked);

	/**���±������Ƿ����*/ //  ***********************���ڸú�����Ч**********************
	void UpdateTackling(bool is_tackling);

	/**���±������Ƿ񱻲���*/
	void UpdateLying(bool is_lying);

	/**����������Ƿ����� ��ʱ���Լ������޷���*/
	bool IsKicked() const { return mIsKicked; }

	/**��ñ������Ƿ���� ��ʱ���Լ������޷���*/
	bool IsAlive() const { return mIsAlive; }

	void SetIsAlive(bool alive);

	/**��ñ������Ƿ����ڲ��������tackle_ban��*/
	bool IsTackling() const { return mTackleBan > 0; }

	bool IsLying() const { return mFoulChargedCycle > 0; }

	/**������Ա�����¼*/
	CardType GetCardType() const { return mCardType; }
	void UpdateCardType(CardType card_type) { if ( mCardType == CR_None || ( mCardType == CR_Yellow && card_type == CR_Red ) ) mCardType = card_type; }

	/**����������Ա����ʱ��*/
	int GetFoulChargedCycle () const { return mFoulChargedCycle; }

	void UpdateFoulChargedCycle(int x)
	{
		mFoulChargedCycle = x;
	}

	bool IsIdling() const {
		Assert(!(IsTackling() && IsLying()));
		return IsTackling() || IsLying();
	}

	int GetIdleCycle() const {
		Assert(!(IsTackling() && IsLying()));
		return GetTackleBan() + GetFoulChargedCycle();
	}

	/**�����Ա������*/
	int GetPlayerType() const { return mPlayerType; }

	/**������Ա������*/
	void UpdatePlayerType(int type);

	/**������Ա���ӽǿ��*/
	void UpdateViewWidth(ViewWidth width);

	/**����ӽǿ��*/
	ViewWidth GetViewWidth() const { return mViewWidth; }

	/**get view angle*/
	AngleDeg GetViewAngle() const { return sight::ViewAngle(mViewWidth);}

	/**�ڲ��Ը���
	* @param ÿ����delay���ϵ�����
	* @param ÿ����conf˥��������*/
	void AutoUpdate(int delay_add = 1 , double conf_dec_factor = 1);

	/**������Ա�ߺ�Unum*/
	void UpdateUnum(Unum num);

	/**���Unum*/
	Unum GetUnum() const { return mUnum; }

	/**update collide with ball*/
	void UpdateCollideWithBall(bool collide) { mCollideWithBall = collide;}

	/**get collide with ball*/
	bool GetCollideWithBall() const { return mCollideWithBall;}

	/**update collide with player*/
	void UpdateCollideWithPlayer(bool collide) { mCollideWithPlayer = collide;}

	/** get collide with player*/
	bool GetCollideWithPlayer() const { return mCollideWithPlayer;}

	/** update collide with post*/
	void UpdateCollideWithPost(bool collide) { mCollideWithPost = collide;}

	/** get collide with post*/
	bool GetCollideWithPost() const { return mCollideWithPost;}

	/**ת���õ�һ�����ڷ����PlayerState*/
	void GetReverseFrom(const PlayerState & o);

private:
	/**�洢�������ڵ�����*/
	double mStamina;

	/**�洢Effort*/
	double mEffort;

	/**�洢capacity*/
	double mCapacity;

	/**store Recovery*/
	double mRecovery;

	/**�洢���ӵĳ���*/
	StateValue<double> mNeckDir;

	/**�洢����ĳ���*/
	StateValue<double> mBodyDir;

	/**���ж������ڿ��Բ���*/
	int mTackleBan;

	/**���ж������ڿ������� ��ʱ�޷���*/
	int mCatchBan;

	/**��Աָ����Ϣ*/
	StateValue<ArmPoint> mArmPoint;

	/**��Աע����Ϣ*/
	StateValue<FocusOn>  mFocusOn;

	/**�ϴο���ʱ�Ƿ���ָ*/
	bool mIsPointing;

	bool mIsKicked;

	/**�Ƿ���� ��ʱ�Լ������޷���*/
	bool mIsAlive;

	/**��Ա����*/
	int mPlayerType;

	/**�ӽǿ��*/
	ViewWidth mViewWidth;

	/**
	 * �Լ���Unum
	 * + ��ʾ�Ƕ��ѣ�- ��ʾ�Ƕ���
	 * */
	Unum mUnum;

	/**
	 * whether collide with ball
	 */
	bool mCollideWithBall;

	/**
	 * whether collide with player
	 */
	bool mCollideWithPlayer;

	/**
	 * whether collide with post
	 */
	bool mCollideWithPost;

	int mFoulChargedCycle; //�������ڵص�����

	//==============================================================================
public:
	bool IsKickable() const { return mIsKickable; }
	bool IsKickable(const BallState &, const double & buffer) const;
	void UpdateKickable(bool kickable) { mIsKickable = kickable; }

	double GetKickRate() const { return mKickRate; }
	void UpdateKickRate(double kick_rate) { mKickRate = kick_rate; }

	bool IsBallCatchable() const { return mIsGoalie && mBallCatchable; }
	void UpdateBallCatchable(bool ball_catchable) { mBallCatchable = ball_catchable; }

	bool UnderDangerousTackleCondition() const { return mUnderDangerousTackleCondition; }
	void UpdateDangerousTackleCondition(const bool may) { mUnderDangerousTackleCondition = may; }

	double GetTackleProb(const bool foul) const { return mTackleProb[foul]; }
	void UpdateTackleProb(double tackle_prob, const bool foul = false) { mTackleProb[foul] = tackle_prob; }

	AngleDeg GetMaxTurnAngle() const { return mMaxTurnAngle; }
	void UpdateMaxTurnAngle(AngleDeg max_turn_angle) { mMaxTurnAngle = max_turn_angle; }

	bool IsTired() const {return mIsTired;}
	void UpdateTired(bool is_tired) { mIsTired = is_tired;}

    const double & GetMinStamina() const { return mMinStamina; }
    void UpdateMinStamina(const double min_stamina) { mMinStamina = min_stamina; } 

	bool IsBodyDirMayChanged() const { return mIsBodyDirMayChanged;}
	void UpdateBodyDirMayChanged(bool may_changed) { mIsBodyDirMayChanged = may_changed;}

	//��������Pos_delay  ��Ҫ��BT_Interceptʹ��;
	void ResetPosDelay(int delay)
	{
		Assert(delay >= 0);
		double conf = 1;
		for (int i = 0;i < delay;i++)
		{
			conf  *= PlayerParam::instance().playerConfDecay();
		}

		UpdatePos(GetPos() , delay , conf);
	}

private:
	bool        mIsKickable;  /** kick */
	double      mKickRate;
	bool        mBallCatchable; /** catch */

	Array<double, 2,true> mTackleProb;    /** tackle */
	bool mUnderDangerousTackleCondition; //��������Ƿ������Σ�����

	AngleDeg    mMaxTurnAngle;  /** turn */
	bool		mIsTired;		/** tell is  tired or not*/
    double      mMinStamina; // �������ֵ��������player��extraStamina
	CardType    mCardType;  /** card */

	//just for update body dir other should not use
	bool        mIsBodyDirMayChanged;

public:
	bool IsGoalie() const { return mIsGoalie; }
	void UpdateIsGoalie(bool is_goalie) { mIsGoalie = is_goalie; }

private:
	bool        mIsGoalie;

public:
    bool IsSensed() const { return mIsSensed; }
    void UpdateIsSensed(bool is_sensed) { mIsSensed = is_sensed; }

private:
    bool        mIsSensed; // Ϊtrue��ʾ�ǿ����յ�sense��Ϣ����Ա���������������Լ�

public:
	/** some useful interfaces */
	const double & GetPlayerSpeedMax() const { return PlayerParam::instance().HeteroPlayer(mPlayerType).playerSpeedMax(); }
	const double & GetStaminaIncMax() const { return PlayerParam::instance().HeteroPlayer(mPlayerType).staminaIncMax(); }
	const double & GetPlayerDecay() const { return PlayerParam::instance().HeteroPlayer(mPlayerType).playerDecay(); }
	const double & GetInertiaMoment() const { return PlayerParam::instance().HeteroPlayer(mPlayerType).inertiaMoment(); }
	const double & GetDashPowerRate() const { return PlayerParam::instance().HeteroPlayer(mPlayerType).dashPowerRate(); }
	const double & GetPlayerSize() const { return PlayerParam::instance().HeteroPlayer(mPlayerType).playerSize(); }
	const double & GetKickableMargin() const {return PlayerParam::instance().HeteroPlayer(mPlayerType).kickableMargin(); }
	const double & GetKickRand() const { return PlayerParam::instance().HeteroPlayer(mPlayerType).kickRand(); }
	const double & GetKickPowerRate() const { return PlayerParam::instance().HeteroPlayer(mPlayerType).kickPowerRate(); }
	const double & GetExtraStamina() const { return PlayerParam::instance().HeteroPlayer(mPlayerType).extraStamina(); }
	const double & GetEffortMax() const { return PlayerParam::instance().HeteroPlayer(mPlayerType).effortMax(); }
	const double & GetEffortMin() const { return PlayerParam::instance().HeteroPlayer(mPlayerType).effortMin(); }
	virtual const double & GetKickableArea() const { return PlayerParam::instance().HeteroPlayer(mPlayerType).kickableArea(); }
	const double & GetCatchAreaLStretch() const { return PlayerParam::instance().HeteroPlayer(mPlayerType).catchableAreaLStretch(); }
	const double & GetMinCatchArea() const { return PlayerParam::instance().HeteroPlayer(mPlayerType).minCatchArea(); }
	const double & GetMaxCatchArea() const { return PlayerParam::instance().HeteroPlayer(mPlayerType).maxCatchArea(); }
	const double & GetFoulDetectProbability() const { return PlayerParam::instance().HeteroPlayer(mPlayerType).foulDetectProbability(); }

	double GetAccelerationRateOnDir(const double dir) const { return PlayerParam::instance().HeteroPlayer(mPlayerType).accelerationRateOnDir(dir); }
    const double & GetAccelerationFrontRate() const { return PlayerParam::instance().HeteroPlayer(mPlayerType).accelerationFrontRate(); }
    const double & GetAccelerationSideRate() const { return PlayerParam::instance().HeteroPlayer(mPlayerType).accelerationSideRate(); }
    const double & GetAccelerationFrontMax() const { return PlayerParam::instance().HeteroPlayer(mPlayerType).accelerationFrontMax(); }
    const double & GetAccelerationSideMax() const { return PlayerParam::instance().HeteroPlayer(mPlayerType).accelerationSideMax(); }
    const double & GetAccelerationFront(const double & power) const { return PlayerParam::instance().HeteroPlayer(mPlayerType).accelerationFront(power); }
    const double & GetAccelerationSide(const double & power) const { return PlayerParam::instance().HeteroPlayer(mPlayerType).accelerationSide(power); }

	double GetCatchProb( const double & dist ) const;
	double GetControlBallProb(const Vector & ball_pos) const; //����kick��catch��tackle�����и�������һ��

	Vector GetPredictedPosWithDash(int steps = 1, double dash_power = 0, AngleDeg dash_dir = 0.0) const;
	Vector GetPredictedVelWithDash(int steps = 1, double dash_power = 0, AngleDeg dash_dir = 0.0) const;

	double CorrectDashPowerForStamina(double dash_power) const;

	AngleDeg GetEffectiveTurn(AngleDeg moment, double my_speed) const;
	AngleDeg GetEffectiveTurn(AngleDeg moment) const
	{ return GetEffectiveTurn(moment, GetVel().Mod()); }
	AngleDeg GetMaxEffectiveTurn(double my_speed) const /* how much we'll actually turn if we try max turn */
	{ return GetEffectiveTurn(ServerParam::instance().maxMoment(), my_speed); }

public:
	bool IsBodyDirValid() const { return GetBodyDirConf() > 0.991; }

	bool IsOutOfStamina() const { return (GetStamina() + GetCapacity() < PlayerParam::instance().MinStamina() + FLOAT_EPS); }

};

//make to be inlined
inline void PlayerState::UpdateStamina(double stamina)
{
	mStamina = stamina;
}

inline void PlayerState::UpdateEffort(double effort)
{
	mEffort = effort;
}

inline void PlayerState::UpdateCapacity(double capacity)
{
	mCapacity = capacity;
}

inline void PlayerState::UpdateNeckDir(double dir, int delay, double conf)
{
	mNeckDir.mValue = GetNormalizeAngleDeg(dir);
	mNeckDir.mCycleDelay = delay;
	mNeckDir.mConf  = conf;
}

inline void PlayerState::UpdateBodyDir(double dir, int delay, double conf)
{
	mBodyDir.mValue = GetNormalizeAngleDeg(dir);
	mBodyDir.mCycleDelay = delay;
	mBodyDir.mConf = conf;
}

inline void PlayerState::UpdateTackleBan(int ban)
{
	mTackleBan = ban;
}

inline void PlayerState::UpdateCatchBan(int ban)
{
	mCatchBan = ban;
}

inline void PlayerState::UpdateArmPoint(AngleDeg dir, int delay, double conf, double dist, int move_ban , int expire_ban )
{
	ArmPoint arm;
	arm.mTargetDir = GetNormalizeAngleDeg(dir);
	arm.mTargetDist = dist;
	arm.mMovableBan = move_ban;
	arm.mExpireBan  = expire_ban;

	mArmPoint.mValue = arm;
	mArmPoint.mCycleDelay = delay;
	mArmPoint.mConf  = conf;
}

inline void PlayerState::UpdateFocusOn(char side, Unum num, int delay, double conf)
{
	FocusOn focus;
	focus.mFocusSide = side;
	focus.mFocusNum  = num;

	mFocusOn.mValue = focus;
	mFocusOn.mCycleDelay = delay;
	mFocusOn.mConf  = conf;
}

inline void PlayerState::UpdateKicked(bool is_kicked)
{
	mIsKicked = is_kicked;
}

inline void PlayerState::AutoUpdate(int delay_add, double conf_dec_factor)
{
	MobileState::AutoUpdate(delay_add , conf_dec_factor);

	mNeckDir.AutoUpdate(delay_add , conf_dec_factor);
	mBodyDir.AutoUpdate(delay_add , conf_dec_factor);
	mArmPoint.AutoUpdate(delay_add , conf_dec_factor);
	mFocusOn.AutoUpdate(delay_add , conf_dec_factor);

	mCollideWithPost = false;
	mCollideWithPlayer = false;
	mCollideWithBall = false;
	mIsKicked    = false;
	mIsTired     = false;

	if (mTackleBan > 0) {
		mTackleBan -= delay_add;
		mTackleBan = Max(mTackleBan, 0);
	}

	if (mFoulChargedCycle > 0) {
		mFoulChargedCycle -= delay_add;
		mFoulChargedCycle = Max(mFoulChargedCycle, 0);
	}

	if (mCatchBan > 0) {
		mCatchBan -= delay_add;
		mCatchBan = Max(mCatchBan, 0);
	}

	if (mArmPoint.mValue.mExpireBan > 0) {
		mArmPoint.mValue.mExpireBan -= delay_add;
		mArmPoint.mValue.mExpireBan = Max(mArmPoint.mValue.mExpireBan, 0);
	}

	if (mArmPoint.mValue.mMovableBan > 0) {
		mArmPoint.mValue.mMovableBan -= delay_add;
		mArmPoint.mValue.mMovableBan = Max(mArmPoint.mValue.mMovableBan, 0);
	}

	if (GetPosDelay() > delay_add) {
		mIsBodyDirMayChanged = true;
	}
}

inline void PlayerState::UpdatePlayerType(int type)
{
	if (mPlayerType != type) {
		mPlayerType = type;

		SetDecay(PlayerParam::instance().HeteroPlayer(mPlayerType).playerDecay());
		SetEffectiveSpeedMax(PlayerParam::instance().HeteroPlayer(mPlayerType).effectiveSpeedMax());
	}
}

inline void PlayerState::UpdateViewWidth(ViewWidth width)
{
	mViewWidth = width;
}

inline void PlayerState::UpdateUnum(Unum num)
{
	mUnum = num;
}

inline void PlayerState::SetIsAlive(bool alive)
{
	mIsAlive = alive;

	if (!mIsAlive) {
		UpdatePos(GetPos(), GetPosDelay(), 0.0);
		UpdateVel(GetVel(), GetVelDelay(), 0.0);

		mNeckDir.mConf = 0.0;
		mBodyDir.mConf = 0.0;
	}
}

#endif
