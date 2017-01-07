#pragma once
#include "OneSpace.h"


class SpaceList
{
public:
	SpaceList();
	virtual ~SpaceList();

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

