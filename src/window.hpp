#pragma once

#include <windows.h>

namespace window {
    struct WindowInfo {
        HWND hwnd = nullptr;
        DWORD process_id = 0;
        long long area = 0;
    };

    inline BOOL CALLBACK enum_windows_proc(HWND hwnd, LPARAM parameter) {
        auto* info = reinterpret_cast<WindowInfo*>(parameter);

        if (!IsWindowVisible(hwnd) || GetWindow(hwnd, GW_OWNER))
            return TRUE;

        DWORD process_id = 0;
        GetWindowThreadProcessId(hwnd, &process_id);

        if (process_id != info->process_id)
            return TRUE;

        RECT rect{};
        if (!GetWindowRect(hwnd, &rect))
            return TRUE;

        long long width = rect.right - rect.left;
        long long height = rect.bottom - rect.top;
        long long area = width * height;

        if (width < 320 || height < 240 || area <= info->area)
            return TRUE;

        info->hwnd = hwnd;
        info->area = area;

        return TRUE;
    }

    inline HWND find_game_window() {
        WindowInfo info{};
        info.process_id = GetCurrentProcessId();

        EnumWindows(enum_windows_proc, reinterpret_cast<LPARAM>(&info));

        return info.hwnd;
    }
}