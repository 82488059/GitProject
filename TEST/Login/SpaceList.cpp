#include "stdafx.h"
#include "SpaceList.h"
#include "str_camp.h"
#include "tinyxml.h"

CSpaceList::CSpaceList()
{
}


CSpaceList::~CSpaceList()
{
}

bool CSpaceList::InitWith(const std::string& dir, const std::string& confname)
{
	std::string name = dir + "\\" + confname;
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
	const char* p = itemElement->Attribute("value");
	if (NULL == p)
	{
		return false;
	}
	int max = atoi(p);
	char BUF[MAX_PATH];

	for (int i = 0; i < max; ++i)
	{
		sprintf_s(BUF, "op%d", i);
		itemElement = node->FirstChildElement(BUF);
		if (NULL == itemElement)
		{
			return false;
		}
		OneSpacePtr osp = std::make_shared<OneSpace>(i);
		OneSpace & os = *osp;
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
		// os.fto = i;
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
		sprintf_s(spacedir, "%s\\%d", dir.c_str(), i);
		for (int j = 0; j < os.feat; ++j)
		{
			sprintf_s(BUF, "%s\\%d.bmp", spacedir, j);
			cv::Mat image = cvLoadImage(BUF);
			if (image.empty())
			{
				return false;
			}
			os.PushFeat(image);
		}
		sprintf_s(BUF, "%s\\check.bmp", spacedir, i);
		cv::Mat image = cvLoadImage(BUF);
        if (image.empty())
		{
			return false;
		}
		os.SetClickImage(image);
		PushSpace(osp);
	}
	return true;
}
