#pragma once
#include <vector>
#include "OneSpace.h"

class Conf
{
public:
	Conf();
	virtual ~Conf();



	std::map<std::string, std::map<int , OneSpace>> SpaceList_;


};

