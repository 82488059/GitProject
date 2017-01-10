#include "stdafx.h"

#include "tinyxml.h"
#include <string>
#include "OneSpace.h"
#include "SpaceList.h"

#include "include/opencv2/opencv.hpp"
#include "opencv2/opencv.hpp"

#include "str_camp.h"
#include "test_list.h"

const std::string test_list::s_dir = "conf\\test";

test_list::test_list()
{
}


test_list::~test_list()
{
}

bool test_list::Init()
{
	if (init_)
	{
		return init_;
	}
	init_ = list_.InitWith(s_dir, "conf.xml");
	
	if (!InitUserList())
	{
		return false;
	}

	return init_;
}
bool test_list::InitUserList()
{
	userlist_.clear();
	
	std::string name = s_dir+"\\user.xml";
	TiXmlDocument doc(name.c_str());
	if (!doc.LoadFile())
	{
		return false;
	}

	TiXmlNode* node = 0;
	TiXmlElement* todoElement = 0;
	TiXmlElement* itemElement = 0;

	node = doc.RootElement();
	if (!node)
	{
		return false;
	}

	TiXmlNode* temp = 0;

	temp = node->FirstChild();
	std::string ss;
	itemElement = node->FirstChildElement("max");
	if (NULL == itemElement)
	{
		return false;
	}
	const char* value = itemElement->Attribute("value");
	if (NULL == value)
	{
		return false;
	}
	int max = atoi(value);

	value = itemElement->Attribute("input");
	if (NULL == value)
	{
		return false;
	}
	User::input = atoi(value);

	User::nameimage = cv::imread(s_dir+"\\input\\name.bmp");
	if (User::nameimage.empty())
	{
		return false;
	}
	User::pwdimage = cv::imread(s_dir+"\\input\\pwd.bmp");
	if (User::pwdimage.empty())
	{
		return false;
	}
	User::right = cv::imread(s_dir+"\\input\\right.bmp");
	if (User::right.empty())
	{
		return false;
	}

	char BUF[MAX_PATH];

	for (int i = 0; i < max; ++i)
	{
		sprintf_s(BUF, "op%d", i);
		itemElement = node->FirstChildElement(BUF);
		if (NULL == itemElement)
		{
			return false;
		}
		User user;
		value = itemElement->Attribute("name");
		if (NULL == value)
		{
			return false;
		}
		user.name = cstr_camp::str_camp(value);

		value = itemElement->Attribute("pwd");
		if (NULL == value)
		{
			return false;
		}
		user.pwd = value;
		userlist_.push_back(user);
	}
	return true;
}

bool test_list::Run()
{
	if (!init_)
	{
		return false;
	}
	bool flag = false;
	int n = 0;
	for (n = 0; n < userlist_.size(); /*++i*/)
	{
		int times = 0;
		if (RunWithUser(userlist_[n]))
		{
			n++;
			continue;
		}
		else
		{
			break;
		}
	}
	return n;
}

bool test_list::RunWithUser(const User& user)
{
	int times = 0;
	do
	{
		if (list_.RunWithUser(user))
		{
			return true;
		}
		times++;
		if (times > 30)
		{
			break;
		}
	} while (true);

	return false;
}


