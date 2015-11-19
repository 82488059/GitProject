#include "stdafx.h"
#include "Analyse.h"

#include "LoadXML.h"
#include <functional>

CAnalyse::CAnalyse(const CString& path)
: m_dirPath(path)
, m_pXml(NULL)
{
    //std::function<bool ()> func;
    //func = std::bind(&CAnalyse::Run, this);
    m_funcMap.insert(std::make_pair("EBMStateResponse", std::bind(&CAnalyse::EBMStateResponse, this)));
    m_funcMap.insert(std::make_pair("EBMStateResponse", std::bind(&CAnalyse::EBMStateResponse, this)));
    m_funcMap.insert(std::make_pair("EBMStateResponse", std::bind(&CAnalyse::EBMStateResponse, this)));
    m_funcMap.insert(std::make_pair("EBMStateResponse", std::bind(&CAnalyse::EBMStateResponse, this)));
    m_funcMap.insert(std::make_pair("EBMStateResponse", std::bind(&CAnalyse::EBMStateResponse, this)));
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
    if (m_pXml)
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
        if (it->second && (it->second)())
        {

        }
    }
    return true;
}
bool CAnalyse::EBMStateResponse()
{

    return false;
}
