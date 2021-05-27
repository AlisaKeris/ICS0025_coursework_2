#include <windows.h> 
#include <future>

#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <string.h>
#include <processthreadsapi.h>

#include "ServerUtils.h"

void ServerUtils::startServer(void)
{
	std::string sh_1 = R"(start "" ICS0025PipeServer.exe)";
	std::async(std::launch::async, &system, sh_1.c_str());
	Sleep(1000);
}

DWORD FindProcessId(const std::wstring& processName);

void ServerUtils::killServer(void)
{
    auto pid = FindProcessId(L"ICS0025PipeServer.exe");

    if (pid)    // Process is running
    {
        HANDLE hProcess = OpenProcess(
            PROCESS_ALL_ACCESS, FALSE, pid);  // get Handles for every found process.
        TerminateProcess(hProcess, 0);     // Should kill the Process with exitcode 0
        CloseHandle(hProcess); 
    }
}


DWORD FindProcessId(const std::wstring& processName)
{
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    Process32First(processesSnapshot, &processInfo);
    if (!processName.compare(processInfo.szExeFile))
    {
        CloseHandle(processesSnapshot);
        return processInfo.th32ProcessID;
    }

    while (Process32Next(processesSnapshot, &processInfo))
    {
        if (!processName.compare(processInfo.szExeFile))
        {
            CloseHandle(processesSnapshot);
            return processInfo.th32ProcessID;
        }
    }

    CloseHandle(processesSnapshot);
    return 0;
}

