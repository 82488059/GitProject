#pragma once

#include "SpaceList.h"
#include "User.h"
#include <vector>

class test_list
{
public:


	test_list();
	virtual ~test_list();

	
	bool Init();
	bool InitUserList();

	bool Run();
	bool RunWithUser(const User& user);
private:
	CSpaceList list_;
	std::vector<User> userlist_;
	int max_{ 0 };
	bool init_{ false };
	static const std::string s_dir;
};

