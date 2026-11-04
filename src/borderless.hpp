#pragma once

#include <windows.h>

#include "config.hpp"

namespace borderless {
    inline bool apply(HWND hwnd, const Options& options) {
        if (!IsWindow(hwnd))
            return false;

        RECT window_rect{};

        if (!GetWindowRect(hwnd, &window_rect))
            return false;

        HMONITOR monitor = MonitorFromWindow(
            hwnd,
            MONITOR_DEFAULTTONEAREST
        );

        MONITORINFO monitor_info{};
        monitor_info.cbSize = sizeof(monitor_info);

        if (!GetMonitorInfoW(monitor, &monitor_info))
            return false;

        const RECT& monitor_rect = monitor_info.rcMonitor;

        int monitor_width = monitor_rect.right - monitor_rect.left;
        int monitor_height = monitor_rect.bottom - monitor_rect.top;

        int width = window_rect.right - window_rect.left;
        int height = window_rect.bottom - window_rect.top;

        if (width <= 0 || height <= 0)
            return false;

        int target_width = monitor_width;
        int target_height = monitor_height;

        if (options.scale_height)
            target_width = MulDiv(width, monitor_height, height);

        int x = monitor_rect.left + (monitor_width - target_width) / 2;
        int y = monitor_rect.top;

        LONG_PTR style = GetWindowLongPtrW(hwnd, GWL_STYLE);
        LONG_PTR exstyle = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);

        style &= ~WS_OVERLAPPEDWINDOW;
        style |= WS_POPUP;

        exstyle &= ~(
            WS_EX_DLGMODALFRAME |
            WS_EX_WINDOWEDGE |
            WS_EX_CLIENTEDGE |
            WS_EX_STATICEDGE
        );

        SetLastError(0);
        LONG_PTR previous = SetWindowLongPtrW(hwnd, GWL_STYLE, style);

        if (previous == 0 && GetLastError() != 0)
            return false;

        SetLastError(0);
        previous = SetWindowLongPtrW(hwnd, GWL_EXSTYLE, exstyle);

        if (previous == 0 && GetLastError() != 0)
            return false;

        return SetWindowPos(
            hwnd,
            HWND_TOP,
            x,
            y,
            target_width,
            target_height,
            SWP_FRAMECHANGED |
            SWP_SHOWWINDOW |
            SWP_NOOWNERZORDER
        );
    }
}