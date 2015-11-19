#pragma once
class CWinTar
{
public:
    CWinTar();
    ~CWinTar();

    bool UnPackTar(const CString& szFileName, const CString& path);
    bool PackTar(const CString & szFileName);
};

