#pragma once
#include <map>
#include <functional>

class CLoadXML;
class CAnalyse
{
public:
    CAnalyse(const CString& path);
    ~CAnalyse();

    bool Run();

    bool FindXml(const CString& szDirPath, CString& xmlFile);


    bool EBMStateResponse();
private:
    CString m_dirPath;
    CLoadXML* m_pXml;
    std::map<CString, std::function<bool()>> m_funcMap;
};

