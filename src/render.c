#include "include/render.h"
#include "include/app.h"
#include "config.h"

void PaintResults(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    //pointer indicator
    int top = AppTopIndex();
    int sel = AppSelectedIndex();
    int count = AppsFilteredCount();

    int rows = count - top;
    if (rows > MAX_RESULTS) rows = MAX_RESULTS;

    HBRUSH sel_brush = CreateSolidBrush(SEL_BG);
    SetBkMode(hdc, TRANSPARENT);

    for (int i = 0; i < rows; i++) {
        const AppEntry *app = AppsFilteredAt(top + i);

        RECT row_rect = {
            10, RESULTS_START_Y + (i * ROW_HEIGHT),
            WINDOW_WIDTH - 10,
            RESULTS_START_Y + ((i + 1) * ROW_HEIGHT)
        };
        
        if (top + i == sel) {
            FillRect(hdc, &row_rect, sel_brush);
            SetTextColor(hdc, SEL_FG);
        } else {
            SetTextColor(hdc, TEXT_FG);
        }

        DeleteObject(sel_brush);
        DrawText(hdc, app -> name, -1, &row_rect , DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }
    EndPaint(hwnd, &ps);
}