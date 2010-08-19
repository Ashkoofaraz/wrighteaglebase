#ifndef WORLDMODEL_H_
#define WORLDMODEL_H_

class Observer;
class WorldState;
class HistoryState;
class BeliefState;

/**
 * WorldModel ���������� WorldState��һ�����ڶ��ѵľ��ߣ���һ�����ڶ��ֵľ���
 */
class WorldModel {
	WorldModel(WorldModel &);
public:
	WorldModel();
	virtual ~WorldModel();

	void Update(Observer *observer, BeliefState *belief_state);

	const WorldState & GetWorldState(bool reverse) const;
	WorldState       & World(bool reverse);

private:
	WorldState *mpWorldState[2];
	HistoryState *mpHistoryState[2];
};

#endif /* WORLDMODEL_H_ */
