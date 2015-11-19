#include "stdafx.h"
#include "Analyse.h"

#include "LoadXML.h"

CAnalyse::CAnalyse(const CString& path)
: m_dirPath(path)
{
}


CAnalyse::~CAnalyse()
{
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
    FindXml(m_dirPath, xmlPath);

    CLoadXML xml(xmlPath);

    if (!xml.init())
    {
        TRACE("XML ERROR!");
        return false;
    }
    CComBSTR value;
    if (!xml.get_node_text(CComBSTR("EBD/EBDType"), value))
    {
        return false;
    }


    return true;
}