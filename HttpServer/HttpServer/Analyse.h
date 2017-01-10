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
	
    // �������ͻ�Ӧ
    bool EBMStateResponse(CString& tarName);
    bool EBDResponse(CString& tarName);
    bool ConnectionCheck(CString& tarName);
    bool EBMStateRequest(CString& tarName);
    bool EBM(CString& tarName);

    // �������XML
    bool CreateEBMStateResponseXml(const CString& path, CLoadXML& xml);
    bool CreateEBDResponseXml(const CString& path, CLoadXML& xml);
    bool CreateConnectionCheckXml(const CString& path, CLoadXML& xml);
    bool CreateEBMStateRequestXml(const CString& path, CLoadXML& xml);
    bool CreateEBMXml(const CString& path, CLoadXML& xml);

    bool CreateTemplateXml(const CString&type, CLoadXML& xml/*, const CString& path*/);

//    bool Post(const CString& szFileName);
private:
    CString m_xmlPathIn; // �յ���tar������ѹ������xml·��
    CString m_xmlPath; // xmlPath
	CString m_useFilePath; //�û��������ڱ����ļ�·��
    CLoadXML* m_pXml;
	CString m_templatePath; //ģ���ļ����·������CGI����·��
    std::map<CString, std::function<bool(CString&)>> m_funcMap;
};

