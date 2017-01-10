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
                    //获取当前进程所在路径
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
            //返回Z序顶部的窗口句柄
            HWND hWnd = ::GetTopWindow(0);
            while (hWnd)
            {
                DWORD pid = 0;
                //根据窗口句柄获取进程ID
                DWORD dwTheardId = ::GetWindowThreadProcessId(hWnd, &pid);

                if (dwTheardId != 0)
                {
                    if (pid == dwProcessID)
                    {
                        return hWnd;
                    }
                }
                //返回z序中的前一个或后一个窗口的句柄
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
