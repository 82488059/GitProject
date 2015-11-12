#pragma once
#include <list>
struct ScmTar{
    char* fileName;
    int nameSize;
    char* data;
    int dataSize;
};

class CHttpDataAnalyse
{
public:
    CHttpDataAnalyse(const char*const data, int dataSize);

    CHttpDataAnalyse(int bufSize);
    ~CHttpDataAnalyse();

    char*& GetBuf(){return m_pBuf;}
    const int GetBufSize(){ return m_bufSize; }
    // 分析 要先设置StringSeparates;
    bool Analyse();

    // 读取一行;
    int ReadLine(char* outBufLine, int outBufSize);
    // 设置分段字符串;
    bool SetStringSeparates(const char* const pStringSeparates, int nStringSeparatesLength);
    //
    int ReadLine(const char*const buf, const int bufSize, char* outBufLine, int outBufSize);
    //
    int MovePNextLine(char*&p, int size);
private:
    // data
    char* m_pBuf;
    // data大小
    int m_bufSize;
    // 游标
    int m_nonius;
    char* m_pStringSeparates;
    int m_nStringSeparatesLength;

    std::list<ScmTar> m_tarList;
};

