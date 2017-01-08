#pragma once
#include "OneSpace.h"
#include <vector>
#include <memory>
#include "User.h"
#include "ImageTool.h"

class CSpaceList
{
public:
	CSpaceList();
	virtual ~CSpaceList();

	void PushSpace(OneSpacePtr& space)
	{
		spacelist_.push_back(space);
	}

	bool Check(IplImage* screen)
	{
		for (auto & it : spacelist_)
		{
			if (it->Check(screen))
			{
				return true;
			}
		}
		return false;
	}
	int detection(IplImage* screen)
	{
		for (auto &it : spacelist_)
		{
			if (it->Check(screen))
			{
				return it->step;
			}
		}
		return -1;
	}
	bool InitWith(const std::string& dir, const std::string& confname);

	bool RunWithUser(const User& user)
	{
		IplImage* screen = ImageTool::Screen();

		int next = detection(screen);
		cvReleaseImage(&screen);
		if (-1 == next)
		{
			return false;
		}

		bool sucess_name = false;
		bool sucess_pwd = false;
		
		while (true)
		{
			DWORD time = GetTickCount();

			for (int i = next; i < spacelist_.size(); /*++i*/)
			{
				screen = ImageTool::Screen();

				bool flag = spacelist_[i]->Run(screen, next);
				if (6 == next)
				{
					sucess_name = true;
				}
				else if (7 == next)
				{
					sucess_pwd = true;
				}

				i = next;
				cvReleaseImage(&screen);
				if (sucess_pwd && sucess_name && next == spacelist_.size())
				{
					return true;
				}
				else if (next == spacelist_.size())
				{
					next = 0;
					++time;
				}

			}
			if (sucess_pwd && sucess_name)
			{
				return true;
			}
		}



		return false;
	}

private:
	std::vector<OneSpacePtr> spacelist_;
};

