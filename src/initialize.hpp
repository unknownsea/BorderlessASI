#pragma once

#include <windows.h>
#include <filesystem>

#include "window.hpp"
#include "borderless.hpp"
#include "config.hpp"

namespace initialize {
    inline DWORD WINAPI run(LPVOID) {
        HWND hwnd = nullptr;

        for (int i = 0; i < 240; ++i) {
            hwnd = window::find_game_window();

            if (hwnd)
                break;

            Sleep(500);
        }

        if (!hwnd)
            return 0;

        HMODULE module = nullptr;

        if (!GetModuleHandleExW(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
            GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            reinterpret_cast<LPCWSTR>(&run),
            &module
        ))
            return 0;

        wchar_t dll_path[MAX_PATH]{};

        if (!GetModuleFileNameW(module, dll_path, MAX_PATH))
            return 0;

        std::filesystem::path config_path =
            std::filesystem::path(dll_path).parent_path() /
            L"BorderlessASI.ini";

        Options options{};
        LoadConfig(config_path.c_str(), options);

        borderless::apply(hwnd, options);

        return 0;
    }
}