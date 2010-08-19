#ifndef __TimeTest_H__
#define __TimeTest_H__


#include <vector>
#include <cstring>
#include "Utilities.h"

/**
 * ���Ժ�������ʱ��Ľӿ�
 * Interface to calculate the time cost of a function.
 */
#define TIMETEST(func_name) TimeTestFunc time_test_func(func_name);


/**
 * TimeCost.
 */
struct TimeCost
{
    /**
     * Constructor.
     */
    TimeCost()
    {
        mNum        = 0;
        mAveCost    = 0;
        mMaxTime    = Time(-3, 0);
        mMaxCost    = 0;
        mMinTime    = Time(-3, 0);
        mMinCost    = 1000000;
    }

	/**
	 * ��������
	 * Number of times a function was called.
	 */
    long    mNum;

    /**
     * ƽ����ʱ
     * Average cost.
     */
    double  mAveCost; //

    /**
     * ����ʱ��Ӧ������
     * Cycle which the highest cost was generated at.
     */
    Time    mMaxTime;

    /**
     * ����ʱ
     * Highest time cost.
     */
    long    mMaxCost;

    /**
     * ��С��ʱ��Ӧ������
     * Cycle which the lowest cost was generated at.
     */
    Time    mMinTime;

    /**
     * ��С��ʱ
     * Lowest time cost.
     */
    long    mMinCost;
};


/**
 * TimeRecord.
 */
struct TimeRecord
{
	/**
	 * Constructor.
	 */
    TimeRecord()
    {
        mBeginTime      = RealTime(0, 0);
        mCycleTimeCost  = 0;
    }

    /**
	 * ÿ���ڵ���Ϣ
	 * Time cost for every cycle.
	 */
    TimeCost    mEachCycle;

    /**
     * ÿһ�ε���Ϣ
     * Time cost for each call.
     */
    TimeCost    mEachTime;

    /**
     * ÿһ�εĿ�ʼʱ��
     * Begin time for each call.
     */
    RealTime    mBeginTime;

    /**
     * ÿһ�����ܹ�����ʱ��
     * Total cost for each cycle.
     */
    long        mCycleTimeCost;
};


/**
 * TimeTest.
 */
class TimeTest
{
    TimeTest();

public:
    ~TimeTest();

    /**
     * ����ʵ��
     * Instacne.
     */
    static TimeTest & instance();

    /**
     * �µ����ڵ�������и��£������current_timeΪ����ǰ���Ǹ�����
     * Update when each cycle begins.
     */
    void Update(Time current_time);

    /**
     * ÿ�β��ԵĿ�ʼ
     * Begin of each test.
     */
    int Begin(std::string func_name);

    /**
     * ÿ�β��ԵĽ���
     * End of each test.
     */
    void End(int event_id);

    /**
     * �����Լ��ĺ��룬�����ļ����ĸ�ֵ
     * Set self unum, which record files will be named after.
     */
    inline void SetUnum(int unum) { mUnum = unum; }

private:
    std::vector<std::string>    mEventQueue; // ��¼���Ժ���������
    std::vector<TimeRecord>     mRecordQueue; // ��¼���ԵĽ��
    std::vector<bool>           mIsExecute; // ��¼��ǰ�����Ƿ�ִ�й�
    std::vector<bool>           mIsBegin; // ������֤Begin()��End()����ִ��

    Time        mUpdateTime; // �ϴ�update������
    int         mUnum; // �Լ��ĺ���
};


/**
 * TimeTestFunc
 */
class TimeTestFunc
{
public:
    TimeTestFunc(std::string func_name);
    ~TimeTestFunc();

private:
    int mEventID;
};


#endif

