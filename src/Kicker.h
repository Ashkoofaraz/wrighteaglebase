#ifndef __Kicker_H__
#define __Kicker_H__

#include "Agent.h"

enum KickMode
{
    KM_Null,
    KM_Hard,
    KM_Quick
};

class Kicker
{
    Kicker();

public:
    ~Kicker();

    static Kicker & instance();

    /**
     * ����mKickerValue��
     * Compute kicker value table.
     */
    void ComputeUtilityTable();

    /**
     * ͨ��kick�����ģ�ͼ���������max_rand
     * Calculate maximum random error after kick action.
     */
    double GetMaxRandAfterKick(const Agent & agent, double power);

    /**
     * ������ĳ��ٶȺ�Ŀ���ٶȣ��õ�kick��Ҫ��power��������Ϊ��������Ϊ�����
     * Calculate power need for an kick action according to initial and objective velocity.
     */
    double GetOneKickPower(const Vector & ball_vel, const Vector & ball_goal_vel, const double kick_rate);

    /**
     * ����2�����غ��������ڸ���Ŀ��������ǶȺ��������ڵ�ǰ���£�ʹ���ܴﵽ��������٣�������Ϊ������
     * The following 2 functions calculate maximum speed the ball can reach by given conditions.
     */
	double GetMaxSpeed(const Agent & agent, const double & kick_angle, const int & cycle, Vector * kick_out_pos = 0);
    double GetMaxSpeed(const Agent & agent, const Vector & kick_pos, const int & cycle, Vector * kick_out_pos = 0, Vector *last_vel = 0);

    /**
     * �����õ��Ľӿڣ�����������Ҫ����������
     * the following 2 functions generate kick action plans.
     */
	bool KickBall(Agent & agent, double angle, double speed_out, KickMode mode = KM_Hard, int *cycle_left = 0, bool is_shoot = false);
    bool KickBall(Agent & agent, const Vector & target, double speed_out, KickMode mode = KM_Hard, int * cycle_left = 0, bool is_shoot = false);

    /**
     * ִ�к���
     * Execute a kick action plan.
     */
    void Execute(Agent &agent, const ActionPlan &plan);

private:

    /** ����ɢ����Ѱ�������һ���� */
    inline int NearestPoint(const Vector & p)
    {
        double ang = 0.0;
	    double value = p.Mod();
	    int idxA;
	    if(value < mD1)
	    {
		    double fix = 180.0 / mNlayer[0];
		    ang = p.Dir() + fix;
		    ang = GetNormalizeAngleDeg(ang, 0.0);
		    idxA = int(ang * mNlayer[0] / 360.0);
		    return idxA;
	    }
	    else if(value > mD2)
	    {
		    double fix = 180.0 / mNlayer[2];
		    ang = p.Dir() + fix;
		    ang = GetNormalizeAngleDeg(ang, 0.0);
		    idxA = int(ang * mNlayer[2] / 360.0);
		    return mNlayer[0] + mNlayer[1] + idxA;
	    }
	    else
	    {
		    double fix = 180.0 / mNlayer[1];
		    ang = p.Dir() + fix;
		    ang = GetNormalizeAngleDeg(ang, 0.0);
		    idxA = int(ang * mNlayer[1] / 360.0);
		    return mNlayer[0] + idxA;
	    }
    }

    /** ��ȡmKickerValue�� */
    void ReadUtilityTable();

    /** ����kick�����ݣ�������ʱ����� */
    void UpdateKickData(const Agent & agent);

    /**
     * �õ�һ��kick��ĳ�����ܴﵽ������ٶȣ�������Ϊ��������Ϊ�����
     * Calculate maximum speed from acceleration. All parameters should be in the same coordinate
     * system. And return value will be in the same system.
     * \param ball_vel velocity of ball.
     * \param kick_angle direction of acceleration.
     * \param max_accel maximum modulus of acceleration.
     * \return maximum speed.
     */
    double GetOneKickMaxSpeed(const Vector & ball_vel, const double & kick_angle, const double & max_accel)
    {
    	SinCosT value = SinCos(ball_vel.Dir() - kick_angle);

    	double y = Sin(value) * ball_vel.Mod();

    	if (fabs(y) < max_accel) {
    		double x = Cos(value) * ball_vel.Mod();
    		return (MinMax(0.0, Sqrt(max_accel * max_accel - y * y) + x, ServerParam::instance().ballSpeedMax()));
    	}
    	else {
    		return 0.0;
    	}
    }

    /** ��֪����������󣬸ú��������� */
    bool KickBall(Agent & agent, const Vector & target, double speed_out, int cycle, bool is_shoot = false);

