#include "include/render.h"
#include "include/app.h"
#include "config.h"

void PaintResults(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    int count = AppsFilteredCount();
    int rows = count < MAX_RESULTS ? count : MAX_RESULTS;

    for (int i = 0; i < rows; i++) {
        const AppEntry *app = AppsFilteredAt(i);

        RECT row_rect = {
            10, RESULTS_START_Y + (i * ROW_HEIGHT),
            WINDOW_WIDTH - 10,
            RESULTS_START_Y + ((i + 1) * ROW_HEIGHT)
        };

        DrawText(hdc, app -> name, -1, &row_rect , DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }
    EndPaint(hwnd, &ps);
}