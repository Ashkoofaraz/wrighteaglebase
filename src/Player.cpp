#include "Player.h"
#include "DecisionTree.h"
#include "DynamicDebug.h"
#include "Formation.h"
#include "CommandSender.h"
#include "Parser.h"
#include "Thread.h"
#include "UDPSocket.h"
#include "WorldModel.h"
#include "Agent.h"
#include "VisualSystem.h"
#include "Logger.h"
#include "BeliefState.h"
#include "CommunicateSystem.h"
#include "TimeTest.h"
#include "Dasher.h"

Player::Player():
	mpDecisionTree( new DecisionTree )
{
}

Player::~Player()
{
	delete mpDecisionTree;
}

void Player::SendOptionToServer()
{
	while (!mpParser->IsClangOk())
	{
		mpAgent->CheckCommands(mpObserver);
		mpAgent->Clang(7, 8);
		mpObserver->SetCommandSend();
		WaitFor(200);
	}

	while (!mpParser->IsSyncOk())
	{
		mpAgent->CheckCommands(mpObserver);
		mpAgent->SynchSee();
		mpObserver->SetCommandSend();
		WaitFor(200);
	}

	mpAgent->CheckCommands(mpObserver);
	mpAgent->EarOff(false);
	mpObserver->SetCommandSend();
	WaitFor(200);
}

void Player::Run()
{
    //TIMETEST("Run");

	static Time last_time = Time(-100, 0);

	mpObserver->Lock();

	/** ���漸������˳���ܱ� */
	Formation::instance.SetTeammateFormations();
	CommunicateSystem::instance().Update(); //���������hear��Ϣ���������ȸ���
	mpAgent->CheckCommands(mpObserver);
	mpWorldModel->Update(mpObserver, mpBeliefState); //�����ͬʱ��������״̬������״̬

	mpObserver->UnLock();

    const Time & time = mpAgent->GetWorldState().CurrentTime();

	if (last_time.T() >= 0) {
		if (time != Time(last_time.T() + 1, 0) && time != Time(last_time.T(), last_time.S() + 1)) {
			if (time == last_time) {
				mpAgent->World().SetCurrentTime(Time(last_time.T(), last_time.S() + 1)); //����������ݸ��»������
			}
		}
	}

	last_time = time;

	Formation::instance.UpdateOpponentRole(); //TODO: ��ʱ�����������δ��������������Ϣʱ�Լ��ȼ���

	VisualSystem::instance().ResetVisualRequest();
	mpDecisionTree->Decision(*mpAgent);

	mpBeliefState->Spread(*mpAgent); //��ɢһ���ڣ����Ӿ�ʹ��

	VisualSystem::instance().Decision();
	CommunicateSystem::instance().Decision();


	Logger::instance().LogSight();
}
