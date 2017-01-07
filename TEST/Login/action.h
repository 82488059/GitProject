#pragma once
#include <string>
#include "SpaceList.h"

class action
{
public:
	enum em_action
	{
		action_run,
		action_error,
		action_end,
		action_max,
	};
	static const char s_name[][MAX_PATH];// = { "run", "error", "end" };
	static const char s_conf[];
	action(const std::string & name);
	virtual ~action();

	virtual bool Init();

protected:

	
	bool InitWithName(int index);
	bool InitOneSpace(int index, int step);

	std::string name_;
	bool init_;
	std::vector<SpaceList> actionlist_;

};

