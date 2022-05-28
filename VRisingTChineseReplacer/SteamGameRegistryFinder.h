#pragma once
#include <string>

class SteamGameRegistryFinder
{
public:
    SteamGameRegistryFinder();

    std::wstring GetSteamPath() { return m_wstrSteamPath; }
    std::wstring FindGamePath(const std::wstring& wstrName);

private:
    std::wstring FindInstallationDir(const std::wstring& wstrLibraryPath, const std::wstring& wstrGameName);

    std::wstring                m_wstrSteamPath;
    std::wstring                m_wstrLibraryFolderVDFPath;
};