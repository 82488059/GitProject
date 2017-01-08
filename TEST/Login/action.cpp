#include "stdafx.h"
#include "action.h"


const char action::s_name[][MAX_PATH] = { "run", "error", "end" };

const char action::s_conf[] = "conf\\";

action::action(const std::string & name)
	:name_(name)

{
}


action::~action()
{
}

bool action::Init()
{
	if (init_)
	{
		return init_;
	}
	actionlist_.resize(action_max);
	int n = 0;
	for (int i = 0; i < action_max; ++i)
	{
		if (InitWithName(i))
		{
			++n;
		}
	}
	if (n == action_max)
	{
		init_ = true;
	}

	return init_;
}

bool action::InitWithName(int index)
{
	std::string opername = s_name[index];
	std::string file = s_conf + name_+ "\\" + opername + "\\conf.ini";
	int max = GetPrivateProfileIntA("CONF", "max", 0, file.c_str());
	if (0 == max)
	{
		return true;
	}
	int n = 0;
	for (int i = 0; i < max; ++i)
	{
		if (InitOneSpace(index, i))
		{
			n++;
		}
	}

	return n == max;
}
bool action::InitOneSpace(int index, int step)
{
	std::string actiondir = s_conf + name_;
	char BUF[MAX_PATH];

	sprintf_s(BUF, "%s\\%s\\%d", actiondir.c_str(), s_name[index], step);
	std::string thisdir(BUF);
	std::string file = thisdir+"\\conf.ini";
	int feat = 0;
	int feat_max = GetPrivateProfileIntA("CONF", "feat", 0, file.c_str());
	OneSpacePtr osp = std::make_shared<OneSpace>(step);
	
	OneSpace& os = *osp;
	for (int i = 0; i < feat_max; ++i)
	{
		sprintf_s(BUF, "%s\\%d.bmp", thisdir.c_str(), i);
		IplImage* image1 = cvLoadImage(BUF);
		if (image1)
		{
			++feat;
			os.PushFeat(image1);
		}
	}
	int check = GetPrivateProfileIntA("CONF", "check", 0, file.c_str());
	if (check)
	{
		sprintf_s(BUF, "%s\\check.bmp", thisdir.c_str());
		IplImage* image1 = cvLoadImage(BUF);
		if (image1)
		{
			os.SetClickImage(image1);
		}
	}
	actionlist_[index].PushSpace(osp);
	return feat == feat_max;
}
