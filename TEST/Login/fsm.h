#pragma once


#include <vector>
#include <map>

class fsm
{
public:

	fsm();
	virtual ~fsm();

	struct State{
		int type{0};
		int index{0};
	};
	typedef std::vector<State> NextStateList;


	bool LoadConf();
	bool str2statelist(const std::string & str, NextStateList & index);
	void index2name(const int & type, const int & index, std::string & name);
	bool name2index(const std::string& name, State & index);
private:
	std::map<int, std::map<int, NextStateList>>  fsm_map_;
	State cur_state_;
};

