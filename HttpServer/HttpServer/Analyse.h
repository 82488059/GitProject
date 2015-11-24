#pragma once
#include <map>
#include <functional>

class CLoadXML;
class CAnalyse
{
public:
    CAnalyse(const CString& path, const CString& templatePath, const CString& useFilePath);
    ~CAnalyse();

    bool Run(CString&tarName);

    bool FindXml(const CString& szDirPath, CString& xmlFile);
	
    // 根据类型回应
    bool EBMStateResponse(CString& tarName);
    bool EBDResponse(CString& tarName);
    bool ConnectionCheck(CString& tarName);
    bool EBMStateRequest(CString& tarName);
    bool EBM(CString& tarName);

    // 构造基础XML
    bool CreateEBMStateResponseXml(const CString& path, CLoadXML& xml);
    bool CreateEBDResponseXml(const CString& path, CLoadXML& xml);
    bool CreateConnectionCheckXml(const CString& path, CLoadXML& xml);
    bool CreateEBMStateRequestXml(const CString& path, CLoadXML& xml);
    bool CreateEBMXml(const CString& path, CLoadXML& xml);

    bool CreateTemplateXml(const CString&type, CLoadXML& xml/*, const CString& path*/);

//    bool Post(const CString& szFileName);
private:
    CString m_xmlPathIn; // 收到的tar包，解压出来的xml路径
    CString m_xmlPath; // xmlPath
	CString m_useFilePath; //用户设置用于保存文件路径
    CLoadXML* m_pXml;
	CString m_templatePath; //模板文件存放路径，即CGI程序路径
    std::map<CString, std::function<bool(CString&)>> m_funcMap;
};

