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

#ifndef __DynamicDebug_H__
#define __DynamicDebug_H__


#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "Observer.h"
#include "PlayerParam.h"

/**
 * ��̬�����м�¼��Ϣ������
 * Message type in dynamic debugging.
 */
enum MessageType
{
    MT_Null,
    MT_Parse,
    MT_Run,
    MT_Send
};

class DynamicDebug
{
	DynamicDebug();

	struct MessageFileHead
	{
		//char    mHeadFlag[2]; // ����ʶ���Ƿ��Ƕ�̬���Լ�¼����Ϣ�ļ�
		Time    mMaxCycle; // �ļ��м�¼���������
		long long  mIndexTableSize; // �������С
		long long  mIndexTableOffset; // ������λ��
		long long  mParserTableSize;
		long long  mParserTableOffset;
		long long  mDecisionTableSize;
		long long  mDecisionTableOffset;
		long long  mCommandSendTableSize;
		long long  mCommandSendTableOffset;
		/*int  mIndexTableSize; // �������С
		int  mIndexTableOffset; // ������λ��
		int  mParserTableSize;
		int  mParserTableOffset;
		int  mDecisionTableSize;
		int  mDecisionTableOffset;
		int  mCommandSendTableSize;
		int  mCommandSendTableOffset;*/
		MessageFileHead(): mMaxCycle( -10 )
		{

		}
	};

	struct MessageIndexTableUnit
	{
		Time    mServerTime; // ���ݶ�Ӧ������
		long long  mDataSize; // ���ݳ���
		long long  mDataOffset; // ���ݵĴ洢λ��
		long long  mTimeOffset; // ʱ���Ĵ洢λ��
		/*int  mDataSize; // ���ݳ���
		int  mDataOffset; // ���ݵĴ洢λ��
		int  mTimeOffset; // ʱ���Ĵ洢λ��*/
	};

	struct Message
	{
		MessageType mType;
		std::string mString;

		Message(MessageType type, const char *msg): mType(type), mString(msg)
		{

		}
	};

public:
	/**
     * ���캯������������
     */
    ~DynamicDebug();

    /**
     * ����ʵ������ʼ������
     */
    static DynamicDebug & instance();
    void Initial(Observer *pObserver);

    /**
     * ���溯������������Ҫ��¼server��Ϣʱ�õ��Ľӿ�
     * AddMessage()�������һ��Ҫ�ŵ������̵߳���ں�����ǰ����ã�����̬���Եõ�ϵͳʱ��ʱ�����
     * ���磬Ҫ��Parser::Parse()��Client::Decision()��CommandSend::Run()֮ǰ����
     */
    void AddMessage(const char *msg, MessageType msg_type);
    void AddTimeParser(timeval &time);
    void AddTimeDecision(timeval &time);
    void AddTimeCommandSend(timeval &time);

    /**
     * ���溯���Ƕ�̬����ʱ�õ��Ľӿ�
     */
    MessageType Run(char *msg);
    MessageType GetMessage(char *msg);
    bool FindCycle(int cycle);
    timeval GetTimeParser();
    timeval GetTimeDecision();
    timeval GetTimeCommandSend();

private:
    void Flush();

private:
    Observer    *mpObserver; // WorldModel��ָ��
    bool        mInitialOK; // �Ƿ��Ѿ���ʼ�����

    // �ļ�ͷ����Ϣ����¼4����Ϣ����
    MessageFileHead mFileHead;

    // ����4����������¼�ͱ���4����Ϣ
    std::vector<MessageIndexTableUnit>  mIndexTable;
    std::vector<timeval>                mParserTimeTable;
    std::vector<timeval>                mDecisionTimeTable;
    std::vector<timeval>                mCommandSendTimeTable;

    std::vector<Message>                mMessageTable;

    // ����4��ָ�������ڶ�д�ļ�ʱʹ��
    MessageIndexTableUnit   *mpIndex;
    timeval                 *mpParserTime;
    timeval                 *mpDecisionTime;
    timeval                 *mpCommandSendTime;

    // ��ǰ��ȡ�ĵ�Ԫ
    MessageIndexTableUnit   *mpCurrentIndex;

    // �����ļ�����
    ThreadMutex     mFileMutex;
    FILE            *mpFile;
    std::ifstream   *mpFileStream;
    std::streambuf  *mpStreamBuffer;

    bool mRunning; // �Ƿ���������
    bool mShowMessage; // �Ƿ������Ϣ
    Time mRuntoCycle; // ҪRun��������
};

#endif

