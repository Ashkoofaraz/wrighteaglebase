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

#include "Utilities.h"
#include "PlayerParam.h"
#include "DynamicDebug.h"

timeval RealTime::mStartTime = GetRealTime();
const long RealTime::ONE_MILLION = 1000000;

timeval GetRealTime() {
	timeval time_val;

#ifdef WIN32
	LARGE_INTEGER frq,count;
	QueryPerformanceFrequency(&frq);
	QueryPerformanceCounter(&count);
	time_val.tv_sec = (count.QuadPart / frq.QuadPart);
	time_val.tv_usec = (count.QuadPart % frq.QuadPart) * 1000000 / frq.QuadPart;
#else
	gettimeofday(&time_val, 0);
#endif

	return time_val;
}

timeval GetRealTimeParser() {
	if (PlayerParam::instance().DynamicDebugMode() == true) {
		return DynamicDebug::instance().GetTimeParser();
	}

	timeval time_val = GetRealTime();
	DynamicDebug::instance().AddTimeParser(time_val);
	return time_val;
}

timeval GetRealTimeDecision() {
	if (PlayerParam::instance().DynamicDebugMode() == true) {
		return DynamicDebug::instance().GetTimeDecision();
	}

	timeval time_val = GetRealTime();
	DynamicDebug::instance().AddTimeDecision(time_val);
	return time_val;
}

timeval GetRealTimeCommandSend() {
	if (PlayerParam::instance().DynamicDebugMode() == true) {
		return DynamicDebug::instance().GetTimeCommandSend();
	}

	timeval time_val = GetRealTime();
	DynamicDebug::instance().AddTimeCommandSend(time_val);
	return time_val;
}

RealTime RealTime::operator +(const RealTime &t) const {
	if (GetUsec() + t.GetUsec() >= ONE_MILLION) {
		return RealTime(GetSec() + t.GetSec() + 1, GetUsec() + t.GetUsec()
				- ONE_MILLION);
	} else {
		return RealTime(GetSec() + t.GetSec(), GetUsec() + t.GetUsec());
	}
}

RealTime RealTime::operator +(int msec) const {
	int usec = GetUsec() + msec * 1000;
	if (usec >= ONE_MILLION) {
		return RealTime(GetSec() + usec / ONE_MILLION, usec % ONE_MILLION);
	} else {
		return RealTime(GetSec(), usec);
	}
}

RealTime RealTime::operator -(int msec) const {
	int usec = GetUsec() - msec * 1000;
	if (usec < 0) {
		usec = -usec;
		return RealTime(GetSec() - usec / ONE_MILLION - 1, ONE_MILLION - usec
				% ONE_MILLION);
	} else {
		return RealTime(GetSec(), usec);
	}
}

int RealTime::operator -(const RealTime &t) const {
	return (GetSec() - t.GetSec()) * 1000 + int((GetUsec() - t.GetUsec())
			/ 1000.0);
}

long RealTime::Sub(const RealTime &t) {
	return (GetSec() - t.GetSec()) * 1000000 + GetUsec() - t.GetUsec();
}

bool RealTime::operator <(const RealTime &t) const {
	if (GetSec() < t.GetSec())
		return true;
	if (GetSec() == t.GetSec() && GetUsec() < t.GetUsec())
		return true;
	return false;
}

bool RealTime::operator >(const RealTime &t) const {
	if (GetSec() > t.GetSec())
		return true;
	if (GetSec() == t.GetSec() && GetUsec() > t.GetUsec())
		return true;
	return false;
}

bool RealTime::operator ==(const RealTime &t) const {
	if (GetSec() == t.GetSec() && GetUsec() == t.GetUsec())
		return true;
	return false;
}

bool RealTime::operator !=(const RealTime &t) const {
	if (GetSec() == t.GetSec() && GetUsec() == t.GetUsec())
		return false;
	return true;
}

std::ostream & operator <<(std::ostream &os, RealTime t) {
	return os << t - RealTime::mStartTime;
}

Time Time::operator-(const int a) const {
	int news = S() - a;
	if (news >= 0) {
		return Time(T(), news);
	}
	return Time(T() + news, 0);
}

int Time::operator-(const Time &a) const {
	if (mT == a.T()) {
		return mS - a.S();
	} else {
		return mT - a.T() + mS; //ʵ�ʿ��ܲ����,��׼
	}
}

ServerPlayModeMap & ServerPlayModeMap::instance() {
	static ServerPlayModeMap server_playmode_parser;
	return server_playmode_parser;
}

