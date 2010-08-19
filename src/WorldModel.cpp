#include "WorldModel.h"
#include "Observer.h"
#include "WorldState.h"
#include "InfoState.h"

WorldModel::WorldModel() {
	mpHistoryState[0] = new HistoryState;
	mpWorldState[0] = new WorldState(mpHistoryState[0]); //���ѵ�����״̬

	mpHistoryState[1] = new HistoryState;
	mpWorldState[1] = new WorldState(mpHistoryState[1]); //������ʱ�õģ����ֵ�����״̬
}

WorldModel::~WorldModel() {
	delete mpWorldState[0];
	delete mpHistoryState[0];

	delete mpWorldState[1];
	delete mpHistoryState[1];
}

void WorldModel::Update(Observer *observer, BeliefState *belief_state)
{
	//�洢һ�µ�ǰ������
	mpHistoryState[0]->UpdateHistory(*mpWorldState[0]);
	mpHistoryState[1]->UpdateHistory(*mpWorldState[1]);

	mpWorldState[0]->UpdateFromObserver(observer, belief_state); //�Լ�������ʹ�õ�����״̬
	mpWorldState[1]->GetReverseFrom(mpWorldState[0]); //�ɹ��������ʱʹ�õ�����״̬
}

const WorldState & WorldModel::GetWorldState(bool reverse) const
{
	return *(reverse? mpWorldState[1]: mpWorldState[0]);
}

WorldState & WorldModel::World(bool reverse)
{
	return *(reverse? mpWorldState[1]: mpWorldState[0]);
}

