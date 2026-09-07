#include <windows.h>
#include "include/window.h"
#include "include/app.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    HWND hwnd = CreateMainWindow(hInstance);
 
    if (hwnd == NULL) {
        return 0;
    }
 
    ShowWindow(hwnd, nCmdShow);
 
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {

        if (msg.message == WM_KEYDOWN) {

            //esc - close/hide window
            if (msg.wParam == VK_ESCAPE) {
                DestroyWindow(hwnd);
                continue;
            }

            //move up or down to bottom >:3
            if (msg.wParam == VK_UP || msg.wParam == VK_DOWN) {
                AppSelectMove(msg.wParam == VK_DOWN ? 1 : -1);
                InvalidateRect(hwnd, NULL, TRUE);
                continue;
            }
            
            //runnya
            if (msg.wParam == VK_RETURN) {
                if (AppLaunchSelected()) DestroyWindow(hwnd);
                continue;
            }
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}