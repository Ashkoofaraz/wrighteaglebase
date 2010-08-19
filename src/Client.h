#ifndef CLIENT_H_
#define CLIENT_H_

class Observer;
class WorldModel;
class Agent;
class Parser;
class CommandSender;

class Client {
	friend class Player;
	friend class Coach;

	Observer        *mpObserver;
	WorldModel      *mpWorldModel;
	Agent           *mpAgent;

	Parser 		    *mpParser;
	CommandSender   *mpCommandSender;

public:
	Client();
	virtual ~Client();

	/**
	* ��̬����ʱ��Ա��ں���
	*/
	void RunDynamicDebug();

	/**
	* ��������ʱ����Ա��ں���
	*/
	void RunNormal();

	/**
	* ��������ʱ����Ա��ѭ������
	*/
	void MainLoop();

	/**
	 * ����Agent���������ص���
	 */
	void ConstructAgent();

	/**
	* ��Ա���ߺ�����ÿ����ִ��1��
	*/
	virtual void Run() = 0;

	/**
	* ��server����һЩѡ���synch_see,eye_on��
	*/
	virtual void SendOptionToServer() = 0;
};

#endif /* CLIENT_H_ */
