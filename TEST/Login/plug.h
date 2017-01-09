#pragma once

#include "opencv2/opencv.hpp"
#include <string>
#include <vector>
const double MAX_VALUE = 0.986;
class plug
{
public:
	static char s_name[][260];// { "login", "input", "use", "exit" };
	static char plug::s_conf[];
	struct NAP{
		std::string n;
		std::string p;
	};
	struct SOP{
		int type;
		int n;
		std::string  name;
		IplImage* image1;
		IplImage* image2;
	};
	enum MyEnum
	{
		em_login_sleep = 1,
		em_login = 0,
		em_input = 1,
		em_use = 2,
		em_exit = 3,
		em_max = 4,
	};
public:
	plug();
	virtual ~plug();
	static bool LoadNAP();
	static bool LoadSOP();
	static bool FindTemplateXY(IplImage*& src, IplImage*& TempPIC, CvPoint& cpt, double& maxval);
	static bool FindAndClick(IplImage* temp);
	static IplImage* Screen();
	static IplImage* StandardFormat(IplImage* imagein);
	static void LClieck(CvPoint pt = CvPoint{ 0, 0 });
	static bool GotoLogin(int type = 0, int index = 0);
	static bool GotoInput(int type = 0, int index = 0);
	
	static bool LoginUser(std::string name, std::string pwd);

	static bool FiindXX(int& index, int type);
	static bool FindLogin(int &index);
	static bool FindInput(int &index);
	static bool FindExit(int &index);
	static bool FindUse(int& index);

	static bool Detection(int& type, int & index, double & maxval);
	static bool Adjust(IplImage* screen, int& type, int & index, double & maxval);

	static bool AutoRun();

private:
	static std::vector<NAP> naplist_;
	static std::vector<std::vector<plug::SOP>> soplist_;
};

