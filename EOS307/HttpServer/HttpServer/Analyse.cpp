#include "stdafx.h"
#include "Analyse.h"

#include "LoadXML.h"
#include <functional>

CAnalyse::CAnalyse(const CString& path)
: m_dirPath(path)
, m_pXml(NULL)
{
    m_funcMap.insert(std::make_pair("EBMStateResponse", std::bind(&CAnalyse::EBMStateResponse, this)));
    m_funcMap.insert(std::make_pair("EBDResponse", std::bind(&CAnalyse::EBDResponse, this)));
    m_funcMap.insert(std::make_pair("ConnectionCheck", std::bind(&CAnalyse::ConnectionCheck, this)));
    m_funcMap.insert(std::make_pair("EBMStateRequest", std::bind(&CAnalyse::EBMStateRequest, this)));
    m_funcMap.insert(std::make_pair("EBM", std::bind(&CAnalyse::EBM, this)));
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

bool CAnalyse::Run()
{
    CString xmlPath;
    if (!FindXml(m_dirPath, xmlPath))
    {
        TRACE("FindXml ERROR!");
        return false;
    }

    //CLoadXML xml(xmlPath);
    m_pXml = new CLoadXML(xmlPath);
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
    CString type = value;
    //std::function;
    auto it = m_funcMap.find(type);
    if (m_funcMap.end() != it )
    {
        if (NULL == it->second || !(it->second)())
        {
            return false;
        }
    }
    return true;
}
#include "WinTar.h"
bool CAnalyse::EBMStateResponse()
{
    // 下级回复， server不做处理

    return false;
}
bool CAnalyse::EBDResponse()
{
    //数据回执
	CLoadXML xml;
	if (!CreateEBDResponseXml(m_dirPath, xml))
		return false;

	CString strKey = "EBD/EBDTime";
	CString strTime;
	SYSTEMTIME st;
	GetLocalTime(&st);
	strTime.Format(_T("%4d-%02d-%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour,
		st.wMinute, st.wSecond);
	if (!xml.set_node_text(strKey, strTime))
		return false;
	xml.save();

    CWinTar tar;
    tar.PackTar("EBDResponse.xml");

    Post(CString("EBDResponse.tar"));

    return true;
}
#include "Http.h"
bool CAnalyse::Post(const CString& szFileName)
{
    CString m_strUrl = "http://127.0.0.1:8090";
    CHttpClient m_http;
    std::string s;
    CFile cfile;
    
    if (!cfile.Open(szFileName, CFile::modeRead | CFile::typeBinary))
    {
        return false;
    }

    int size = cfile.GetLength();
    char *pBuf = new char[size];

    CString szHead;
    char head1[] = { "--53758868654a4bcd91675cf3ee92a801\r\nContent-Disposition: form-data; name=\"file\"; filename=\"" };
    char head2[] = { "\"\r\n\r\n" };
    szHead = head1+szFileName+head2;
    char end[] = { "\r\n--53758868654a4bcd91675cf3ee92a801--\r\n" };

    cfile.Read(pBuf, size);
    int len = szHead.GetLength () + strlen(end) + size;
    char *pPostBuffer = new char[len];
    memcpy(pPostBuffer, szHead, szHead.GetLength());
    memcpy(pPostBuffer + szHead.GetLength(), pBuf, size);
    memcpy(pPostBuffer + szHead.GetLength() + size, end, strlen(end));

    //if (SUCCESS == m_http.HttpPost(m_strUrl, pBuf, size, s))
    if (SUCCESS == m_http.HttpPost(m_strUrl, pPostBuffer, len, s))
    {
        //TRACE(s.c_str ());
    }
    delete pBuf;
    pBuf = NULL;
    delete pPostBuffer;
    pPostBuffer = NULL;
    return true;
}

bool CAnalyse::ConnectionCheck()
{
    // 心跳 直接回执
    CLoadXML xml;
    CreateEBDResponseXml("EBDResponse.Xml", xml);
    return false;
}
bool CAnalyse::EBMStateRequest()
{
    // 上级查询，些处回复
    CLoadXML xml;
    if (!CreateEBMStateResponseXml(m_dirPath, xml))
        return false;

    CString strKey = "EBD/EBDTime";
    CString strTime;
    SYSTEMTIME st;
    GetLocalTime(&st);
    strTime.Format(_T("%4d-%02d-%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour,
        st.wMinute, st.wSecond);
    if (!xml.set_node_text(strKey, strTime))
        return false;
    xml.save();

    // 压缩
    CWinTar tar;
    tar.PackTar("EBMStateResponse.xml");
    //发送
    Post(CString("EBMStateResponse.tar"));

    return true;
}
bool CAnalyse::EBM()
{
    // 上级播发请求

    return false;
}
bool CAnalyse::CreateTemplateXml(const CString&type, CLoadXML& xml, const CString& path)
{
    xml.SetPath("XMLTemplate\\EBD.xml");
    if (!xml.init())
    {
        return false;
    }
	//EBDID 自增
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
    xml.SetPath(path);

    CLoadXML template95Xml("XMLTemplate\\" + type + ".xml");
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

    if (!xml.save())
    {
        return false;
    }

    return true;
}

bool CAnalyse::CreateEBMStateResponseXml(const CString& path, CLoadXML& xml)
{
    if (!CreateTemplateXml("EBMStateResponse", xml, path))
    {
        return false;
    }
    return true;
#if 0
    xml.SetPath("XMLTemplate\\EBD.xml");
    if (!xml.init ())
    {
        return false;
    }
    xml.SetPath(path);

    CLoadXML template95Xml("XMLTemplate\\EBMStateResponse.xml");
    if (!template95Xml.init())
    {
        return false;
    }
    IXMLDOMNodePtr pNode;
    if (!template95Xml.get_node(CComBSTR("EBMStateResponse"), pNode))
    {
        return false;
    }

    if (!xml.add_node(CComBSTR("EBD"), pNode))
    {
        return false;
    }
    if (!xml.set_node_text (CComBSTR("EBD/EBDType"), "EBMStateResponse"))
    {
        return false;
    }

    if (!xml.save())
    {
        return false;
    }
#endif
    return true;
}
bool CAnalyse::CreateEBDResponseXml(const CString& path, CLoadXML& xml)
{
    if (!CreateTemplateXml("EBDResponse", xml, path))
    {
        return false;
    }
    return true;
    return false;
}
bool CAnalyse::CreateConnectionCheckXml(const CString& path, CLoadXML& xml)
{
    if (!CreateTemplateXml("ConnectionCheck", xml, path))
    {
        return false;
    }
    return true;
    return false;
}
bool CAnalyse::CreateEBMStateRequestXml(const CString& path, CLoadXML& xml)
{
    if (!CreateTemplateXml("EBMStateRequest", xml, path))
    {
        return false;
    }
    return true;
    return false;
}
bool CAnalyse::CreateEBMXml(const CString& path, CLoadXML& xml)
{
    if (!CreateTemplateXml("EBM", xml, path))
    {
        return false;
    }
    return true;
    return false;
}