    /** �����ǵõ�ActionInfo�ӿڵĺ��� */
	AtomicAction GetOneKickAction(const Agent & agent, const Vector & ball_pos, const Vector & ball_vel, const Vector & target, bool is_self = true);
	AtomicAction GetKickOutAction(const Agent & agent, Vector & ball_pos, Vector & ball_vel, bool is_self = true);
    AtomicAction GetMaxKickOutAction(const Agent & agent, Vector & ball_pos, Vector & ball_vel, bool is_self = true);
    AtomicAction GetTurnAction(const Agent & agent);

    /** һ��������Ͷ����������Լ�ת��Ĺ滮 */
    ActionPlan OneCycleKick(const Agent & agent, bool is_shoot = false);
    ActionPlan MultiCycleKick(const Agent & agent, int cycle);
    AtomicAction TurnPlan(const Agent & agent, int index, double turn_max_speed);

private:
    enum {
    	STEP_KICK_ANGLE = 10, // Kicker���������ǶȵĲ���������360 / 10 = 36���Ƕ�
    	POINTS_NUM = 81 // Kicker�����������ܵ�����POINTS_NUM = nlayer[0] + nlayer[1] + nlayer[2]
    };

    AgentID     mAgentID;

    Array<int, 3>  mNlayer;         /** ÿ�����ɢ���� */
    double      mD1;                /** 1,2��뾶�ľ�ֵ */
    double      mD2;                /** 2,3��뾶�ľ�ֵ */
    Array<double, 3> mDlayer;    /** ÿ��İ뾶 */
    Array<Vector, POINTS_NUM> mPoint; /** �洢���е� */

    float mKickerValue[3][36][POINTS_NUM][POINTS_NUM]; /** 2,3,4������36���Ƕȣ�POINTS_NUM���� */ // float���ɣ���ʡ��ռ�ռ�

    ReciprocalCurve mOppCurve;      /** ���ֵ�Ӱ�� */
    ReciprocalCurve mRandCurve;     /** ����Ӱ�� */
    ReciprocalCurve mSpeedCurve;    /** ���ٵ�Ӱ�� */

    ActionInput mInput;                 /** ��¼kick��״̬��Ϣ */
    double      mKickSpeed;             /** kick�������ٶȴ�С */
    Vector      mKickTarget;            /** kick��Ŀ��㣬��player����ϵ�� */

    Array<double, POINTS_NUM> mKickRate;/** ��ÿ������Ա��kick_rate */
    Array<double, POINTS_NUM> mMaxAccel;/** ��ÿ������Ա�ܲ����������ٶ�eff_power */
    Array<double, POINTS_NUM> mRandEva; /** ��ÿ������Աkick������������ */
    Array<double, POINTS_NUM> mPointEva;/** ��ÿ������Աkick��eva */

    double      mMaxRandFactor;         /** ��¼��kick randʱ��һ���� */

    Array<Array<AgentID, 3>, 360 > mMaxSpeedFlag;     /** ��¼�����Ƕȵļ����ǣ� */
    Array<Array<double, 3>, 360 > mMaxSpeed;          /** 360���Ƕȣ�1,2,3��������ܴﵽ������ٶ� */
    Array<Array<Vector, 3>, 360 > mMaxSpeedOutPos;    /** 360���Ƕȣ�1,2,3������´ﵽ����ٶ�ʱ�ĳ����λ�� */
    Array<Array<Vector, 3>, 360 > mMaxSpeedOutFirstPos;
    Array<Array<Vector, 3>, 360 > mMaxSpeedOutLastVel;/** ����ٶȳ���ʱ�����һ�����ڵ��ٶȣ�kickǰ�� */

public:
    /** �����ṩ�й�����agent�Ľӿڣ�ֱ�ӷ���AtomicAction�����ⲿ���� */

    /**
     * ��֤kick��ʹ���ٴﵽ��С
     * Get an atomic action to stop ball or minimum ball speed.
     */
    AtomicAction GetStopBallAction(const Agent & agent, bool forceStop = true);

    /**
     * ������ٵ��ض���Ŀ���ٶ�
     * Accelerate the ball to a certain velocity.
     */
    AtomicAction GetAccelerateBallAction(const Agent & agent, Vector & goal_vel);

    /**
     * ���������ض�����
     * Kick the ball to a certain direction.
     */
    AtomicAction GetKickBallToAngleAction(const Agent & agent, AngleDeg rel_angle, double rel_dist_rate = 0.9);
};


#endif

