#include "pch.h"
#include "FileLoader.h"
#include "Console.h"
#include "SceneManager.h"

wchar_t *FileLoad(const wchar_t *filePath)
{
    FILE *loadFile;
    errno_t err = _wfopen_s(&loadFile, filePath, L"rb");
    if (err != 0 || loadFile == nullptr)
    {
        // error 상황
        return nullptr;
    }

    fseek(loadFile, 0, SEEK_END);
    int fileSize = ftell(loadFile);
    fseek(loadFile, 0, SEEK_SET);

    wchar_t *loadBuffer = (wchar_t *)malloc(sizeof(wchar_t) * fileSize);
    if (loadBuffer == nullptr)
    {
        // error 상황
        return nullptr;
    }
    
    fread_s(loadBuffer, sizeof(wchar_t) * fileSize, sizeof(wchar_t), fileSize, loadFile);
    return loadBuffer;
}

void FileSceneParse(wchar_t *buffer)
{
    const wchar_t *del = L"\r\n";
    wchar_t *nextTok = nullptr;
    wchar_t *token = wcstok_s(buffer, del, &nextTok);

    int colNum = 0;
    while (token != nullptr)
    {
        wmemcpy_s(g_curScene.m_szConsoleBuffer[colNum++], dfSCREEN_WIDTH, token, dfSCREEN_WIDTH);
        token = wcstok_s(nullptr, del, &nextTok);
    }
}
