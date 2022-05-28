#pragma once
#include "Windows.h"
#include <string>

HRESULT GetRegistryValue(HKEY hKey, LPCWSTR wszRegPath, LPCWSTR wszKeyName, DWORD &dwRegValue);
HRESULT GetRegistryString(HKEY hKey, LPCWSTR wszRegPath, LPCWSTR wszKeyName, std::wstring& wszRegValue);