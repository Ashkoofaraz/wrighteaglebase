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

#ifndef __Formation_H__
#define __Formation_H__

#include "Types.h"
#include "Geometry.h"
#include "FormationTactics.h"
#include <vector>
#include <string>
#include <map>
#include <stack>

class Agent;
class WorldState;

#define FORMATION_LINE_NUM 3 // ������3������

/**
 * ������4�ֿ�ѡ���ͣ�����teammate��һ��ֻ��FT_Attack_Forward��FT_Defend_Back���֣�����opponent�������������������ǰ���4������ֱ�������
 * There are four kinds of optional formation, the teammate, the general FT_Attack_Forward and FT_Defend_Back
 * only two, the opponent, the coach sent in accordance with the formation for each of four kinds of fat to the
 */
enum FormationType
{
    FT_Attack_Forward = 0,  // ǰ������
    FT_Attack_Midfield, // �г�����
    FT_Defend_Midfield, // �г�����
    FT_Defend_Back,     // �󳡷���
    FT_Max
};

/**
 * ��������
 * Front type
 */
enum LineType
{
    LT_Null = -1,
    LT_Goalie = 0,
    LT_Defender,
    LT_Midfielder,
    LT_Forward
};

/**
 * λ������
 * Position type.
 */
enum PositionType
{
    PT_Null,
    PT_Left,
    PT_Middle,
    PT_Right,
	PT_RightRight,
	PT_RightMiddle,
	PT_LeftMiddle,
	PT_LeftLeft
};

enum Pside
{
	PS_None,
	PS_Left,
	PS_Center,
	PS_Right
};

enum PlayerRole
{
	PR_Goalie,
	PR_BackCenter,
	PR_BackSide,
	PR_MidCenter,
	PR_MidSide,
	PR_ForwardCenter,
	PR_ForwardSide,
	PR_Max
};

struct RoleType
{
	RoleType(): mIndexX(0), mIndexY(0), mLineType(LT_Null), mPositionType(PT_Null) {}

	/** x�����y��������� */
	int mIndexX; /** 0 - 3 */
	int mIndexY; /** 1 - 4*/

	LineType mLineType;
	PositionType mPositionType;

	PlayerRole ToPlayerRole() const;
};

class FormationBase
{
	FormationBase(const FormationBase &);

public:
	FormationBase(FormationType type);
	virtual ~FormationBase() {}

    FormationType GetFormationType() const { return mFormationType; }

    double GetHInterval(int index_H) const
    {
	    return mHInterval[index_H];
    }

    double GetVInterval(int index_V) const
    {
    	Assert(index_V != 0); //��������Ա�������û�������
	    return mVInterval[index_V];
    }

    int GetLineArrange(int index_i) const
    {
        return mLineArrange[index_i]; // index_i == 0��ʾ������Ա��������
    }

    void SetLineArrange(int index_i, int num)
    {
        mLineArrange[index_i] = num;
    }

    double GetHBallFactor() const { return mHBallFactor; }
    double GetVBallFactor() const { return mVBallFactor; }

    virtual bool IsRoleValid()
    {
    	return true;
    }

    const RoleType & GetPlayerRoleType(Unum unum) const
	{
        return mPlayerRole[mUnum2Index[unum]];
    }

    const Rectangular & GetActiveField(Unum unum) const
	{
        return mActiveField[mUnum2Index[unum]];
    }

    const Vector & GetOffside(Unum player1, Unum player2) const
	{
        return mOffsetMatrix[mUnum2Index[player1]][mUnum2Index[player2]];
    }

    const Unum & GetIndex2Unum(int index) const
    {
        return mIndex2Unum[index];
    }

    void SetIndex2Unum(int index, Unum unum)
    {
        mIndex2Unum[index] = unum;
    }

    const int & GetUnum2Index(Unum unum) const
    {
        return mUnum2Index[unum];
    }

    void SetUnum2Index(Unum unum, int index)
    {
        mUnum2Index[unum] = index;
    }

    /** ͨ����λ�õ���Ա���� */
	Unum GetUnumFromPos(int index_i, int index_j);
	bool SetUnumForPos(Unum player1, int index_i, int index_j);

