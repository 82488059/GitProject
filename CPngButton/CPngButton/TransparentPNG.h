/////////////////////////////////////////////////////////////////////
// 工程: 
// 作者: 
// 描述: 处理png图片，使背景变白
// 主要函数：
// 日期: 2014.11.28
// 版本: 1.0
// 修改:
/////////////////////////////////////////////////////////////////////
#pragma once
class CTransparentPNG
{

public:
    CTransparentPNG() {}
    ~CTransparentPNG() {}


    bool operator()(CImage* image) {
        if (!image)
            return false;
        if (image->GetBPP() != 32)
            return false;
        for (int i = 0; i < image->GetWidth(); ++i)
        {
            for (int j = 0; j < image->GetHeight(); ++j)
            {
                unsigned char* pucColor = reinterpret_cast<unsigned char*>(image->GetPixelAddress(i, j));
                pucColor[0] = pucColor[0] * pucColor[3] / 255;
                pucColor[1] = pucColor[1] * pucColor[3] / 255;
                pucColor[2] = pucColor[2] * pucColor[3] / 255;
            }
        }

        return true;
    }

    static bool TransPng(CImage* image)
    {
        if (!image)
            return false;
        if (image->GetBPP() != 32)
            return false;
        for (int i = 0; i < image->GetWidth(); ++i)
        {
            for (int j = 0; j < image->GetHeight(); ++j)
            {
                unsigned char* pucColor = reinterpret_cast<unsigned char*>(image->GetPixelAddress(i, j));
                pucColor[0] = pucColor[0] * pucColor[3] / 255;
                pucColor[1] = pucColor[1] * pucColor[3] / 255;
                pucColor[2] = pucColor[2] * pucColor[3] / 255;
            }
        }
        return true;
    }

};

