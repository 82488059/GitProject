#include "stdafx.h"

#include "tinyxml.h"
#include <string>
#include "OneSpace.h"
#include "SpaceList.h"

#include "include/opencv2/opencv.hpp"
#include "opencv2/opencv.hpp"

#include "str_camp.h"
#include "test_list.h"

test_list::test_list()
	:o(0,0)
	, list_()
{
}


test_list::~test_list()
{
}

bool test_list::Init(int i )
{
	list_;
	CString name = "Conf\\test\\conf.xml";
	TiXmlDocument doc("Conf\\test\\conf.xml");
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
	const char* p =	itemElement->Attribute("value");
	if (NULL == p)
	{
		return false;
	}
	max_ = atoi(p);
	char BUF[MAX_PATH];
	const char testdir[] = "conf\\test";

	for (int i = 0; i < max_; ++i)
	{

		sprintf_s(BUF, "op%d", i);
		itemElement = node->FirstChildElement(BUF);
		if (NULL == itemElement)
		{
			return false;
		}
		OneSpace os(0,i);
	

		p = itemElement->Attribute("feat");
		if (NULL == p)
		{
			return false;
		}
		os.feat = cstr_camp::str_camp(p);

		p = itemElement->Attribute("must");
		os.must = 0;
		if (NULL != p)
		{
			os.must = cstr_camp::str_camp(p);
		}

		p = itemElement->Attribute("fgoto");
		os. fto = i;
		if (NULL != p)
		{
			os.fto = cstr_camp::str_camp(p);
		}

		p = itemElement->Attribute("goto");
		os.to = i + 1;
		if (NULL != p)
		{
			os.to = cstr_camp::str_camp(p);
		}
		char spacedir[MAX_PATH];
		sprintf_s(spacedir, "%s\\%d", testdir, i);
		for (int j = 0; j < os.feat; ++j)
		{
			sprintf_s(BUF, "%s\\%d.bmp", spacedir, j);
			IplImage* image = cvLoadImage(BUF);
			if (NULL == image)
			{
				return false;
			}
			os.PushFeat(image);
		}
		sprintf_s(BUF, "%s\\check.bmp", spacedir, i);
		IplImage* image = cvLoadImage(BUF);
		if (NULL == image)
		{
			return false;
		}
		os.SetClickImage(image);
		list_.PushSpace(os);
	}
	return true;
}