	/** �������ĺ����ͻ����㣬�������﹩�ⲿ���ã���Ҫ����WorldState */
    const Vector & GetFormationCenter(const WorldState & world_state, bool is_teammate, double min_conf = 0.6);
	void SetFormationCenter(Unum num, Vector position);
    Vector GetFormationPoint(const WorldState &world_state, bool is_teammate, Unum unum, double min_conf = 0.6);

    Vector GetExpectedGoaliePos(const Vector & ball_pos, double run, const Vector & goalie_pos, int cycle_delay);

    virtual FormationTacticBase * GetTactic(FormationTacticType tactic) = 0;

protected:
    FormationType mFormationType;

    /** �������߼�ļ�࣬[0]��ʾ�������г�֮�䣬[1]��ʾ�г���ǰ��֮�� */
	Array<double, FORMATION_LINE_NUM-1, true> mHInterval;

	/** ���������ڲ���Ա��� */
	Array<double, FORMATION_LINE_NUM+1, true> mVInterval;

	/** ÿ�������ϵ���Ա��Ŀ���� */
	Array<int, FORMATION_LINE_NUM+1, true> mLineArrange;

	double      mHBallFactor;
	double      mVBallFactor;

	/** [2]��ʾ2����λ��RoleType��Ϣ */
	PlayerArray<RoleType> mPlayerRole;

	/** ÿ����Ա����Ч���� */
	PlayerArray<Rectangular> mActiveField; //��������Աδ׼ȷ���壬����ʹ��
	/** ͨ��λ�õĵ���λ����һάָ�ڼ����ߣ�����Ա��0�����ڶ�άָ�����ϵĵڼ����ˣ�����ߵ���0�� */
    Array<Array<int, TEAMSIZE, true>, FORMATION_LINE_NUM+1> mPos2Index;

	/**
	 * ��Ա�����
	 * 0��Զ��Ӧ0���Է�����ĳЩʱ���ʾ���򡱻��ߡ�û���ˡ���
	 */
    Array<Unum, TEAMSIZE+1> mIndex2Unum;
	Array<int, TEAMSIZE+1> mUnum2Index;

	/** ƫ�ƾ��� */
	Array<Array<Vector, TEAMSIZE+1>, TEAMSIZE+1 > mOffsetMatrix; //��������Աδ׼ȷ���壬����ʹ��
    Vector      mFormationCT;
};


/**
 * �ҷ������Σ��ɳ�ʼ��ʱ���
 */
class TeammateFormation: public FormationBase {
	TeammateFormation(const TeammateFormation & formation);

public:
	TeammateFormation(FormationType type, const char * config_file);
	virtual ~TeammateFormation();

	double GetFormationMaxX() const {return m_FORMATION_MAX_X;}
	double GetFormationMaxY() const {return m_FORMATION_MAX_Y;}

	FormationTacticBase * GetTactic(FormationTacticType tactic) { return mTactics[tactic]; }

private:
	double m_FORMATION_MAX_X;
	double m_FORMATION_MAX_Y;
  
    /** ��ʼ�����͵Ļ�����Ϣ */
    void InitialFormation(std::vector<std::string> & config);

    /** �������ö��ѵ����� */
    void SetTeammateRole();

    /** �������ø������������,����֮ǰ��Ҫ���ȷ������Ա����ͽ�ɫ���� */
	void SetHInterval(double back_middle, double middle_front);

	/** �������ø������ڲ���Ա���������,����֮ǰ��Ҫ���ȷ������Ա����ͽ�ɫ���� */
	void SetVInterval(double back, double middle, double front);

    Array<FormationTacticBase *, FTT_MAX> mTactics;
};


/**
 * �Է�����
 */
class OpponentFormation: public FormationBase {
	friend class Formation;
	friend class Client;
	friend class Coach;
    friend class CoachTeamModel;
	friend class CommunicateSystem;
    friend class WorldStateUpdater;

    OpponentFormation(const OpponentFormation & formation);

public:
    static double mForwardMaxX; // forward max x
    static double mDefenderMinX;  // defender min x

    OpponentFormation(Unum goalie_unum, FormationType type);
    virtual ~OpponentFormation();

    void SetFormationRole();
    void SetOffsetMatrix();
    bool IsRoleValid() { return (mUsedTimes > 0); }
    void SetGoalieUnum(Unum goalie_unum) { mGoalieUnum = goalie_unum; }

