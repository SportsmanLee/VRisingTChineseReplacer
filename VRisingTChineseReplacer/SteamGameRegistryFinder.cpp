#include "SteamGameRegistryFinder.h"
#include "RegistryUtil.h"
#include "WindowsUtil.h"

#include <iostream>
#include <fstream>
#include <regex>
#include <filesystem>

std::size_t replace_all(std::wstring& inout, std::wstring_view what, std::wstring_view with)
{
    std::size_t count{};
    for (std::string::size_type pos{};
         inout.npos != (pos = inout.find(what.data(), pos, what.length()));
         pos += with.length(), ++count) {
        inout.replace(pos, what.length(), with.data(), with.length());
    }
    return count;
}

SteamGameRegistryFinder::SteamGameRegistryFinder()
{
    std::wcout << L"Steam installed path: ";

    BOOL bWindowsIs32Bit, bIsWOW64, bProcessIs32Bit;
    if (!GetWindowsBits(bWindowsIs32Bit, bIsWOW64, bProcessIs32Bit)) return;

    if (bWindowsIs32Bit && SUCCEEDED(GetRegistryString(HKEY_LOCAL_MACHINE, LR"(SOFTWARE\Valve\Steam)", L"InstallPath", m_wstrSteamPath)) ||
        !bWindowsIs32Bit && SUCCEEDED(GetRegistryString(HKEY_LOCAL_MACHINE, LR"(SOFTWARE\Wow6432Node\Valve\Steam)", L"InstallPath", m_wstrSteamPath)))
    {
        std::wcout << m_wstrSteamPath << std::endl;
        m_wstrLibraryFolderVDFPath = m_wstrSteamPath + LR"(\steamapps\libraryfolders.vdf)";
    }
    else
    {
        std::wcout << L"Not found!" << std::endl;
    }
}

std::wstring SteamGameRegistryFinder::FindGamePath(const std::wstring& wstrName)
{
    std::wstring wstrGamePath;

    std::wifstream vdfFile(m_wstrLibraryFolderVDFPath);
    if (!vdfFile.is_open())
    {
        std::wcerr << L"Could not open the vdf file: " << m_wstrLibraryFolderVDFPath << std::endl;
        return L"";
    }

    std::wstring wstrLine;
    while (std::getline(vdfFile, wstrLine))
    {
        std::wstring wstrItemName = LR"("path")";
        if (const auto pos = wstrLine.find(wstrItemName); pos != std::wstring::npos)
        {
            const auto startPos = pos + wstrItemName.length() + 3; // skip "{wstrItemName}\t\t\""

            auto wstrLibraryPath = wstrLine.substr(startPos, wstrLine.length() - startPos - 1);
            replace_all(wstrLibraryPath, LR"(\\)", LR"(\)");

            if (auto wstrInstallDir = FindInstallationDir(wstrLibraryPath, wstrName); !wstrInstallDir.empty())
                return wstrLibraryPath + LR"(\steamapps\common\)" + wstrInstallDir;
        }
    }

    return L"";
}

std::wstring SteamGameRegistryFinder::FindInstallationDir(const std::wstring& wstrLibraryPath, const std::wstring& wstrGameName)
{
    const std::wstring wstrSteamAppsPath = wstrLibraryPath + LR"(\steamapps)";
    for (const auto & file : std::filesystem::directory_iterator(wstrSteamAppsPath))
    {
        auto path = file.path().wstring();
        if (path.find(L"appmanifest_") != std::wstring::npos) // Check if it is appmanifest_{appid}.acf
        {
            std::wifstream acfFile(path);
            if (!acfFile.is_open())
            {
                std::wcerr << L"Could not open the acf file: " << path << std::endl;
                return L"";
            }

            std::wstring wstrLine;
            bool bMatched = false;
            while (std::getline(acfFile, wstrLine))
            {
                std::wstring wstrItemName = LR"("installdir")";
                auto pos = wstrLine.find(wstrItemName);
                if (bMatched && pos != std::wstring::npos)
                {
                    const auto startPos = pos + wstrItemName.length() + 3; // skip "{wstrItemName}\t\t\""
                    return wstrLine.substr(startPos, wstrLine.length() - startPos - 1);
                }

                wstrItemName = LR"("name")";
                pos = wstrLine.find(wstrItemName);
                if (pos != std::wstring::npos)
                {
                    const auto startPos = pos + wstrItemName.length() + 3; // skip "{wstrItemName}\t\t\""
                    auto wstrGame = wstrLine.substr(startPos, wstrLine.length() - startPos - 1);
                    if (wstrGame == wstrGameName)
                        bMatched = true;
                    else
                        break;
                }
            }
        }
    }

    return L"";
}
