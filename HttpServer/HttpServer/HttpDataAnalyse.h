#pragma once
#include <list>
#include <vector>

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
    // ���� Ҫ������StringSeparates;
    bool Analyse();

    // ��ȡһ��;
    int ReadLine(char* outBufLine, int outBufMaxSize);
    // ���÷ֶ��ַ���;
    bool SetStringSeparates(const char* const pStringSeparates, int nStringSeparatesLength);
    // ��ȡһ��
    int ReadLine(const char*const bufIn, const int bufInSize, char* outBufLine, int outBufMaxSize);
    // �ƶ�ָ�뵽��һ��
    int MovePNextLine(char*&p, int size);
    // 
    const std::vector<CString>& GetAllFileName()const ;
private:
    // data
    char* m_pBuf;
    // data��С
    int m_bufSize;
    // �α�
    int m_nonius;
    char* m_pStringSeparates;
    int m_nStringSeparatesLength;

    std::list<ScmTar> m_tarList;
    std::vector<CString> m_fileNameVector;
};