	FormationTacticBase * GetTactic(FormationTacticType tactic) { return mTactics[tactic]; }

private:
    Array<std::vector<int>, 3> mLineMember; // 3�����ڲ���Ա��0�Ǻ�����1���г���2��ǰ��
    unsigned long mUsedTimes; // ��Чͳ�ƴ���
    Array<int, 2> mHIntervalTimes; // HInterval����Ч����
    Unum mGoalieUnum;
    Array<FormationTacticBase *, FTT_MAX> mTactics;

public:
    enum
    {
        FRONT_CODE_SIZE = 10,
        BACK_CODE_SIZE = 10,
        FORMATION_CODE_SIZE = 21 // ���ͱ��볤�� = 1 + 10 + 10
    };

    bool operator < (const OpponentFormation& other) const
    {
    	Assert(mFormationType == other.mFormationType);
        return ((mFormationType == other.mFormationType) && (mUsedTimes < other.mUsedTimes));
    }

    bool operator == (const OpponentFormation& other) const
    {
        if (mFormationType == other.mFormationType &&
            mLineMember[0].size() == other.mLineMember[0].size() &&
            mLineMember[1].size() == other.mLineMember[1].size() &&
            mLineMember[2].size() == other.mLineMember[2].size())
	    {
		    for (int n = 0; n < 3; n++)
            {
                for (unsigned int i = 0; i < mLineMember[n].size(); i++)
                {
				    if (mLineMember[n][i] != other.mLineMember[n][i])
                    {
					    return false;
				    }
			    }
		    }
		    return true;
	    }
	    return false;
    }

    struct PtrCompare {
    	bool operator() (const OpponentFormation * const & a, const OpponentFormation * const & b) const {
    		return *a < *b;
    	}
    };
};


/**
 * ͳһά���Լ������κͶԷ������Σ��������涼ʹ��������ṩ�Ľӿ�
 */
class Formation
{
public:
	Formation (Agent & agent);
	virtual ~Formation() {}

    FormationBase & GetTeammateFormation() { return *mpTeammateFormation; }
    FormationBase & GetOpponentFormation() { return *mpOpponentFormation; }

public:
    bool IsTeammateAttackFormation() const
    {
        return (mpTeammateFormation->GetFormationType() == FT_Attack_Forward || mpTeammateFormation->GetFormationType() == FT_Attack_Midfield);
    }

    bool IsOpponentAttackFormation() const
    {
        return (mpOpponentFormation->GetFormationType() == FT_Attack_Forward || mpOpponentFormation->GetFormationType() == FT_Attack_Midfield);
    }

    Unum GetTeammateUnumFromIndex(int index_i, int index_j) const
    {
	    return mpTeammateFormation->GetUnumFromPos(index_i, index_j);
    }

    Unum GetOpponentUnumFromIndex(int index_i, int index_j) const
    {
	    return mpOpponentFormation->GetUnumFromPos(index_i, index_j);
    }

    // �Լ������ͱ�֤ÿ����������2���ˣ����Լ�����Ľӿڣ������ͷ���   2009-05-04
    Unum GetTeammateLeftDefender() const
    {
        return GetTeammateUnumFromIndex(1, 1);
    }

    Unum GetTeammateRightDefender() const
    {
        return GetTeammateUnumFromIndex(1, GetTeammateLineArrange(1));
    }

    Unum GetTeammateLeftMidfielder() const
    {
        return GetTeammateUnumFromIndex(2, 1);
    }

    Unum GetTeammateRightMidfielder() const
    {
        return GetTeammateUnumFromIndex(2, GetTeammateLineArrange(2));
    }

    Unum GetTeammateLeftForward() const
    {
        return GetTeammateUnumFromIndex(3, 1);
    }

    Unum GetTeammateRightForward() const
    {
        return GetTeammateUnumFromIndex(3, GetTeammateLineArrange(3));
    }

    int GetTeammateLineArrange(int index_i) const
    {
        return mpTeammateFormation->GetLineArrange(index_i);
    }

    int GetOpponentLineArrange(int index_i) const
    {
        return mpOpponentFormation->GetLineArrange(index_i);
    }

    double GetTeammateHInterval(int index_H) const
    {
	    return mpTeammateFormation->GetHInterval(index_H);
    }

    double GetTeammateVInterval(int index_V) const
    {
	    return mpTeammateFormation->GetVInterval(index_V);
    }

    double GetOpponentHInterval(int index_H) const
    {
	    return mpOpponentFormation->GetHInterval(index_H);
    }

