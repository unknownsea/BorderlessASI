#pragma once

#include <windows.h>

struct Options {
    bool scale_height = false;
};

inline bool MakeConfig(const wchar_t* path) {
    constexpr char config[] =
        "[options]\n"
        "\n"
        "; Scales the game window's height to match the monitor while preserving its aspect ratio; if it is not wide enough, your desktop may be visible on the sides.\n"
        ";\n"
        "; 0 = disabled (default)\n"
        "scale_height=0\n";

    HANDLE file = CreateFileW(
        path,
        GENERIC_WRITE,
        0,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (file == INVALID_HANDLE_VALUE)
        return false;

    DWORD written = 0;

    bool success = WriteFile(
        file,
        config,
        sizeof(config) - 1,
        &written,
        nullptr
    );

    CloseHandle(file);

    return success && written == sizeof(config) - 1;
}

inline bool LoadConfig(const wchar_t* path, Options& options) {
    if (GetFileAttributesW(path) == INVALID_FILE_ATTRIBUTES)
        MakeConfig(path);

    options.scale_height = GetPrivateProfileIntW(
        L"options",
        L"scale_height",
        0,
        path
    ) != 0;

    return true;
}