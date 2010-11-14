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

#ifndef __BASESTATE_H__
#define __BASESTATE_H__


#include "ServerParam.h"
#include "PlayerParam.h"
#include <cstring>

//�������ֵ
template<class T>
class StateValue
{
public:
	StateValue():
		mValue (T()),
		mCycleDelay(9999),
		mConf(0)
	{
	}

	StateValue(const StateValue & o) {
		mValue = o.mValue;
		mCycleDelay = o.mCycleDelay;
		mConf = o.mConf;
	}

	/**�ڲ��Ը���
	* @param ÿ����delay���ϵ�����
	* @param ÿ����conf˥��������*/
	void AutoUpdate(int delay_add = 1 , double conf_dec_factor = 1)
	{
		mCycleDelay += delay_add;
		mConf *= conf_dec_factor;
	}

public:
	/**ֵ��С*/
	T mValue;

	/**ֵ�൱ǰʱ��*/
	int mCycleDelay;

	/**ֵ�Ŀ��Ŷ�*/
	double mConf;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/**�������BaseState��*/
class BaseState
{
public:
	BaseState()
	{
		mPos.mValue = Vector(10000 , 10000);
		mPosEps = 10000;
	}

	BaseState(const BaseState & o) {
		mPos = o.mPos;
	}

	/**����λ��
	* @param λ��
	* @param ʱ��
	* @param ���Ŷ�*/
	void UpdatePos(const Vector & pos , int delay = 0, double conf = 1);

	/**��õ�ǰ��ǰ���ε�λ��
	* @param ����ֵ֮ǰ������������ΧΪ0��RECORDSIZE-1,�����ѭ��
	*/
	const Vector & GetPos() const { return mPos.mValue; }

	/**��õ�ǰ��ǰ���ε�ʱ��
	* @param ����ֵ֮ǰ������������ΧΪ0��RECORDSIZE-1,�����ѭ��
	*/
	int GetPosDelay() const { return mPos.mCycleDelay; }

	/**��õ�ǰ��ǰ���ε����Ŷ�
	* @param ����ֵ֮ǰ������������ΧΪ0��RECORDSIZE-1,�����ѭ��
	*/
	const double & GetPosConf() const { return mPos.mConf; }


	/**
	 *  ����pos����Χ,��Ҫָ���Բ�İ뾶
	 */
	void UpdatePosEps(double eps)  { mPosEps = eps;}

	/**
	 * ���pos������eps
	 */
	const double & GetPosEps() const { return mPosEps;}

	/**�ڲ��Ը���
	* @param ÿ����delay���ϵ�����
	* @param ÿ����conf˥��������*/
	void AutoUpdate(int delay_add = 1 , double conf_dec_factor = 1);
private:
	/**�洢�����ڵ�λ��*/
	StateValue<Vector> mPos;

	double mPosEps;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
/**��̬�������*/
/**ע����ʱû�뵽��ʲô��Ϣ*/
class StaticState: public BaseState
{
};

class MobileState: public BaseState
{
public:
	MobileState(double decay, double effective_speed_max):
		mDecay(decay),
		mEffectiveSpeedMax(effective_speed_max),
		mpPredictor(new Predictor(mDecay)),
		mGuessed (0)
	{
		mVelEps = 10000;
		ResetPredictor();
	}

    MobileState(const MobileState &m):
    	BaseState(m),
    	mDecay(m.GetDecay()),
    	mEffectiveSpeedMax(m.GetEffectiveSpeedMax()),
    	mpPredictor(new Predictor(mDecay)),
    	mGuessed (0)
    {
		mVel = m.mVel;
		mVelEps = m.mVelEps;
		UpdatePos(m.GetPos(), m.GetPosDelay(), m.GetPosConf());
	}

	virtual ~MobileState() { delete mpPredictor; }

	void UpdatePos(const Vector & pos , int delay = 0, double conf = 1.0);

	const MobileState & operator=(const MobileState &v)
	{
		mVel = v.mVel;
		mDecay = v.mDecay;

		UpdatePos(v.GetPos(), v.GetPosDelay(), v.GetPosConf()); //use old predictor

		mEffectiveSpeedMax = v.mEffectiveSpeedMax;

		return *this;
	}

	/**�����ٶ�
	* @param �ٶ�
	* @param ʱ��
	* @param ���Ŷ�*/
	void UpdateVel(const Vector & vel , int delay = 0, double conf = 1);

