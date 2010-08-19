#ifndef INTERCEPTMODEL_H_
#define INTERCEPTMODEL_H_

#include "Geometry.h"
#include "ServerParam.h"

class PlayerState;
class WorldState;

class InterceptModel {
	InterceptModel();

public:
	virtual ~InterceptModel();
	static InterceptModel &instance();

	static const double IMPOSSIBLE_BALL_SPEED;

public:
	struct InterceptSolution
	{
		InterceptSolution ():
			tangc (0),
			interc (1)
		{

		}

		int tangc;     	//�е���Ŀ {0, 1, 2}
		int interc;		//������Ŀ {1, 3}

		Array<double, 2> tangp;		//���е�(0)�����е�(1)
		Array<double, 2> tangv;		//���е��ٶ�(0)�����е��ٶ�v
		Array<double, 3> interp;	//�����������
		Array<double, 3> intert;	//���㴦��Ӧ��������
	};

public:
	/**
	 * ����ĺ�������������������ģ��
	 */
	void CalcInterception(const Vector & ball_pos, const Vector & ball_vel, const double buffer, const PlayerState * player, InterceptSolution * sol);
	int CalcTangPoint(double x0, double y0, double vp, double ka, double cd, InterceptSolution * sol);
	double CalcInterPoint(double x_init, double x0, double y0, double vb, double vp, double ka, double cd);

private:
	/**
	 * ���������������
	 * @param x0
	 * @param y0
	 * @param v0
	 * @param vp
	 * @param ka
	 * @param cd
	 */
	void PlotInterceptCurve(double x0, double y0, double v0, double vp, double ka, double cd, double max_x);
};

#endif /* INTERCEPTMODEL_H_ */
