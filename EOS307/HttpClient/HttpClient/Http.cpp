////////////////////////////////// HttpClient.cpp
#include "StdAfx.h"
#include "Http.h"
//#include "yazuoLog.h"

#define  BUFFER_SIZE       1024

#define  NORMAL_CONNECT             INTERNET_FLAG_KEEP_CONNECTION
#define  SECURE_CONNECT                NORMAL_CONNECT | INTERNET_FLAG_SECURE
#define  NORMAL_REQUEST             INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE 
#define  SECURE_REQUEST             NORMAL_REQUEST | INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID

CHttpClient::CHttpClient(LPCTSTR strAgent)
{
    m_pSession = new CInternetSession(strAgent);
    m_pConnection = NULL;
    m_pFile = NULL;
}


CHttpClient::~CHttpClient(void)
{
    Clear();
    if (NULL != m_pSession)
    {
        m_pSession->Close();
        delete m_pSession;
        m_pSession = NULL;
    }
}

void CHttpClient::Clear()
{
    if (NULL != m_pFile)
    {
        m_pFile->Close();
        delete m_pFile;
        m_pFile = NULL;
    }

    if (NULL != m_pConnection)
    {
        m_pConnection->Close();
        delete m_pConnection;
        m_pConnection = NULL;
    }
}

int CHttpClient::ExecuteRequest(LPCTSTR strMethod, LPCTSTR strUrl, LPCTSTR strPostData, UINT postDataSize, string &strResponse)
{
    CString strServer;
    CString strObject;
    DWORD dwServiceType;
    INTERNET_PORT nPort;
    strResponse = "";

    AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort);

    if (AFX_INET_SERVICE_HTTP != dwServiceType && AFX_INET_SERVICE_HTTPS != dwServiceType)
    {
        return FAILURE;
    }

    try
    {
        m_pConnection = m_pSession->GetHttpConnection(strServer,
            dwServiceType == AFX_INET_SERVICE_HTTP ? NORMAL_CONNECT : SECURE_CONNECT,
            nPort);
        if (NULL == m_pConnection)
        {
            return FAILURE;
        }
        
        m_pFile = m_pConnection->OpenRequest(strMethod, strObject,
            NULL, 1, NULL, NULL,
            (dwServiceType == AFX_INET_SERVICE_HTTP ? NORMAL_REQUEST : SECURE_REQUEST));
        
        //DWORD dwFlags;
        //m_pFile->QueryOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);
        //dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
        ////set web server option
        //m_pFile->SetOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);

        m_pFile->AddRequestHeaders("Accept: */*");
        //m_pFile->AddRequestHeaders("Accept-Language: zh-cn");
        m_pFile->AddRequestHeaders("Content-Type: multipart/form-data; boundary=53758868654a4bcd91675cf3ee92a801");
        m_pFile->AddRequestHeaders("Accept-Encoding: gzip, deflate");
        m_pFile->AddRequestHeaders("Connection: keep-alive");

//         m_pFile->AddRequestHeaders("--7953519a4b6c412c9d50acef63fb22bd\r\n\
// Content-Disposition: form-data; name=\"file\"; filename=\"EBDT1.tar\"");

        //"Content-Disposition: form-data; name="file"; filename="EBDT1.tar""
        m_pFile->SendRequest(NULL, 0, (LPVOID)strPostData, strPostData == NULL ? 0 : postDataSize);
        //char end[] = { "--53758868654a4bcd91675cf3ee92a801--\r\n" };
        //m_pFile->SendRequest(NULL, 0, (LPVOID)(LPCTSTR)end, strlen (end));

        char szChars[BUFFER_SIZE + 1] = { 0 };
        string strRawResponse = _T("");
        UINT nReaded = 0;

        FILE* fp = fopen("recv\\xx.tar", "wb");

        while ((nReaded = m_pFile->Read((void*)szChars, BUFFER_SIZE)) > 0)
        {
            if (fp)
            {
                fwrite(szChars, nReaded, 1, fp);
            }
            szChars[nReaded] = '\0';
            strRawResponse += szChars;
            memset(szChars, 0, BUFFER_SIZE + 1);
        }
        if (fp)
        {
            fclose(fp);
        }

        int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, strRawResponse.c_str(), -1, NULL, 0);
        WCHAR *pUnicode = new WCHAR[unicodeLen + 1];
        memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));

        MultiByteToWideChar(CP_UTF8, 0, strRawResponse.c_str(), -1, pUnicode, unicodeLen);
        CString cs(pUnicode);
        delete[]pUnicode;
        pUnicode = NULL;
        
        strResponse = cs;
        //cs.ReleaseBuffer();
        //m_pFile->SendRequest(NULL, 0, (LPVOID)strPostData, strPostData == NULL ? 0 : postDataSize);
        Clear();
    }
    catch (CInternetException* e)
    {
        Clear();
        DWORD dwErrorCode = e->m_dwError;
        e->Delete();

        DWORD dwError = GetLastError();

        //PRINT_LOG("dwError = %d", dwError, 0);

        if (ERROR_INTERNET_TIMEOUT == dwErrorCode)
        {
            return OUTTIME;
        }
        else
        {
            return FAILURE;
        }
    }
    return SUCCESS;
}

int CHttpClient::HttpGet(LPCTSTR strUrl, LPCTSTR strPostData, UINT getDataSize, string &strResponse)
{
    return ExecuteRequest(_T("GET"), strUrl, strPostData, getDataSize, strResponse);
}

int CHttpClient::HttpPost(LPCTSTR strUrl, LPCTSTR strPostData, UINT postDataSize, string &strResponse)
{
    return ExecuteRequest(_T("POST"), strUrl, strPostData, postDataSize, strResponse);
}
