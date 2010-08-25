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

/**
 * 1. ����ļ���Ҫ�Ĺ�����ͨ����sence��Ϣ����ʱ��¼һ��ʱ�䣬���Ӿ���Ϣ����ʱ
 *    ��¼һ��ʱ�䣬������ʱ�����������sense��Ϣ������sight��Ϣ������
 *    ʱ������������ʱ��������Сʱ������ƽ��ֵ�����ͳ�Ʋ�����Ϊ��
 *    ���ȴ��Ӿ���Ϣ�ĵ�����Buffer���ο���ͬʱ�����ڱ�����ʱ�������绷����
 *   ���������ӳ٣������̫��������ȫ���������Ӧ��Ҫ��
 * 2. ͨ����BasicAction.cpp �ڼ�¼����ÿ�����ڵķ��͵������sense ʱ��Ϣ��
 *    ������Server���صĲ���������ֵ�ıȽϿ��Բ�����Ǹ��������Ƿ�©�����
 *    ������һ�����淴Ӧ����������ܡ�
 * 3. ����໹������չ��Ӧ�Ĺ��ܣ�������Բ��Sense ��Sense ��ʱ����������
 *    һ����ķ�Ӧ���绷�������Բ��Sense ��Ϣ�Ƿ�ʧ������Ƿ񶪰����Լ�������
 *    �Ӹ������淴Ӧ�����绷���������Խ�һ��ʵ�֣���
 */
/**
 * 1. This paper's main function is to sence information coming through a time when the record, when the arrival of visual information in
 * Record a time, with two-phase by the time difference. Sense of information obtained after the arrival of, sight of information coming
 * Time interval, derive the maximum time interval, the minimum time interval, the mean, variance. Statistical difference, for me
 * Will be waiting for the arrival of visual information reference the Buffer. At the same time when the game can detect the network environment
 * (And the network delay). If we can raise too much the corresponding requirements.
 * 2. BasicAction.cpp recorded in each cycle we have sent the order when the information in the sense
 * Analysis of the Server to return the difference, compare these two values can be measured in various orders, we missed an order.
 * The re-addition reaction of one aspect of the performance of the network.
 * 3. This class can also be a corresponding expansion of the functions of, for example, can be measured Sense to Sense of time interval, from another
 * On the one hand, the response of the network environment. Sense can detect whether or not the loss of information. Packet loss is detected. And packet loss rate
 * All aspects of the response from the network environment. (Can be further).
 */

#ifndef __NetworkTest_H__
#define __NetworkTest_H__


#include <map>
#include <vector>
#include <cstring>
#include <fstream>
#include "ActionEffector.h"


typedef struct{
	long val;
	Time ValTime;
}TIMEDATA;

typedef struct{
	TIMEDATA 	MinDelay;		//���ֵ����ʱ��
	TIMEDATA 	MaxDelay;		//��Сֵ����ʱ��
	double 		average;         //ƽ��ֵ
	long 		count;				//ͳ�ƴ���
	double 		variance;		//����
	double		quadrature;		//����ԭ���
}COUNTUNIT;

typedef struct{
    std::string Name;		//ϵͳʱ��ļ�¼������
	RealTime RT;		//ϵͳʱ�䣬û���ڵĶ�����
	bool	IsRecord;	//�Ƿ��Ѿ���¼
}TIMEUNIT;


typedef struct
{
	long threshold;
	int count;
	double probility;
}INTERVAL;

typedef struct
{
	int Kicks;
    int Dashs;
    int Turns;
	int Says;
	int TurnNecks;
	int Catchs;
	int Moves;
	int ChangeViews;
	int Pointtos;
	int Attentiontos;
	int Tackles;
}CMDCOUNT;


struct RealTimeRecord
{
    RealTime    mBeginTime; // ��ʼ��ϵͳʱ��
    RealTime    mEndTime;   // ������ϵͳʱ��
    int         mCostTime;   // ���ѵ�ʱ��
    Time        mTime;      // ��ǰ����
};


class StatisticUnit{
public:
	StatisticUnit();
	~StatisticUnit();
	void SetMinVal(TIMEDATA td);
	void SetMaxVal(TIMEDATA td);
	void SetAverage(long val);
	void Setquadrature(long val);
	void SetVariance();
	void SetStatistic(long val, Time current);
	void InterValStacistic(long val);
	void OutPutInterVal(FILE *fp=0);
	void PrintStatic(FILE *fp=0);
	void PrintCommond(FILE *fp);
	void Flush();
	void setCMDData(CMDCOUNT Send, CMDCOUNT Execute)
	{
		CMDSend = Send;
		CMDExecute = Execute;
	}
    void setBegin(std::string BName)
	{
		BeginName = BName;
	}
    void setEndName(std::string EName)
	{
		EndName = EName;
	}
	void setMyID(int ID)
	{
		MyID = ID;
	}
	std::string GetBegin()
	{
		return BeginName;
	}
	std::string GetEndName()
	{
		return EndName;
	}
	void MissBeginIncrement()
	{
		MissBegin++;
	}

private:
	COUNTUNIT CU;
	long InterBase;
	long Increment;
	int MyID;
	int MissBegin;
	CMDCOUNT CMDSend;	//����ͼ�����
	CMDCOUNT CMDExecute; //����ִ�м�����
	std::string	 BeginName;
	std::string	 EndName;
	INTERVAL Interval[9];//<=30,<=40,<=50,<=60,<=70,<=80,<=90,<=100,�������������
};


class NetworkTest{
	NetworkTest();
public:
	~NetworkTest();
    static NetworkTest &instance();

    void SetUnum(Unum unum) { mUnum = unum; }
    void Update(const Time& time);
    void SetCommandExecuteCount(int d,int k,int tu,int s,int tn,int c,int m, int cv,int pt,int tk, int fc);
    void SetCommandSendCount(const CommandInfo& cmd);

    void Begin(const std::string BeginName);
	void End(const std::string BeginName, const std::string EndName);

private:
	std::vector<TIMEUNIT>BeginTimelist;		//��ʼ��ʱ���������ϵͳʱ�伯����;
	std::vector<TIMEUNIT>EndTimelist;		//������ʱ���������ϵͳʱ�伯����;
	std::vector<StatisticUnit>StatUnit;		//ͳ�Ƶ�λ
	std::map<std::string,std::string>EndtoBegin;
	int getNameID(std::string Name, bool tag);
	long getInterval(const std::string BeginName, const std::string EndName);  //ʱ����
	void Setstatistic(const std::string BeginName, const std::string EndName);

private:
	Time CurrentTime;
	int mUnum;
	double Probility;
	long INTV;
	CMDCOUNT CMDSend;	//����ͼ�����
	CMDCOUNT CMDExecute; //����ִ�м�����

public:
    void AddParserBegin();
    void AddParserEnd(Time current_time);
    void AddDecisionBegin();
    void AddDecisionEnd(Time current_time);
    void AddCommandSendBegin();
    void AddCommandSendEnd(Time current_time);

    void WriteRealTimeRecord();

private:
    std::vector<RealTimeRecord> mParserList;
    std::vector<RealTimeRecord> mDecisionList;
    std::vector<RealTimeRecord> mCommandSendList;

    RealTimeRecord  mParserRecord;
    RealTimeRecord  mDecisionRecord;
    RealTimeRecord  mCommandSendRecord;
};


#endif

