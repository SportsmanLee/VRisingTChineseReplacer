// VRisingTChineseReplacer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include "SteamGameRegistryFinder.h"

int main()
{
    SteamGameRegistryFinder gameFinder;
    const auto wstrGamePath = gameFinder.FindGamePath(L"V Rising");
    std::wcout << L"Found game path: " << wstrGamePath << std::endl;

    const auto dstFilePath = std::filesystem::path(wstrGamePath + LR"(\VRising_Data\StreamingAssets\Localization\TChinese.json)");
    const auto srcFilePath = std::filesystem::path(L"TChinese.json");
    if (std::filesystem::exists(srcFilePath))
    {
        std::filesystem::copy_file(srcFilePath, dstFilePath, std::filesystem::copy_options::overwrite_existing);
        std::wcout << L"File copied to path: " << dstFilePath << std::endl;
    }
    else
    {
        std::wcerr << L"Cannot find file: " << srcFilePath << std::endl
        << LR"(Please place "TChinese.json" beside this .exe file.)" << std::endl;
    }

    std::wcout << std::endl << L"Press any key to close this window . . .";
    std::getchar();
}
