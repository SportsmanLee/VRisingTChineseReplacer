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
    std::wcout << L"遊戲所在路徑: " << wstrGamePath << std::endl;

    const auto dstFilePath = std::filesystem::path(wstrGamePath + LR"(\VRising_Data\StreamingAssets\Localization\繁體中文.json)");
    const auto srcFilePath = std::filesystem::path(L"TChinese.json");
    if (std::filesystem::exists(srcFilePath))
    {
        std::filesystem::copy_file(srcFilePath, dstFilePath, std::filesystem::copy_options::overwrite_existing);
        std::wcout << L"繁中化檔案已複製至: " << dstFilePath.wstring() << std::endl;
    }
    else
    {
        std::wcerr << L"找不到繁中化檔案: " << srcFilePath.wstring() << std::endl
            << LR"(請將 TChinese.json 置於此程式所在資料夾。)" << std::endl;
    }

    std::wcout << std::endl << L"輸入任何按鍵以關閉視窗 . . .";
    std::getchar();
}
