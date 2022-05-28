// VRisingTChineseReplacer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include "SteamGameRegistryFinder.h"

int main()
{
    std::locale::global(std::locale(""));

    SteamGameRegistryFinder gameFinder;
    const auto wstrGamePath = gameFinder.FindGamePath(L"V Rising");
    std::wcout << L"�C���Ҧb��|: " << wstrGamePath << std::endl;

    const auto dstFilePath = std::filesystem::path(wstrGamePath + LR"(\VRising_Data\StreamingAssets\Localization\TChinese.json)");
    const auto srcFilePath = std::filesystem::path(L"TChinese.json");
    if (std::filesystem::exists(srcFilePath))
    {
        std::filesystem::copy_file(srcFilePath, dstFilePath, std::filesystem::copy_options::overwrite_existing);
        std::wcout << L"�c�����ɮפw�ƻs��: " << dstFilePath.wstring() << std::endl;
    }
    else
    {
        std::wcerr << L"�䤣���c�����ɮ�: " << srcFilePath.wstring() << std::endl
            << LR"(�бN TChinese.json �m�󦹵{���Ҧb��Ƨ��C)" << std::endl;
    }

    std::wcout << std::endl << L"��J������H�������� . . .";
    std::getchar();
}