ServerPlayModeMap::ServerPlayModeMap() {
	Bind("before_kick_off", SPM_BeforeKickOff);
	Bind("time_over", SPM_TimeOver);
	Bind("play_on", SPM_PlayOn);
	Bind("kick_off_l", SPM_KickOff_Left);
	Bind("kick_off_r", SPM_KickOff_Right);
	Bind("kick_in_l", SPM_KickIn_Left);
	Bind("kick_in_r", SPM_KickIn_Right);
	Bind("free_kick_l", SPM_FreeKick_Left);
	Bind("free_kick_r", SPM_FreeKick_Right);
	Bind("corner_kick_l", SPM_CornerKick_Left);
	Bind("corner_kick_r", SPM_CornerKick_Right);
	Bind("goal_kick_l", SPM_GoalKick_Left);
	Bind("goal_kick_r", SPM_GoalKick_Right);
	Bind("goal_l", SPM_AfterGoal_Left);
	Bind("goal_r", SPM_AfterGoal_Right);
	Bind("drop_ball", SPM_Drop_Ball);
	Bind("offside_l", SPM_OffSide_Left);
	Bind("offside_r", SPM_OffSide_Right);
	Bind("penalty_kick_l", SPM_PK_Left);
	Bind("penalty_kick_r", SPM_PK_Right);
	Bind("first_half_over", SPM_FirstHalfOver);
	Bind("pause", SPM_Pause);
	Bind("human_judge", SPM_Human);
	Bind("foul_charge_l", SPM_Foul_Charge_Left);
	Bind("foul_charge_r", SPM_Foul_Charge_Right);
	Bind("foul_push_l", SPM_Foul_Push_Left);
	Bind("foul_push_r", SPM_Foul_Push_Right);
	Bind("foul_multiple_attack_l", SPM_Foul_MultipleAttacker_Left);
	Bind("foul_multiple_attack_r", SPM_Foul_MultipleAttacker_Right);
	Bind("foul_ballout_l", SPM_Foul_BallOut_Left);
	Bind("foul_ballout_r", SPM_Foul_BallOut_Right);
	Bind("back_pass_l", SPM_Back_Pass_Left);
	Bind("back_pass_r", SPM_Back_Pass_Right);
	Bind("free_kick_fault_l", SPM_Free_Kick_Fault_Left);
	Bind("free_kick_fault_r", SPM_Free_Kick_Fault_Right);
	Bind("catch_fault_l", SPM_CatchFault_Left);
	Bind("catch_fault_r", SPM_CatchFault_Right);
	Bind("indirect_free_kick_l", SPM_IndFreeKick_Left);
	Bind("indirect_free_kick_r", SPM_IndFreeKick_Right);
	Bind("penalty_setup_l", SPM_PenaltySetup_Left);
	Bind("penalty_setup_r", SPM_PenaltySetup_Right);
	Bind("penalty_ready_l", SPM_PenaltyReady_Left);
	Bind("penalty_ready_r", SPM_PenaltyReady_Right);
	Bind("penalty_taken_l", SPM_PenaltyTaken_Left);
	Bind("penalty_taken_r", SPM_PenaltyTaken_Right);
	Bind("penalty_miss_l", SPM_PenaltyMiss_Left);
	Bind("penalty_miss_r", SPM_PenaltyMiss_Right);
	Bind("penalty_score_l", SPM_PenaltyScore_Left);
	Bind("penalty_score_r", SPM_PenaltyScore_Right);

	Bind("goalie_catch_ball_l", SPM_GoalieCatchBall_Left);
	Bind("goalie_catch_ball_r", SPM_GoalieCatchBall_Right);
	Bind("foul_l", SPM_Foul_Left);
	Bind("foul_r", SPM_Foul_Right);
	Bind("penalty_onfield_l", SPM_PenaltyOnfield_Left);
	Bind("penalty_onfield_r", SPM_PenaltyOnfield_Right);
	Bind("penalty_foul_l", SPM_PenaltyFoul_Left);
	Bind("penalty_foul_r", SPM_PenaltyFoul_Right);
	Bind("penalty_winner_l", SPM_PenaltyWinner_Left);
	Bind("penalty_winner_r", SPM_PenaltyWinner_Right);
	Bind("half_time", SPM_HalfTime);
	Bind("time_up", SPM_TimeUp);
	Bind("time_extended", SPM_TimeExtended);

	Assert(mString2Enum.size() == SPM_MAX - 1);
}

void ServerPlayModeMap::Bind(const std::string & str, ServerPlayMode spm) {
	Assert(mString2Enum.count(str) == 0);
	Assert(mEnum2String.count(spm) == 0);

	mString2Enum[str] = spm;
	mEnum2String[spm] = str;
}

ServerPlayMode ServerPlayModeMap::GetServerPlayMode(const std::string & str) {
	//special case: server ���������� goal_[lr]_[[:digit:]]
	if (strncmp(str.c_str(), "goal_l", strlen("goal_l")) == 0) {
		return mString2Enum["goal_l"];
	}
	else if (strncmp(str.c_str(), "goal_r", strlen("goal_r")) == 0) {
		return mString2Enum["goal_r"];
	}
	else if (mString2Enum.count(str)) {
		return mString2Enum[str];
	}
	else {
		Assert(!"server playmode error");
		return SPM_Null;
	}
}

const char * ServerPlayModeMap::GetPlayModeString(ServerPlayMode spm) {
	if (mEnum2String.count(spm)) {
		return mEnum2String[spm].c_str();
	}
	else {
		return "";
	}
}

