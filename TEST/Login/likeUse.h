#pragma once
#include "opencv2/opencv.hpp"
#include <string>
#include <list>


class likeUse
{
public:
    likeUse();
    virtual ~likeUse();

	struct NAP{
		std::string n;
		std::string p;
	};
	struct SOP{
		int n;
		std::string  opstr;
		IplImage* image;
	};

public:
    static bool FindTemplateXY(IplImage*& src, IplImage*& templ, CvPoint& pt, double& maxval);

    static IplImage* Screen();

    static IplImage* StandardFormat(IplImage* imagein);

	static bool FindAndClick(IplImage* imagein);
	static void LClick();


	static bool LoadConf(int& max, int & u, int &p);

	static bool LoadNAP(std::list<NAP>& naplist);

	static bool LoadSOP(std::list<SOP>& soplist);
};