    double GetOpponentVInterval(int index_V) const
    {
    	return mpOpponentFormation->GetVInterval(index_V);
    }

    double GetTeammateHBallFactor() const
    {
        return mpTeammateFormation->GetHBallFactor();
    }

    double GetTeammateVBallFactor() const
    {
        return mpTeammateFormation->GetVBallFactor();
    }

    double GetOpponentHBallFactor() const
    {
        return mpOpponentFormation->GetHBallFactor();
    }

    double GetOpponentVBallFactor() const
    {
        return mpOpponentFormation->GetVBallFactor();
    }

    const RoleType & GetTeammateRoleType(Unum unum) const
    {
        return mpTeammateFormation->GetPlayerRoleType( unum);
    }

	const RoleType & GetOpponentRoleType(Unum unum) const
    {
        return mpOpponentFormation->GetPlayerRoleType(unum);
    }

	const RoleType & GetPlayerRoleType(Unum unum) const
    {
        return (unum > 0) ? GetTeammateRoleType(unum) : GetOpponentRoleType(-unum);
    }

    const RoleType & GetMyRole()  const;

    FormationType GetTeammateFormationType() const { return mpTeammateFormation->GetFormationType(); }

    const Vector & GetTeammateFormationCenter(double min_conf = 0.6);
	void SetTeammateFormationCenter(Unum num, Vector position);
    Vector GetTeammateFormationPoint(Unum unum, double min_conf = 0.6);

    Vector GetTeammateExpectedGoaliePos(Vector bp, double run);
    Vector GetTeammateFormationPoint(Unum unum, const Vector & ball_pos);
    Vector GetTeammateFormationPoint(Unum unum, Unum focusTm, Vector focusPt);

    FormationType GetOpponentFormationType() const { return mpOpponentFormation->GetFormationType(); }

    const Vector & GetOpponentFormationCenter(double min_conf = 0.6);
    void SetOpponentFormationCenter(Unum num, Vector position);
    Vector GetOpponentFormationPoint(Unum unum, double min_conf = 0.6);

    Vector GetOpponentExpectedGoaliePos(Vector bp, double run);
    Vector GetOpponentFormationPoint(Unum unum, const Vector & ball_pos);
    bool IsOpponentRoleValid();

    FormationTacticBase * GetTeammateTactic(FormationTacticType tactic) { return GetTeammateFormation().GetTactic(tactic); }
    FormationTacticBase * GetOpponentTactic(FormationTacticType tactic) { return GetOpponentFormation().GetTactic(tactic); }

public:
	void SetTeammateFormationType(FormationType type);
	void SetOpponentFormationType(FormationType type);

	enum UpdatePolicy { Offensive, Defensive };
    void Update(UpdatePolicy policy, std::string update_name);
    void Rollback(std::string update_name);

private:
	Agent & mAgent;
	FormationBase *mpTeammateFormation;
	FormationBase *mpOpponentFormation;

	std::stack<std::pair<FormationType, FormationType> > mFormationTypeStack;

public:
    friend class Instance;
    static class Instance
    {
    	Array<TeammateFormation*, 4> mpTeammateFormationsImp;
    	Array<OpponentFormation*, 4> mpOpponentFormationsImp;

    	Array<TeammateFormation*, FT_Max> mpTeammateFormations;
    	Array<OpponentFormation*, FT_Max> mpOpponentFormations;

    public:
    	Instance();
    	~Instance();
    	void AssignWith(Agent * agent);
    	void SetTeammateFormations();

    	Formation & operator ()();

        /**
         * The following 2 methods return the static member directly.
         */
        TeammateFormation & GetTeammateFormation(FormationType type) { return *mpTeammateFormations[type]; }
        OpponentFormation & GetOpponentFormation(FormationType type) { return *mpOpponentFormations[type]; }

        /**
         * And the following methods set the static member directly.
         */
        void SetOpponentFormation(FormationType type, OpponentFormation * formation) { mpOpponentFormations[type] = formation; }

    	/**
         * �ڽ���������Ϣ֮ǰ���Լ������Լ���һ�¶������ͣ����漰���㻹�Ƿ��㡣
         */
        void UpdateOpponentRole();
    	void SetOpponentGoalieUnum(Unum goalie_unum);

    private:
    	Agent * mpAgent;
    } instance;
};

#endif
