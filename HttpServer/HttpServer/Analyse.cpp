#include "stdafx.h"
#include "Analyse.h"
#include "Common.h"
#include "LoadXML.h"
#include <functional>
//#include "HttpPostRequest.h"

CAnalyse::CAnalyse(const CString& path, const CString& templatePath, const CString& useFilePath)
: m_xmlPathIn(path)
, m_templatePath(templatePath)
, m_useFilePath(useFilePath)
, m_pXml(NULL)
{
    m_funcMap.insert(std::make_pair("EBMStateResponse", std::bind(&CAnalyse::EBMStateResponse, this, std::placeholders::_1)));
    m_funcMap.insert(std::make_pair("EBDResponse", std::bind(&CAnalyse::EBDResponse, this, std::placeholders::_1)));
    m_funcMap.insert(std::make_pair("ConnectionCheck", std::bind(&CAnalyse::ConnectionCheck, this, std::placeholders::_1)));
    m_funcMap.insert(std::make_pair("EBMStateRequest", std::bind(&CAnalyse::EBMStateRequest, this, std::placeholders::_1)));
    m_funcMap.insert(std::make_pair("EBM", std::bind(&CAnalyse::EBM, this, std::placeholders::_1)));
}


CAnalyse::~CAnalyse()
{
    if (NULL != m_pXml)
    {
        delete m_pXml;
        m_pXml = NULL;
    }
}
bool CAnalyse::FindXml(const CString& szDirPath, CString& xmlFile)
{
    CFileFind ff;
    BOOL ret = ff.FindFile(szDirPath + "\\*.xml");
    if (!ret)
    {
        return false;
    }

    while (ret)
    {
        ret = ff.FindNextFile();
        if (ff.IsDots())
        {
            continue;
        }
        xmlFile = ff.GetFileName();
        if (!xmlFile.IsEmpty())
        {
            xmlFile = ff.GetFilePath ();
            return true;
        }
    }
    return false;
}

bool CAnalyse::Run(CString& tarName)
{
//     CString xmlPath;
//     if (!FindXml(m_dirPath, xmlPath))
//     {
//         TRACE("FindXml ERROR!");
//         return false;
//     }

    //CLoadXML xml(xmlPath);
    m_pXml = new CLoadXML(m_xmlPathIn);
    if (NULL == m_pXml)
    {
        TRACE("new CLoadXML ERROR!");
        return false;
    }
    if (!m_pXml->init())
    {
        TRACE("XML ERROR!");
        return false;
    }
    CComBSTR value;
    if (!m_pXml->get_node_text(CComBSTR("EBD/EBDType"), value))
    {
        TRACE("get_node_text ERROR!");
        return false;
    }
    CString type(value);
    //std::function;
    auto it = m_funcMap.find(type);
    if (m_funcMap.end() != it )
    {
        if (NULL == it->second || !(it->second)(tarName))
        {
            return false;
        }
    }
    return true;
}
//#include "WinTar.h"
bool CAnalyse::EBMStateResponse(CString& tarName)
{
    // �¼��ظ��� server��������

    return false;
}

#include "HttpPostRequest.h"


