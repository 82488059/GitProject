// serverthread.cpp

#include "stdafx.h"
#include "blocksock.h"
#include "WinTar.h"
#include "LoadXML.h"
#include "Analyse.h"
#define SERVERMAXBUF 5000
#define MAXLINELENGTH 100

volatile int g_nConnection=0;//连接的个数
volatile BOOL g_bListening=FALSE;//侦听套接字状态
volatile UINT g_nPortServer=8089;//服务端口
CString g_strDirect="c:\\WebSite\\";//服务路径
CString g_strIPServer = "192.168.1.177";//服务器地址
CString g_strDefault="Default.htm";//缺省网页的名字
CMyBlockSocket g_sListen;//侦听套接字

//解析请求
BOOL Parse(char* pStr, char** ppToken1, char** ppToken2)
{
	*ppToken1=pStr;
	char* pch=strchr(pStr, ' ');
	if(pch) 
	{
		*pch='\0';
		pch++;
		*ppToken2=pch;
		pch=strchr(pch, ' ');
		if(pch) {
			*pch='\0';
			return TRUE;
		}
	}
	return FALSE;
}

//打开文件
void LogRequest(LPVOID pParam, char* pch, CSocketAddress sa)
{
	//pParam参数保存了CListBox对象的指针
	CString strList;
	CListBox* pList=(CListBox*)pParam;
	CString strGmt=CTime::GetCurrentTime().FormatGmt("%m/%d/%y %H:%M:%S GMT");
	strList.Format("服务器连接 # %d ",g_nConnection);
	pList->AddString(strList);
	strList.Format("     IP地址：%s 端口：%d",sa.DottedDecimal(), sa.Port());
	pList->AddString(strList);
	strList.Format("     时间：%s",strGmt);
	pList->AddString(strList);
	strList.Format("     请求：%s",pch);
	pList->AddString(strList);
}

CFile* OpenFile(const char* pName)
{
	//打开文件
	//注意，这里加上了服务器路径
	CFileException e;
	CFile* pFile=new CFile();
	if(*pName=='/') pName++;
	CString strName=pName;
	strName.Replace('/','\\');
	if(pFile->Open(g_strDirect+strName, CFile::modeRead, &e)) {
		return pFile;
	}
	if((e.m_cause==CFileException::accessDenied) ||
			(e.m_cause==CFileException::badPath)) 
	{
		//错误处理
		int nLength;
		// add a \ unless it's the "root" directory
		if((nLength=strName.GetLength()) > 1) 
		{
			if(strName[nLength-1]!='\\') 
			{
				strName+='\\';
			}
		}
		//指定缺省的文件名
		strName+=g_strDefault;
		if(pFile->Open(g_strDirect+strName, CFile::modeRead, &e)) 
			return pFile;
	}
	delete pFile;
	return NULL;
}

