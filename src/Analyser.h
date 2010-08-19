#ifndef ANALYSER_H_
#define ANALYSER_H_

#include <vector>
#include "Utilities.h"
#include "Geometry.h"
#include "ServerParam.h"
#include "DecisionData.h"
#include "InterceptInfo.h"

class Agent;

/**
 * ��Ҫ�Ƿ�����Ҫʹ�õ�һЩ����
 * */
class Analyser: public DecisionData {
public:
	Analyser(Agent & agent);
	virtual ~Analyser();

	void UpdateRoutine();

public:
	/**
	 * broadcast the position of ball & opponents
	 */
	void BroadcastPosition();

	Vector mLightHouse;
	PlayerArray<Vector, true> mHome;
};

#endif /* ANALYSER_H_ */
