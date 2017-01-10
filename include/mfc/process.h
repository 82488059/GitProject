#pragma once
#include <Windows.h>
#include <tlhelp32.h>
namespace mfc{

    class process
    {
    public:
        process();
        ~process();

        DWORD  GetProcessidFromName(const CString& processName/*, CString & processPath*/)
        {
            PROCESSENTRY32 pe;
//             MODULEENTRY32 me;
            DWORD id = 0;
            HANDLE mehandle;
            HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            pe.dwSize = sizeof(PROCESSENTRY32);
            BOOL ret = Process32First(hSnapshot, &pe);
            if (!ret)
                return 0;
            while (1)
            {
                pe.dwSize = sizeof(PROCESSENTRY32);
                if (Process32Next(hSnapshot, &pe) == FALSE)
                    break;
                if (processName.CompareNoCase(pe.szExeFile) == 0)
                {
                    id = pe.th32ProcessID;
                    //��ȡ��ǰ��������·��
//                     mehandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, id);
//                     me.dwSize = sizeof(MODULEENTRY32);
//                     Module32First(mehandle, &me);
//                     processPath = me.szExePath;
                    break;
                }
            }
            CloseHandle(hSnapshot);
            return id;

        }

        HWND GetHwndByPid(DWORD dwProcessID)
        {
            //����Z�򶥲��Ĵ��ھ��
            HWND hWnd = ::GetTopWindow(0);
            while (hWnd)
            {
                DWORD pid = 0;
                //���ݴ��ھ����ȡ����ID
                DWORD dwTheardId = ::GetWindowThreadProcessId(hWnd, &pid);

                if (dwTheardId != 0)
                {
                    if (pid == dwProcessID)
                    {
                        return hWnd;
                    }
                }
                //����z���е�ǰһ�����һ�����ڵľ��
                hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT);

            }
            return hWnd;
        }
    private:

    };

    process::process()
    {
    }

    process::~process()
    {
    }
}
