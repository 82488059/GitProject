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

	bool Check(cv::Mat screen)
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
	int detection(cv::Mat screen)
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
		cv::Mat screen = ImageTool::Screen();
		int next = detection(screen);
		if (-1 == next)
		{
			return false;
		}

		bool input_sucess = false;
		
		while (true)
		{
			DWORD time = GetTickCount();

			for (int i = next; i < spacelist_.size(); /*++i*/)
			{
				screen = ImageTool::CaptureScreen();

				bool flag = spacelist_[i]->Run(screen, next);
				if (User::input == next)
				{
					input_sucess = user.Input();
					//input_sucess = true;
				}

				i = next;

				if ( input_sucess && next == spacelist_.size())
				{
					return true;
				}
				else if (next == spacelist_.size())
				{
					next = 0;
					++time;
				}
				Sleep(200);
			}
			if (input_sucess)
			{
				return true;
			}
		}



		return false;
	}

private:
	std::vector<OneSpacePtr> spacelist_;
};

