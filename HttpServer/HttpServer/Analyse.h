#pragma once
class CAnalyse
{
public:
    CAnalyse(const CString& path);
    ~CAnalyse();

    bool Run();

    bool FindXml(const CString& szDirPath, CString& xmlFile);
private:
    CString m_dirPath;
};

