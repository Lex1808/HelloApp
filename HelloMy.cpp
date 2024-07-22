#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <tchar.h>


#define INFO_BUFFER_SIZE 32767
TCHAR infoBuf[INFO_BUFFER_SIZE] = { '\0' };


void printError(const TCHAR* msg) {
    TCHAR sysMsg[MAX_PATH] = { '\0' };
    TCHAR* p = sysMsg;
    DWORD eNum = ::GetLastError();

    ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, eNum,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        sysMsg, MAX_PATH, nullptr);

   
    while (*p++)
    {
        if ((*p != 9 && *p < 32) || *p == 46)
        {
            *p = 0;
            break;
        }
    }

    // Display the message
    _tprintf(TEXT("\n\t%s failed with error %d (%s)"), msg, eNum, sysMsg);

    p = nullptr;
}

int main()
{
    SYSTEM_INFO si;
    ::GetNativeSystemInfo(&si);
    DWORD bufCharCount = INFO_BUFFER_SIZE;
    LARGE_INTEGER freq;
    LARGE_INTEGER start, end;
    double elapsedTime;
    OSVERSIONINFO ver;

    std::cout << "***Hello thear!It is my program***\n";

    std::cout << "System Information\n";
    printf("Processor Architecture: %hu\n", si.wProcessorArchitecture);
    printf("Processor Type: %d\n", si.dwProcessorType);
    printf("Number processors: %d\n", si.dwNumberOfProcessors);
    printf("Page size: %d Bytes\n", si.dwPageSize);
    printf("Processor Mask: 0x%p\n", (PVOID)si.dwActiveProcessorMask);
    printf("Minimum process address: 0x%p\n", si.lpMinimumApplicationAddress);
    printf("Maximum process address: 0x%p\n", si.lpMaximumApplicationAddress);

    if (!::GetComputerName(infoBuf, &bufCharCount)) {
        printError(TEXT("GetComputerName"));
    }
    _tprintf(TEXT("\nUser name: %s"), infoBuf);

    if (!::GetWindowsDirectory(infoBuf, INFO_BUFFER_SIZE)) {
        printError(TEXT("GetWindowsDirectory"));
    }
    _tprintf(TEXT("\nWindows Directory:  %s"), infoBuf);

    //GetComputerObjectName
    bufCharCount = sizeof(infoBuf) / sizeof(TCHAR);
    if (!::GetComputerNameExW(ComputerNameDnsFullyQualified, infoBuf, &bufCharCount)) {
        printError(TEXT("GetComputerNameExW"));
    }
    _tprintf(TEXT("\nComputer name: %s"), infoBuf);

    std::cout << "\n\nMeasuring performance\n";
    QueryPerformanceFrequency(&freq);

    //Start
    QueryPerformanceCounter(&start);
    Sleep(1000);

    //End
    QueryPerformanceCounter(&end);

    elapsedTime = (end.QuadPart - start.QuadPart) / (double)freq.QuadPart;
    printf("Elapsed time: %f seconds\n", elapsedTime);

    std::cout << "\n\nProduct Info\n";
    //Get Product info
    typedef NTSTATUS(WINAPI* RtlGetVersionFunc)(LPOSVERSIONINFOEXW);

    RtlGetVersionFunc rtlGetVersion = (RtlGetVersionFunc)GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlGetVersion");
    if (!rtlGetVersion) {
        printError(TEXT("GetProcAddress for RtlGetVersionFunc"));
        return 1;
    }
    if (rtlGetVersion != nullptr) {
        OSVERSIONINFOEXW osInfo;
        ZeroMemory(&osInfo, sizeof(OSVERSIONINFOEXW));
        osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
        if (0 == rtlGetVersion(&osInfo)) {
            // Successfully retrieved OS version info
            printf("Product info: %d.%d.%d.%u\n", osInfo.dwMajorVersion,
                osInfo.dwMinorVersion,
                osInfo.dwBuildNumber,
                osInfo.dwOSVersionInfoSize);
        }
    }

    return 0;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
