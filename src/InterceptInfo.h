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

#ifndef __InterceptInfo_H__
#define __InterceptInfo_H__

#include "InfoState.h"
#include "InterceptModel.h"

enum InterceptRes { //���ؽ��
	IR_None,

	IR_Failure,
	IR_Success
};

//���ݼ�ģ�����õ��Ľ�����Ϣ
class PlayerInterceptInfo
{
private:
	InterceptModel::InterceptSolution solution;
	friend class InterceptInfo;

public:
	PlayerInterceptInfo():
		mTime (Time(-3, 0)),
		mpPlayer (0),
		mMinCycle (1000),
		mInterPos (Vector(1000, 1000)),
		mIntervals (1),
		mRes (IR_Failure)
	{

	}

	Time mTime;		//����ʱ��
	const PlayerState *mpPlayer;
	Array<int, 3> mInterCycle; //������Ľ��������Ӧ��������
	int mMinCycle; //��С�������ڣ�����ǰ������ݼ�����ã�
	Vector mInterPos; //��С���ڶ�Ӧ�Ľ����
	int mIntervals; //������� {1, 2}
	InterceptRes mRes; //����ɹ���ָ���ڳ���֮�ڽص���

	friend std::ostream &operator<<(std::ostream &os, const PlayerInterceptInfo &it){
		if (it.solution.interc == 1){
			return os << "[" << it.mInterCycle[0] << ", $]";
		}
		else {
			return os << "[" << it.mInterCycle[0] << ", " << it.mInterCycle[1] << "] [" << it.mInterCycle[2] << ", $]";
		}
	}
};

struct OrderedIT {
	OrderedIT(PlayerInterceptInfo *pinfo, Unum unum, int cd): mpInterceptInfo(pinfo), mUnum(unum), mCycleDelay(cd) {

	}

	PlayerInterceptInfo *mpInterceptInfo; //ָ��num��player������info,��ЧʱΪNULL
	Unum mUnum; //+ Ϊ���ѣ�- Ϊ����
	int mCycleDelay;   //cycle delay

	bool operator < (const OrderedIT& it) const {
		return (mpInterceptInfo->mMinCycle == it.mpInterceptInfo->mMinCycle)? mCycleDelay < it.mCycleDelay: mpInterceptInfo->mMinCycle < it.mpInterceptInfo->mMinCycle;
	}
};

class InterceptInfo: public InfoStateBase
{
public:
    InterceptInfo(WorldState *pWorldState, InfoState *pInfoState);

    PlayerInterceptInfo *GetPlayerInterceptInfo(Unum unum) const;
    const std::vector<OrderedIT> & GetOIT() const { return mOIT; }
    bool IsPlayerBallInterceptable(Unum unum) const { return GetPlayerInterceptInfo(unum)? GetPlayerInterceptInfo(unum)->mRes == IR_Success: false; } //�����������ǰ���ص���

	static void CalcTightInterception(const BallState & ball, PlayerInterceptInfo *pInfo, bool can_inverse = true); //�����߼��ɽ�`��'�������� -- ����gotopoint����
	static void CalcLooseInterception(const BallState & ball, PlayerInterceptInfo *pInfo, const double & buffer); //���buffer�ɽص�`�ɡ��������� -- ������gotopoint����

private:
	static void CalcIdealInterception(const BallState & ball, PlayerInterceptInfo *pInfo, const double & buffer);
	static void AnalyseInterceptSolution(const BallState & ball, PlayerInterceptInfo *pInfo);

private:
	void UpdateRoutine();

	void SortIntercerptInfo();
	PlayerInterceptInfo *VerifyIntInfo(Unum unum);

private:
    PlayerArray<PlayerInterceptInfo> mTeammateInterceptInfo;
    PlayerArray<PlayerInterceptInfo> mOpponentInterceptInfo;
    std::vector<OrderedIT> mOIT; //������������
};

#endif
