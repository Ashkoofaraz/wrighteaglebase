#include "Client.h"
#include "DynamicDebug.h"
#include "Formation.h"
#include "Kicker.h"
#include "CommandSender.h"
#include "Parser.h"
#include "Thread.h"
#include "UDPSocket.h"
#include "WorldModel.h"
#include "Agent.h"
#include "Logger.h"
#include "CommunicateSystem.h"
#include "NetworkTest.h"
#include "Dasher.h"
#include "Tackler.h"
#include "TimeTest.h"
#include "VisualSystem.h"
#include "InterceptModel.h"
#include "Plotter.h"

Client::Client() {
	srand(time(0)); //global srand once
	srand48(time(0));

	/** Observer and World Model */
	mpAgent         = 0;
    mpObserver      = new Observer;
	mpWorldModel    = new WorldModel;

	/** Parser thread and CommandSend thread */
	mpParser        = new Parser(mpObserver);
	mpCommandSender = new CommandSender(mpObserver);

    // instance�������ﴴ���Խ�ʡ���������Ч�ʣ�����instance���ܷ��������Ҫʱ����

    /** Assistant instance */
	TimeTest::instance();
    NetworkTest::instance();
    DynamicDebug::instance();
    Logger::instance().Initial(mpObserver, &(mpWorldModel->World(false)));
    Plotter::instance();

    /** Param */
    ServerParam::instance();
    PlayerParam::instance();

	/** Base Model */
    InterceptModel::instance();

    /** Smart Action */
    Dasher::instance();
    Tackler::instance();
    Kicker::instance();

    /** Other Useful instance */
    BehaviorFactory::instance();
    UDPSocket::instance();
    VisualSystem::instance();
    CommunicateSystem::instance();
}

Client::~Client()
{
	delete mpCommandSender;
	delete mpParser;

	delete mpObserver;
	delete mpWorldModel;
	delete mpAgent;
}

void Client::RunDynamicDebug()
{
	static char msg[MAX_MESSAGE];
	DynamicDebug::instance().Initial(mpObserver); // ��̬���Եĳ�ʼ����ע��λ�ò����ƶ�

	DynamicDebug::instance().Run(msg); // ��ʼ����Ϣ
	mpParser->ParseInitializeMsg(msg);

	ConstructAgent();

	MessageType msg_type;

	bool first_parse = true;

	while (true)
	{
		msg_type = DynamicDebug::instance().Run(msg);

		switch (msg_type)
		{
		case MT_Parse:
			if (first_parse) {
				mpObserver->Reset();
				first_parse = false;
			}
			mpParser->Parse(msg);
			break;
		case MT_Run:
			Run();
			Logger::instance().Flush(); //flush log
			mpObserver->SetPlanned();
			break;
		case MT_Send:
			mpCommandSender->Run();
			first_parse = true;
			break;
		default:
			return;
		}
	}
}

void Client::RunNormal()
{
	mpParser->Start(); //�����̣߳�����server��������Ϣ
	mpCommandSender->Start(); //���������̣߳���server������Ϣ
	Logger::instance().Start(); //log�߳�

	int past_cycle = 0;
	do
	{
		WaitFor(100); // wait for the parser thread to connect the server
		if (++past_cycle > 20)
		{
			std::cout << PlayerParam::instance().teamName() << ": Connect Server Error ..." << std::endl;
			return;
		}
	} while (mpParser->IsConnectServerOk() == false);

	ConstructAgent();

	SendOptionToServer();

	MainLoop();

	WaitFor(mpObserver->MyUnum() * 100);
    if (mpObserver->MyUnum() == 0)
    {
        std::cout << PlayerParam::instance().teamName() << " Coach: Bye ..." << std::endl;
    }
    else
    {
        std::cout << PlayerParam::instance().teamName() << " " << mpObserver->MyUnum() << ": Bye ..." << std::endl;
    }
}

void Client::ConstructAgent()
{
	Assert(mpAgent == 0);
	mpAgent = new Agent(mpObserver->MyUnum(), mpWorldModel, false); //Ҫ֪��������ܳ�ʼ��

	Formation::instance.AssignWith(mpAgent);
	mpCommandSender->RegisterAgent(mpAgent);
	CommunicateSystem::instance().Initial(mpObserver , mpAgent); //init communicate system
	VisualSystem::instance().Initial(mpAgent);
}

void Client::MainLoop()
{
	while (mpObserver->WaitForNewInfo()) // �ȴ����Ӿ�
	{
        NetworkTest::instance().AddDecisionBegin();

        // ����������bye ...
        if (mpObserver->GetPlayMode() == PM_Time_Over)
        {
            mpAgent->CheckCommands(mpObserver);
            mpAgent->Bye();

            mpObserver->SetPlanned();
            mpObserver->SetCommandSend();
            Logger::instance().SetFlushCond();

            break;
        }

        DynamicDebug::instance().AddMessage("\0", MT_Run); // ��̬���Լ�¼Run��Ϣ
        Run();

		mpObserver->SetPlanned();
		mpObserver->SetCommandSend(); //���ѷ���������߳�
		Logger::instance().SetFlushCond(); // set flush cond and let the logger thread flush the logs to file.

        NetworkTest::instance().AddDecisionEnd(mpObserver->CurrentTime());
	}
}