bool CAnalyse::EBDResponse(CString& tarName)
{
    //���ݻ�ִ
	CLoadXML xml;
	if (!CreateEBDResponseXml(m_xmlPathIn, xml))
		return false;

	CString strKey = "EBD/EBDTime";
	CString strTime;
	SYSTEMTIME st;
	GetLocalTime(&st);
	strTime.Format(_T("%4d-%02d-%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour,
		st.wMinute, st.wSecond);
	if (!xml.set_node_text(strKey, strTime))
		return false;

	
	//
	
//     CWinTar tar;
//     tar.PackTar("EBDResponse.xml");
//    Post(CString("EBDResponse.tar"));


    return true;
}
//#include "Http.h"
// bool CAnalyse::Post(const CString& szFileName)
// {
//     CString m_strUrl = "http://127.0.0.1:8090";
//     CHttpClient m_http;
//     std::string s;
//     CFile cfile;
//     
//     if (!cfile.Open(szFileName, CFile::modeRead | CFile::typeBinary))
//     {
//         return false;
//     }
// 
//     int size = cfile.GetLength();
//     char *pBuf = new char[size];
// 
//     CString szHead;
//     char head1[] = { "--53758868654a4bcd91675cf3ee92a801\r\nContent-Disposition: form-data; name=\"file\"; filename=\"" };
//     char head2[] = { "\"\r\n\r\n" };
//     szHead = head1+szFileName+head2;
//     char end[] = { "\r\n--53758868654a4bcd91675cf3ee92a801--\r\n" };
// 
//     cfile.Read(pBuf, size);
//     int len = szHead.GetLength () + strlen(end) + size;
//     char *pPostBuffer = new char[len];
//     memcpy(pPostBuffer, szHead, szHead.GetLength());
//     memcpy(pPostBuffer + szHead.GetLength(), pBuf, size);
//     memcpy(pPostBuffer + szHead.GetLength() + size, end, strlen(end));
// 
//     //if (SUCCESS == m_http.HttpPost(m_strUrl, pBuf, size, s))
//     if (SUCCESS == m_http.HttpPost(m_strUrl, pPostBuffer, len, s))
//     {
//         //TRACE(s.c_str ());
//     }
//     delete pBuf;
//     pBuf = NULL;
//     delete pPostBuffer;
//     pPostBuffer = NULL;
//     return true;
// }

bool CAnalyse::ConnectionCheck(CString& tarName)
{
    // ���� ֱ�ӻ�ִ
    CLoadXML xml;
    CreateEBDResponseXml("EBDResponse.Xml", xml);
    return false;
}
bool CAnalyse::EBMStateRequest(CString& tarName)
{
    // �ϼ���ѯ��Щ���ظ�
	CString strTempXml;
	if (!CCommon::GetNewGUID(strTempXml))
	{
		TRACE("�����������ʧ��%d\n", strTempXml);
		return false;
	}
	CLoadXML xml;
	strTempXml = CCommon::GetProgramPath() + _T("tar\\") + strTempXml + _T(".xml");
    
	if (!CreateEBMStateResponseXml(strTempXml, xml))
        return false;

    CString strKey = "EBD/EBDTime";
    CString strTime;
    SYSTEMTIME st;
    GetLocalTime(&st);
    strTime.Format(_T("%4d-%02d-%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour,
        st.wMinute, st.wSecond);
    if (!xml.set_node_text(strKey, strTime))
        return false;

	CString strEBDID;
	strKey = "EBD/EBDID";
	if (!xml.get_node_text(strKey,strEBDID))
	{
		TRACE("Analyse.cpp EBDResponse() ��ȡ%sʧ��\n ", strKey);
		return false;
	}
	//CString strWorkPath = m_xmlPathIn.Left(m_xmlPathIn.ReverseFind(_T('\\'))+1);
	CString strWorkPath = CCommon::GetProgramPath() + _T("tar\\");
	CString strXml = strWorkPath + "EBDB_" + strEBDID + ".xml";
	CString strTar = strWorkPath + "EBDB_" + strEBDID + ".tar";
	//��EBMIDд������͵�xml��
	strKey = "EBD/EBMStateRequest/EBMID";
	if (!m_pXml->get_node_text(strKey, strEBDID))
	{
		TRACE("��ȡxml strKey=%s ʧ��\n", strKey);
		return false;
	}
	strKey = "EBD/EBMStateResponse/EBMID";
	if (!xml.set_node_text(strKey,strEBDID))
	{
		TRACE("д��xml strKey=%s ʧ��\n", strKey);
		return false;
	}
	xml.SetPath(strXml);
	xml.save();
	int Idx = strXml.Find(strWorkPath);
	if (Idx != -1)
	{
		//strXml = _T(".") + strXml.Right(strXml.GetLength() - strWorkPath.GetLength() + 1);
		strXml = strXml.Right(strXml.GetLength() - strWorkPath.GetLength());
	}
	if (CCommon::ExecTar(strWorkPath, strWorkPath + _T("tar.exe"), strXml, _T("")))
	{
		tarName = strTar;
		//����ɹ�,ɾ��ԭ����xml�ļ�
		TRACE(_T("ɾ��xml�ļ���%s\n"), strXml);
		DeleteFile(strWorkPath + strXml);
		TRACE(_T("ɾ��xml�ļ���%s\n"), m_xmlPathIn);
		DeleteFile(m_xmlPathIn);
	}
	else{
		TRACE(_T("���tarʧ�ܣ�strWorkPath=%d\ntar.exe=%s\nstrXmlTemp=%s\n"), strWorkPath,
			strWorkPath + "tar.exe", strXml);
	}
//     // ѹ��
//     CWinTar tar;
//     tar.PackTar("EBMStateResponse.xml");
//     //����
//     Post(CString("EBMStateResponse.tar"));

    return true;
}
bool CAnalyse::EBM(CString& tarName)
{
    // �ϼ���������
// 	CLoadXML xml;
// 	if (!CreateTemplateXml(_T("EBM"),xml))
// 	{
// 		TRACE("����EBMģ�壬xmlʧ��\n");
// 		return false;
// 	}
	CString strEBMID;
 	CString strKey = _T("EBD/EBM/EBMID");

	if (!m_pXml->get_node_text(strKey, strEBMID))
	{
		TRACE("��ȡxml:%s-->key=%sʧ��\n", m_xmlPathIn, strKey);
		return false;
	}
 		
	//m_pXml->release();
 	//xml����һ�ݣ�������ΪEBMID.xml���浽EBMID�ļ�·����
 	CString strEBMIDxml = CCommon::GetProgramPath() + _T("EBMID\\") + strEBMID + _T(".xml");
	if (!CopyFile(m_xmlPathIn, strEBMIDxml, FALSE))
	{
		TRACE("�����ļ���%s-->%sʧ��\n", m_xmlPathIn, strEBMIDxml);
		return false;
	}
		
 	//��xml�ļ��ƶ����û�ָ����Ŀ¼��
	CString fileName = m_xmlPathIn.Right(m_xmlPathIn.ReverseFind(_T('\\'))+1);
	CString strUsePath = m_useFilePath + fileName;
	if (!MoveFileEx(m_xmlPathIn, strUsePath, MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED))
	{
		TRACE("�����ļ���%s-->%sʧ��\n", m_xmlPathIn, strUsePath);
		return false;
	}
		
    return true;
}
bool CAnalyse::CreateTemplateXml(const CString&type, CLoadXML& xml/*, const CString& path*/)
{
	CString strEBDxml = m_templatePath + "XMLTemplate\\EBD.xml";
	xml.SetPath(strEBDxml);
    //xml.SetPath("XMLTemplate\\EBD.xml");
    if (!xml.init())
    {
        return false;
    }
	//EBDID ����
	CString strKey = _T("EBD/EBDID");
	CString strValue = _T("");
	if (xml.get_node_text(strKey, strValue))
	{
		__int64 nEBDID = _atoi64(strValue);
		++nEBDID;
		strValue.Format(_T("%lld"), nEBDID);
		if (xml.set_node_text(strKey,strValue))
		{
			if (!xml.save())
				return false;
		}
		else{
			return false;
		}
	}
	else
	{
		return false;
	}
    //xml.SetPath(path);

    //CLoadXML template95Xml("XMLTemplate\\" + type + ".xml");
	CLoadXML template95Xml(m_templatePath + "XMLTemplate\\" + type + ".xml");
    if (!template95Xml.init())
    {
        return false;
    }
    IXMLDOMNodePtr pNode;
    if (!template95Xml.get_node(CComBSTR(type), pNode))
    {
        return false;
    }

    if (!xml.add_node(CComBSTR("EBD"), pNode))
    {
        return false;
    }
    if (!xml.set_node_text(CComBSTR("EBD/EBDType"), CComBSTR(type)))
    {
        return false;
    }

//     if (!xml.save())
//     {
//         return false;
//     }

    return true;
}

bool CAnalyse::CreateEBMStateResponseXml(const CString& path, CLoadXML& xml)
{
    if (!CreateTemplateXml("EBMStateResponse", xml/*, path*/))
    {
        return false;
    }
    return true;


    return true;
}
bool CAnalyse::CreateEBDResponseXml(const CString& path, CLoadXML& xml)
{
    if (!CreateTemplateXml("EBDResponse", xml/*, path*/))
    {
        return false;
    }
    return true;
    return false;
}
bool CAnalyse::CreateConnectionCheckXml(const CString& path, CLoadXML& xml)
{
    if (!CreateTemplateXml("ConnectionCheck", xml/*, path*/))
    {
        return false;
    }
    return true;
    return false;
}
bool CAnalyse::CreateEBMStateRequestXml(const CString& path, CLoadXML& xml)
{
    if (!CreateTemplateXml("EBMStateRequest", xml/*, path*/))
    {
        return false;
    }
    return true;
    return false;
}
bool CAnalyse::CreateEBMXml(const CString& path, CLoadXML& xml)
{
    if (!CreateTemplateXml("EBM", xml/*, path*/))
    {
        return false;
    }
    return true;
    return false;
}