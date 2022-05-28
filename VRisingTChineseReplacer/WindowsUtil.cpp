#include "WindowsUtil.h"

#include <iostream>

BOOL GetWindowsBits(BOOL& windowsIs32Bit, BOOL& isWOW64, BOOL& processIs32Bit)
{
  USHORT ProcessMachine;
  USHORT NativeMachine;

  if (!IsWow64Process2(GetCurrentProcess(), &ProcessMachine, &NativeMachine)) {
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
