#pragma once
#include "OneSpace.h"
#include <vector>

class CSpaceList
{
public:
	CSpaceList();
	virtual ~CSpaceList();

	void PushSpace(OneSpace& space)
	{
		spacelist_.push_back(space);
	}

	bool Check(IplImage* screen)
	{
		for (auto & it : spacelist_)
		{
			if (it.Check(screen))
			{
				return true;
			}
		}
		return false;
	}
	bool detection(IplImage* screen)
	{

		return false;
	}


private:
	std::vector<OneSpace> spacelist_;
};

