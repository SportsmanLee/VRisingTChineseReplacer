#include "WindowsUtil.h"

#include <iostream>

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS2) (HANDLE, USHORT*, USHORT*);

// Reference: https://docs.microsoft.com/en-us/windows/win32/api/wow64apiset/nf-wow64apiset-iswow64process
BOOL GetWindowsBits(BOOL& bIsWow64)
{
    //IsWow64Process is not available on all supported versions of Windows.
    //Use GetModuleHandle to get a handle to the DLL that contains the function
    //and GetProcAddress to get a pointer to the function if available.

    auto fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
        GetModuleHandle(TEXT("kernel32")),"IsWow64Process");
    if(!fnIsWow64Process)
    {
        std::cerr << "Function not found: IsWOW64Process(). returned FALSE (failed)." << std::endl;
        return FALSE;
    }
    
    if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
    {
        std::cerr << "IsWOW64Process returned FALSE (failed). GetLastError returned: " << GetLastError() << std::endl;
        return FALSE;
    }

    return bIsWow64;
}

// Reference: https://www.xitalogy.com/programming/2019/12/21/windows-internals-determining-bits.html
BOOL GetWindowsBits(BOOL& windowsIs32Bit, BOOL& isWOW64, BOOL& processIs32Bit)
{
    USHORT ProcessMachine;
    USHORT NativeMachine;
    
    auto fnIsWow64Process2 = (LPFN_ISWOW64PROCESS2) GetProcAddress(
        GetModuleHandle(TEXT("kernel32")),"IsWow64Process2");
    if(!fnIsWow64Process2)
    {
        std::cerr << "Function not found: IsWOW64Process2(). returned FALSE (failed)." << std::endl;
        return FALSE;
    }
    
    if (!fnIsWow64Process2(GetCurrentProcess(), &ProcessMachine, &NativeMachine)) {
        std::cerr << "IsWOW64Process2 returned FALSE (failed). GetLastError returned: " << GetLastError() << std::endl;
        return FALSE;
    }

    if (ProcessMachine == IMAGE_FILE_MACHINE_UNKNOWN) {
        isWOW64 = FALSE;

    if (NativeMachine == IMAGE_FILE_MACHINE_IA64 || NativeMachine == IMAGE_FILE_MACHINE_AMD64 || NativeMachine == IMAGE_FILE_MACHINE_ARM64) {
        windowsIs32Bit = FALSE;
        processIs32Bit = FALSE;

        return TRUE;
    }

    if (NativeMachine == IMAGE_FILE_MACHINE_I386 || NativeMachine == IMAGE_FILE_MACHINE_ARM) {
        windowsIs32Bit = TRUE;
        processIs32Bit = TRUE;

        return TRUE;
    }

    std::cerr << "Unknown Windows Architecture." << std::endl;
        return FALSE;
    }

    windowsIs32Bit = FALSE;
    isWOW64 = TRUE;
    processIs32Bit = TRUE;

    return TRUE;
}

BOOL IsWow64()
{
    BOOL bWindowsIs32Bit, bIsWow64 = FALSE, bProcessIs32Bit;

    if (!GetWindowsBits(bWindowsIs32Bit, bIsWow64, bProcessIs32Bit))
        GetWindowsBits(bIsWow64);

    return bIsWow64;
}