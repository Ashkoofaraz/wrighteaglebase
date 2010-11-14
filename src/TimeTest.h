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

