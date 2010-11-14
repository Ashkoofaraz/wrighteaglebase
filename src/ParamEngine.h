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

#ifndef PARAMENGINE_H_
#define PARAMENGINE_H_

#include <string>
#include <list>
#include <iostream>
#include <cstring>
#include "Utilities.h"

/**
 * ���������ͣ�
 * ����Ϊ�˼����ȥ���� V_BOOL ���ͣ�V_BOOL ������������ʾѡ���Ͳ����ģ�
 * �� -name ��ʾ name Ϊ true���� -goalie, -dd,
 * ����Ҫʵ��ͬ���Ĺ�����Ϊ -goalie on, -dd on
 */
enum ParamType {
	V_NONE,

	V_INT,		//����
	V_DOUBLE,	//ʵ��
	V_STRING,	//�ַ��� -- ֻ���� std::string
	V_ONOFF		//����
};

template < typename T >
struct ParamTypes {
	static const ParamType TYPE = V_NONE;
};

template <>
struct ParamTypes <int>
{
	static const ParamType TYPE = V_INT;
};

template <>
struct ParamTypes <double>
{
	static const ParamType TYPE = V_DOUBLE;
};

template <>
struct ParamTypes <std::string>
{
	static const ParamType TYPE = V_STRING;
};

template <>
struct ParamTypes <bool>
{
	static const ParamType TYPE = V_ONOFF;
};

struct Param {
	std::string name;
	void *ptr;        //pointer to data field
	ParamType type;

	Param(const char *str, void *p, ParamType t):
		name(str),
		ptr(p),
		type(t)
	{
	}
};

/**
 * ��Ҫ��������������ֵ������������������/server��Ϣ/�����ļ���;��
 */
class ParamEngine {
public:
	typedef std::list<Param>::iterator ParamPtr;

private:
	typedef std::list<Param> ParamList;

	enum {
		HASH_SIZE = 512
	};

	Array<ParamList, HASH_SIZE> mParamLists;

private:
	int hash(const char *str) {
		if (str != 0)
		{
			unsigned h = str [0];
			for (int i = 1; str [i] != 0; ++i)
			{
				h = (h << 5) - h + str [i];
			}
			return h % HASH_SIZE; // remainder
		}
		else
		{
			return -1;
		}
	}

	bool str_equal(const char *s1, const char *b) {
		return !strcmp(s1, b);
	}

public:
	ParamEngine() {}
	virtual ~ParamEngine() {}

	template <typename T>
	void AddParam(const char *name, T *address, T value){
		if (address){
			// assign default value
			*address = value;

			// allocate Param struct
			Param param(name, static_cast<void*>(address), ParamTypes<T>::TYPE);

			if (param.type == V_NONE) {
				std::cerr << __FILE__ << ":" << __LINE__ << ": param type error \"" << name << "\"" << std::endl;
				return;
			}
			// put into list
			int id = hash(name);
			if (id > -1){
				this->mParamLists[id].push_back(param);
			}
		}
	}

	template <typename T>
	bool SetParam(const char *name, T value)
	{
		if (name != 0 ){
			ParamPtr it;
			bool found = GetParam(name, it);
			if (found == true){
				switch(it->type){
					case V_INT:
					case V_DOUBLE:
					case V_STRING:
					case V_ONOFF:
						*static_cast<T*>(it->ptr) = value;
						break;
					default:
						return false;
				}
				return true;
			}
			else {
				std::cerr << __FILE__ << ":" << __LINE__ << ": unknown argument \'" << name << "\'" << std::endl;
			}
		}
		return false;
	}

	virtual void AddParams() = 0;

	bool SetParamFromString(const char *name, const char *buffer);

	bool GetParam(const char *name, ParamPtr & it);
	bool PrintParam(const char *name, std::ostream &os);
	bool PrintParam(const char *name);
	void DumpParam(std::ostream &os);
	void DumpParam();
	bool SaveToConfigFile(const char *file_name);

	/**
	 * ����������ȡ������Ϣ����ʽ��-name value ...
	 * @param argc ��������
	 * @param argv �����ַ���������׵�ַ
	 */
	void ParseFromCmdLine(int argc, char **argv);

	/**
	 * ��server��Ϣ����ȡ������Ϣ����ʽ��(name value) ...
	 * @param line server��������Ϣԭ��
	 */
	void ParseFromServerMsg(const char *line);

	/**
	 * �������ļ�����ȡ������Ϣ����ʽ��name = value
	 * @param file_name
	 * @return �Ƿ�ɹ���ȡ
	 */
	bool ParseFromConfigFile(const char *file_name);
};

#endif /* PARAMENGINE_H_ */