	/**��õ�ǰ��ǰ���ε��ٶ�
	* @param ����ֵ֮ǰ������������ΧΪ0��RECORDSIZE-1,�����ѭ��
	*/
	const Vector & GetVel() const { return mVel.mValue; }

	/**��õ�ǰ��ǰ���ε��ٶȵ�ʱ��
	* @param ����ֵ֮ǰ������������ΧΪ0��RECORDSIZE-1,�����ѭ��
	*/
	int GetVelDelay() const { return mVel.mCycleDelay; }

	/**��õ�ǰ��ǰ���ε��ٶȵ����Ŷ�
	* @param ����ֵ֮ǰ������������ΧΪ0��RECORDSIZE-1,�����ѭ��
	*/
	const double & GetVelConf() const { return mVel.mConf; }

	/**
	 * �����ٶ����
	 */
	void UpdateVelEps(double eps) { mVelEps = eps;}

	/**
	 * ����ٶ����
	 */
	double GetVelEps() const { return mVelEps;};

	/**
	* �õ��˶� decay
	* @return
	*/
	const double & GetDecay() const { return mDecay; }

	void SetDecay(const double & decay) { mDecay = decay; }

	/**
	* ʵ������ٶ�
	*/
	const double & GetEffectiveSpeedMax() const { return mEffectiveSpeedMax; }
	void SetEffectiveSpeedMax(double effective_speed_max) { mEffectiveSpeedMax = effective_speed_max; }

	/**�ڲ��Ը���
	* @param ÿ����delay���ϵ�����
	* @param ÿ����conf˥��������*/
	void AutoUpdate(int delay_add = 1 , double conf_dec_factor = 1);

public:
	class Predictor {
	public:
		Predictor(const double & decay): mDecay(decay) {
		}

		enum {
			MAX_STEP = 50
		};

		const Vector & GetPredictedPos(int step = 1){
			step = Min(step, int(MAX_STEP));

			if (step <= mPredictedStep){
				return mPredictedPos[step];
			}
			else {
				PredictTo(step);

				return mPredictedPos[step];
			}
		}	

		const Vector & GetPredictedVel(int step = 1){
			step = Min(step, int(MAX_STEP));

			if (step <= mPredictedStep){
				return mPredictedVel[step];
			}
			else {
				PredictTo(step);

				return mPredictedVel[step];
			}
		}

		void UpdatePosAndVel(const Vector & pos, const Vector & vel) {
			mPredictedPos[0] = pos;
			mPredictedVel[0] = vel;
			mPredictedStep = 0;
		}

	private:
		void PredictTo(const int & step) {
			for (int i = mPredictedStep + 1; i <= step; ++i){
				mPredictedPos[i] = mPredictedPos[i-1] + mPredictedVel[i-1];
				mPredictedVel[i] = mPredictedVel[i-1] * mDecay;
			}
			mPredictedStep = step;
		}

		const double mDecay;

		Array<Vector, MAX_STEP + 1> mPredictedPos;
		Array<Vector, MAX_STEP + 1> mPredictedVel;
		int mPredictedStep;
	};

	void ResetPredictor() {
		mpPredictor->UpdatePosAndVel(GetPos(), GetVel());
	}

public:
	const Vector & GetPredictedPos(int step = 1) const {
		return mpPredictor->GetPredictedPos(step);
	}

	const Vector & GetPredictedVel(int step = 1) const {
		return mpPredictor->GetPredictedVel(step);
	}

	Vector GetFinalPos() const { return GetPos() + GetVel() / (1.0 - GetDecay()); }

public:
	//guessed time is now only for WorldStateUpdater , other should not use it;
	/** set guessed times*/
	void UpdateGuessedTimes(int guessed)
	{
		Assert(guessed >= 0);
		mGuessed = guessed;
	}

	/** get guessed times*/
	int GetGuessedTimes() const
	{
		return mGuessed;
	}

private:
	/**�洢�������ڵ��ٶ�*/
	StateValue<Vector> mVel;

	double mVelEps;

	double mDecay;
	double mEffectiveSpeedMax;

private:
	Predictor *mpPredictor;

	int mGuessed; //indicate that the times of this object ot be guessed -- only used from WorldStateUpdater::Run
};
#endif

