#pragma once

#include "SpaceList.h"

class test_list
{
public:


	test_list();
	virtual ~test_list();

	
	bool Init(int i = 0);
private:
	CSpaceList list_;
	OneSpace o;
	int max_{ 0 };

};

