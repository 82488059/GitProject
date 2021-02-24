#include "StdAfx.h"
#include "HttpDataAnalyse.h"

CHttpDataAnalyse::CHttpDataAnalyse(int bufSize)
: m_pBuf(NULL)
, m_bufSize(bufSize)
, m_nonius(0)
, m_pStringSeparates(NULL)
, m_nStringSeparatesLength(0)
{
    m_pBuf = new char[bufSize];
}

CHttpDataAnalyse::CHttpDataAnalyse(const char*const data, int dataSize)
: m_pBuf(NULL)
, m_bufSize(dataSize)
, m_nonius(0)
, m_pStringSeparates(NULL)
, m_nStringSeparatesLength(0)
{
    m_pBuf = new char[dataSize];
    memcpy(m_pBuf, data, dataSize);

}


CHttpDataAnalyse::~CHttpDataAnalyse()
{
    if (NULL != m_pBuf)
    {
        delete m_pBuf;
        m_pBuf = NULL;
    }
    if (NULL != m_pStringSeparates)
    {
        delete m_pStringSeparates;
        m_pStringSeparates = NULL;
    }
}

int CHttpDataAnalyse::ReadLine(char* outBufLine, int outBufSize)
{
    int nLineLength = 0;
    ASSERT(m_nonius >= 0);
    char* pBegin = m_pBuf + m_nonius;
    char* pEnd = NULL;
    if (NULL != (pEnd = (char*)memchr(pBegin, '\n', m_bufSize - m_nonius)))
    {
        ASSERT(*(pEnd - 1) == '\r');
        nLineLength = pEnd - pBegin + 1;
        if (nLineLength >= outBufSize) 
        {
            nLineLength = outBufSize - 1;
        }
        memcpy(outBufLine, pBegin, nLineLength);
        m_nonius += nLineLength;
    }
    return nLineLength;
}

bool CHttpDataAnalyse::SetStringSeparates(const char* const pStringSeparates, int nStringSeparatesLength)
{
    if (NULL != m_pStringSeparates)
    {
        if (0 == memcmp(m_pStringSeparates, pStringSeparates, nStringSeparatesLength))
        {
            return true;
        }
        delete m_pStringSeparates;
        m_pStringSeparates = NULL;
    }

    m_pStringSeparates = new char[nStringSeparatesLength + 1];

    memcpy(m_pStringSeparates, pStringSeparates, nStringSeparatesLength + 1);
    m_nStringSeparatesLength = nStringSeparatesLength;
    return true;
}

bool CHttpDataAnalyse::Analyse()
{
    if (NULL == m_pStringSeparates)
    {
        return false;
    }
    if (0 != memcmp(m_pBuf+2, m_pStringSeparates, m_nStringSeparatesLength))
    {
        return false;
    }

    char* pNonius = m_pBuf;
    bool rval = false;
    char* pFile = NULL;
    ScmTar tmp;
    memset(&tmp, 0, sizeof(tmp));
    char* psp = NULL;

    do {
        if (0 == memcmp(pNonius+2, m_pStringSeparates, m_nStringSeparatesLength))
        {
            if (NULL != tmp.fileName)
            {
                ScmTar v = tmp;
                m_tarList.push_back(v);
            }
            ASSERT('-' == *pNonius);
            ASSERT('-' == *(pNonius+1));
            if ('-' == *(pNonius+m_nStringSeparatesLength+2))
            {
                rval = true;
                break;
            }
            char* p[6] = { 0 };
            p[1] = pNonius;
            p[2] = p[1];
            int line1 = MovePNextLine(p[2], m_bufSize - (m_pBuf - p[1]));
            p[3] = p[2];
            int line2 = MovePNextLine(p[3], m_bufSize - (m_pBuf - p[2]));
            p[4] = p[3];
            int line3 = MovePNextLine(p[4], m_bufSize - (m_pBuf - p[3]));
            p[5] = p[4];
            int line4 = MovePNextLine(p[5], m_bufSize - (m_pBuf - p[4]));
            char* pname = strstr(pNonius, "filename");
            if (NULL == pname)
            {
                break;
            }
            memset(&tmp, 0, sizeof(tmp));

            pname += 10;
            tmp.fileName = pname;
            do {
                ++tmp.nameSize;
            } while ('\"' != *++pname);
            CString szName(tmp.fileName, tmp.nameSize);
            m_fileNameVector.push_back (szName);
            tmp.data = p[5];
            pNonius = p[5];
        }
        else
        {
            ++tmp.dataSize;
        }
        
    } while (pNonius++);

    if (rval)
    {
        for (auto it = m_tarList.begin(); it != m_tarList.end();++it)
        {
            CString path = _T("recv\\");
            CString name(it->fileName, it->nameSize);
            FILE*fp = fopen(path+name, "wb");
            if (fp)
            {
                int s = fwrite(it->data, 1, it->dataSize, fp);
                fclose(fp);
            }
        }

    }

    return rval;
}

int CHttpDataAnalyse::ReadLine(const char*const buf, const int bufSize, char* outBufLine, int outBufSize)
{
    int nLineLength = 0;
    const char* const pBegin = buf;
    char* pEnd = NULL;
    if (NULL != (pEnd = (char*)memchr(pBegin, '\n', bufSize)))
    {
        ASSERT(*(pEnd - 1) == '\r');
        nLineLength = pEnd - pBegin + 1;
        if (nLineLength >= outBufSize)
        {
            nLineLength = outBufSize - 1;
        }
        memcpy(outBufLine, pBegin, nLineLength);
    }
    return nLineLength;
}
int CHttpDataAnalyse::MovePNextLine(char*&p, int size)
{
    char* pEnd = NULL;
    int nLineLength = 0;
    if (NULL != (pEnd = (char*)memchr(p, '\n', size)))
    {
        ASSERT(*(pEnd - 1) == '\r');
        nLineLength = pEnd - (p) + 1;
        p = pEnd + 1;
    }
    return nLineLength;
}

const std::vector<CString>& CHttpDataAnalyse::GetAllFileName()const
{
    return m_fileNameVector;
}