//记录错误信息
void LogBlockingSocketException(LPVOID pParam, char* pch, CMyBlockSocketException* pe)
{
	//pParam参数保存了CListBox对象的指针
	CListBox* pList=(CListBox*)pParam;
	CString strGmt = CTime::GetCurrentTime().FormatGmt("%m/%d/%y %H:%M:%S GMT");
	char text2[50];
	pe->GetErrorMessage(text2, 49);
	CString strList;
	pList->AddString(strList);
	strList.Format("WINSOCK错误");
	pList->AddString(strList);
	strList.Format("     %s",pch);
	pList->AddString(strList);
	strList.Format("     错误信息：%s",text2);
	pList->AddString(strList);
	strList.Format("     时间：%s",strGmt);
	pList->AddString(strList);
}
#include "HttpDataAnalyse.h"
UINT ServerThreadProc(LPVOID pParam)
{
	CSocketAddress saClient;
	CMyHttpBlockSocket sConnect;
	CListBox* pList=(CListBox*)pParam;
	//缓存区
	char* buffer=new char[SERVERMAXBUF];
	char headers[500], 
		 request1[MAXLINELENGTH],  
		 request2[MAXLINELENGTH];
	//连接错误返回到浏览的信息
	char hdrErr[]=
		"HTTP/1.0 404 对象没有找到\r\n"
		"Server: MySocket Server\r\n"
		"Content-Type: text/html\r\n"
		"Accept-Ranges: bytes\r\n"
		"Content-Length: 66\r\n\r\n"
		"<html><h1><body>HTTP/1.0 404 对象没有找到</h1></body></html>\r\n\r\n";
	//连接正确时返回的信息
    char hdrFmt[] =
        "HTTP/1.0 200 OK\r\n"
        "Server: MySocket Server\r\n"
        //"Date: %s\r\n"
        "Content-Type: text/html\r\n"
        "Accept-Ranges: bytes\r\n"
        "Content-Length: %d\r\n\r\n";
		//"Content-Length: %d\r\n\r\n";
	//默认的页面--default HTML page
	char CustomHtml[]=
		"<html>\r\n"
		"<head>\r\n"
		"<title></title>\r\n"
		"</head>\r\n"
		"<body>\r\n"
		"<p align=\"center\">欢迎访问我的主页</p>\r\n"
		"<h3 align=\"center\"><a href=\"Default.htm\">快乐天地</a></h3>\r\n"
		"<p>结束</p>\r\n"
		"<p>　</p>\r\n"
		"</body></html>\r\n\r\n";
	
	CString strGmtNow=
		CTime::GetCurrentTime().FormatGmt("%a, %d %b %Y %H:%M:%S GMT");
	int nBytesSent=0;
	CFile* pFile=NULL;
	char* pToken1;
	char* pToken2;
    char* pData = NULL;
    char StringSeparates[64] = { 0 };
    int nStringSeparates = 0;
	try 
	{
		//开始侦听连接请求
		if(!g_sListen.Accept(sConnect, saClient)) 
		{
			//在应用程序关闭时的处理
			g_bListening=FALSE;
			delete [] buffer;
			return 0;
		}
		//连接数增加一个
		g_nConnection++;
		//开始另一个服务器线程
		AfxBeginThread(ServerThreadProc, pParam, THREAD_PRIORITY_NORMAL);
		//从客户端(浏览器)读取请求
		sConnect.ReadHttpHeaderLine(request1, MAXLINELENGTH, 10);
		//记录请求
		LogRequest(pParam, request1, saClient);
		//解析请求并作相应的处理
		if(Parse(request1, &pToken1, &pToken2))
		{
			//浏览器GET方式
			if(!stricmp(pToken1, "GET")) 
			{
				do 
				{	
					//读取请求的剩余部分
					sConnect.ReadHttpHeaderLine(request2, MAXLINELENGTH, 10);
				}
				while(strcmp(request2, "\r\n"));
				if(!stricmp(pToken2, "/custom"))// || !stricmp(pToken2, "/"))
				{
					//发送默认的页面--default HTML page
					wsprintf(headers, hdrFmt, (const char*) strGmtNow, strlen(CustomHtml));
					strcat(headers, "\r\n");
					sConnect.Write(headers, strlen(headers), 10);
					sConnect.Write(CustomHtml, strlen(CustomHtml), 10);
				}
				else if(strchr(pToken2, '?')) 
				{
					//CGI请求
					//该HTTP服务器还不能对CGI请求作出响应
					//也不能调用相应的ISAPI的DLL(动态连接库)
				}
				else
				{
					//文件处理
					//注意对服务器目录的设定
					//获得文件在服务器上的路径
					if((pFile=OpenFile(pToken2))!=NULL) 
					{
						//文件打开
						CFileStatus fileStatus;
						pFile->GetStatus(fileStatus);
						CString strGmtMod=fileStatus.m_mtime.FormatGmt("%a, %d %b %Y %H:%M:%S GMT");
						char hdrModified[50];
						wsprintf(hdrModified, "Last-Modified: %s\r\n\r\n", (const char*) strGmtMod);
						DWORD dwLength=pFile->GetLength();
						wsprintf(headers, hdrFmt,  (const char*) strGmtNow, dwLength);
						strcat(headers, hdrModified);
						nBytesSent=sConnect.Write(headers, strlen(headers), 10);
						//传送的文件应该是在某一个时间之后被修改过的
						//因此文件的时间应该是小于该设定时间
						nBytesSent=0;
						DWORD dwBytesRead=0;
						UINT uBytesToRead;
						//将文件以5k的大小为单位发送，避免内存分配错误
						while(dwBytesRead < dwLength) 
						{
							uBytesToRead=min(SERVERMAXBUF, dwLength-dwBytesRead);
							VERIFY(pFile->Read(buffer, uBytesToRead)==uBytesToRead);
							nBytesSent+=sConnect.Write(buffer, uBytesToRead, 10);
							dwBytesRead+=uBytesToRead;
						}
                        pFile->Close();
					}
					else
					{
						//向浏览器发送“出错信息”
						nBytesSent=sConnect.Write(hdrErr, strlen(hdrErr), 10);
					}
				}
			}
			else if(!stricmp(pToken1, "POST"))
			{
                int nDataSize = 0;
				//浏览器POST方式
				do 
				{
					//读取请求的剩余部分
					sConnect.ReadHttpHeaderLine(request2, MAXLINELENGTH, 10);
                    char *p = strstr(request2, "Content-Length: ");
                    if (NULL != p)
                    {
                        char *Size = p + 16;
                        nDataSize = atoi(Size);
                    }
                    char *psp = strstr(request2, "boundary");
                    if (NULL != psp)
                    {
                        psp+=9;
                        for (int i = 0; '\r' != *psp && i < 64; ++i, ++psp)
                        {
                            StringSeparates[i] = *psp;
                            nStringSeparates++;
                        }

                        
                    }
				}
				while(strcmp(request2, "\r\n"));

//                 sConnect.ReadHttpHeaderLine(request2, MAXLINELENGTH, 10);
//                 sConnect.ReadHttpHeaderLine(request2, MAXLINELENGTH, 10);
//                 sConnect.ReadHttpHeaderLine(request2, MAXLINELENGTH, 10);
                pData = new char[nDataSize];
                sConnect.ReadHttpResponse(pData, nDataSize, 10);

				LogRequest(pParam, request2, saClient);
				//向浏览器发送“出错信息”
                //nBytesSent=sConnect.Write(hdrErr, strlen(hdrErr), 10);
                //nBytesSent=sConnect.Write(hdrFmt, strlen(hdrFmt), 10);

                {
                    //FILE *fp = fopen("", "rb");
                    CFile cfile;
                    if (cfile.Open("XML\\EBDB_100000000001.tar", CFile::modeRead | CFile::typeBinary))
                    {
                        int size = cfile.GetLength();

                        CString str;
                        str.Format(hdrFmt, size);
                        nBytesSent = sConnect.Write(str, str.GetLength (), 10);

                        char *pBuf = new char[size];
                        cfile.Read(pBuf, size);
                        nBytesSent = sConnect.Write(pBuf, size, 500);
                        delete pBuf;
                    }
                    else
                    {
                        nBytesSent=sConnect.Write(hdrErr, strlen(hdrErr), 10);
                    }
                }

                CHttpDataAnalyse ana(pData, nDataSize);
                {
                    FILE* fp = fopen(CString(_T("recv\\"))+StringSeparates, "wb");
                    if (fp)
                    {
                        fwrite(pData, nDataSize, 1, fp);
                        fclose(fp);
                    }
                }
                ana.SetStringSeparates(StringSeparates, nStringSeparates);
                if (ana.Analyse())
                {
                    TRACE("ok");
                }

                CString szTarName;
                auto v = ana.GetAllFileName();
                for (int i = 0; i < v.size(); ++i)
                {
                    szTarName = v[i];
                    szTarName.MakeUpper();
                    if (-1 != szTarName.Find(_T(".TAR")))
                    {
                        break;
                    }
                }
                CWinTar tar;
                tar.UnPackTar("recv\\"+szTarName, "recv");
                szTarName = szTarName.Left(szTarName.GetLength() - 4);

//                CAnalyse analyse("recv\\" + szTarName);
// 
//                 analyse.Run();
			}
			else 
			{
				//其它的请求方式
			}
		}
		else 
		{
			//错误的请求
		}
		//关闭套接字
		sConnect.Close();

        // 
	}
	catch(CMyBlockSocketException* pe)
	{
		//错误处理
		LogBlockingSocketException(pParam, "服务器:", pe);
		pe->Delete();
	}

	delete [] buffer;
	if(pFile) delete pFile;
    if (NULL != pData) 
    {
        delete pData;
        pData = NULL;
    }
    return 0;
}


