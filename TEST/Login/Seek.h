#pragma once

#include "OneSpace.h"
#include "action.h"
// Ì½Ë÷

class Seek : public action
{
public:
	Seek();
	virtual ~Seek();

	bool Run();
	bool Next(std::string s);
private:
};

