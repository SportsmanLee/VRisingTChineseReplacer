#include "RegistryUtil.h"
#include <vector>

HRESULT GetRegistryKey(HKEY hKey, LPCWSTR wszRegPath, HKEY& hKeyReg)
{
	LONG nRegRval;
	nRegRval = RegOpenKeyExW(hKey, wszRegPath, 0, KEY_QUERY_VALUE, &hKeyReg);

	return (nRegRval == ERROR_SUCCESS) ? S_OK : E_FAIL;
}

HRESULT GetRegistryValue(HKEY hKey, LPCWSTR wszRegPath, LPCWSTR wszKeyName, DWORD &dwRegValue)
{
	HRESULT hr;

	HKEY hKeyReg = nullptr;
	if (FAILED(hr = GetRegistryKey(hKey, wszRegPath, hKeyReg))) return hr;

	DWORD dwSize = sizeof(DWORD);
    const LONG lRetQuery = RegQueryValueExW(hKeyReg, wszKeyName, NULL, NULL, (LPBYTE)&dwRegValue, &dwSize);
	if(lRetQuery == ERROR_SUCCESS)
		return S_OK;

	return E_FAIL;
}

HRESULT GetRegistryString(HKEY hKey, LPCWSTR wszRegPath, LPCWSTR wszKeyName, std::wstring& wszRegValue)
{
	HRESULT hr;

	HKEY hKeyReg = nullptr;
	if (FAILED(hr = GetRegistryKey(hKey, wszRegPath, hKeyReg))) return hr;

	DWORD dwSize = sizeof(DWORD);
	HKEY hKeyTemp = NULL;
	auto lRetQuery = RegQueryValueExW(hKeyReg, wszKeyName, NULL, NULL, NULL, &dwSize); // Get size of string value
	if(lRetQuery == ERROR_SUCCESS)
	{
		hKeyTemp = hKeyReg;
	}

	std::vector<WCHAR> wszTempRegValue((dwSize / sizeof(WCHAR)), 0);
	lRetQuery = RegQueryValueExW(hKeyTemp, wszKeyName, NULL, NULL, (LPBYTE)wszTempRegValue.data(), &dwSize); // Get the string
	if(lRetQuery != ERROR_SUCCESS)
	{
		return E_FAIL;
	}

	wszRegValue = wszTempRegValue.data();
	return S_OK;
}