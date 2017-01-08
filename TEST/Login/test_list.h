#pragma once

#include "SpaceList.h"
#include "User.h"

class test_list
{
public:


	test_list();
	virtual ~test_list();

	
	bool Init();

	bool Run();
	bool RunWithUser(const User& user);
private:
	CSpaceList list_;
	int max_{ 0 };
	bool init_{ false };

};

