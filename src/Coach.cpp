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

#include <list>
#include <string>
#include <sstream>
#include "Coach.h"
#include "DynamicDebug.h"
#include "Formation.h"
#include "CommandSender.h"
#include "Parser.h"
#include "Thread.h"
#include "UDPSocket.h"
#include "WorldModel.h"
#include "Agent.h"
#include "Logger.h"
#include "TimeTest.h"
#include "PlayerParam.h"
#include <iostream>
using namespace std;

Coach::Coach()
{
}

Coach::~Coach()
{

}

void Coach::SendOptionToServer()
{
	while (!mpParser->IsEyeOnOk())
	{
		UDPSocket::instance().Send("(eye on)");
		WaitFor(200);
	}

	for (int i = 1; i <= TEAMSIZE; ++i)
    {
        if (i != PlayerParam::instance().ourGoalieUnum() && mpObserver->Teammate_Coach(i).IsAlive())
        {
            mpAgent->CheckCommands(mpObserver);
            mpAgent->ChangePlayerType(i, i);
            mpObserver->SetCommandSend();
            WaitFor(5);
        }
	}

    /** check whether each player's type is changed OK */
	WaitFor(200);
	for (int i = 1; i <= TEAMSIZE; ++i)
    {
        if (i != PlayerParam::instance().ourGoalieUnum() && mpObserver->Teammate_Coach(i).IsAlive())
        {
            while (!mpParser->IsChangePlayerTypeOk(i) && mpObserver->CurrentTime().T() < 1)
            {
			    mpAgent->CheckCommands(mpObserver);
			    mpAgent->ChangePlayerType(i, i);
			    mpObserver->SetCommandSend();
                WaitFor(200);
		    }
        }
	}
}

/*
 * The main loop for coach.
 */
void Coach::Run()
{
	mpObserver->Lock();

	/** ���漸������˳���ܱ� */
	Formation::instance.SetTeammateFormations();
	mpAgent->CheckCommands(mpObserver); // ��������ڷ����������
	mpWorldModel->Update(mpObserver);

	mpObserver->UnLock();

	//do planning...

	Logger::instance().LogSight();
}
