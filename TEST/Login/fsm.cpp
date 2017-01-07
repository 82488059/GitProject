#include "stdafx.h"
#include "fsm.h"
#include "plug.h"

fsm::fsm()
{
}


fsm::~fsm()
{
}
bool fsm::name2index(const std::string& name, fsm::State & index)
{
	int n = name.find('_');
	if (-1 == n)
	{
		return false;
	}
	std::string num = name.substr(n + 1, name.size() - n - 1);
	index.index = atoi(num.c_str());
	for (int i = 0; i < plug::em_max; ++i)
	{
		int n = name.find(plug::s_name[i]);
		if (-1 != n)
		{
			index.type = i;
			return true;
		}
	}
	return false;
}
bool fsm::str2statelist(const std::string & str, NextStateList & index)
{
	std::string tmp = str;
	std::vector<std::string> v;
	bool flag = false;
	while (!tmp.empty())
	{
		int n = tmp.find(',');
		if (-1 == n)
		{
			v.push_back(tmp);
			break;
		}
		std::string l = tmp.substr(0, n);
		v.push_back(l);
		tmp = tmp.substr(n+1, tmp.size() - n - 1);
	}
	for (auto it : v)
	{
		State s;
		flag = name2index(it, s);
		index.push_back(s);
	}

	return flag;

}

void fsm::index2name(const int & type, const int & index, std::string & name)
{
	char BUF[MAX_PATH];
	sprintf_s(BUF, "%s_%d", plug::s_name[type], index);
	name = BUF;
}

bool fsm::LoadConf()
{
	plug::s_name;
	bool flag = false;
	fsm_map_.clear();
	for (int i = 0; i < plug::em_max; ++i)
	{
		std::map<int, NextStateList> index_map;
		int max = GetPrivateProfileIntA("CONF", plug::s_name[i], 0, plug::s_conf);
		char BUF1[MAX_PATH];
		char BUF2[MAX_PATH];
		for (int j = 0; j < max; ++j)
		{
			sprintf_s(BUF1, "%s_%d", plug::s_name[i], j);
			DWORD l = GetPrivateProfileStringA("fsm", BUF1, 0, BUF2, MAX_PATH, plug::s_conf);
			std::string tmp(BUF2);
			NextStateList sl;
			flag = str2statelist(tmp, sl);
			index_map.insert(std::make_pair(j, sl));
		}
		fsm_map_.insert(std::make_pair(i, index_map));
	}
	return flag;
}
