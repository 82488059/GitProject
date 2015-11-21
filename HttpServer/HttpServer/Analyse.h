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

    // 根据类型回应
    bool EBMStateResponse();
    bool EBDResponse();
    bool ConnectionCheck();
    bool EBMStateRequest();
    bool EBM();

    // 构造基础XML
    bool CreateEBMStateResponseXml(const CString& path, CLoadXML& xml);
    bool CreateEBDResponseXml(const CString& path, CLoadXML& xml);
    bool CreateConnectionCheckXml(const CString& path, CLoadXML& xml);
    bool CreateEBMStateRequestXml(const CString& path, CLoadXML& xml);
    bool CreateEBMXml(const CString& path, CLoadXML& xml);

    bool CreateTemplateXml(const CString&type, CLoadXML& xml, const CString& path);

    bool Post(const CString& szFileName);
private:
    CString m_dirPath;
    CLoadXML* m_pXml;
    std::map<CString, std::function<bool()>> m_funcMap;
};

