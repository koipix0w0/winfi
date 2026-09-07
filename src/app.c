#include "include/app.h"
#include <wctype.h>
#include <string.h>

static const wchar_t MAIN_DIR[] = L"Microsoft\\Windows\\Start Menu\\Programs"; 

static AppEntry g_apps[MAX_ITEMS];
static int g_app_count;
static int g_filtered[MAX_ITEMS];
static int g_filtered_count;
static int g_selected;
static int g_top;

static int ContainsNoCase(const wchar_t *haystack, const wchar_t *needle);
static int HasLnkExt(const wchar_t *name);
static void ScanDir(const wchar_t *dir);
static void ScanStartMenu(const wchar_t *env_var);
static int JoinPath(wchar_t *dst, size_t cap, const wchar_t *a, const wchar_t *b);

void AppsInit(void) {
    g_app_count = 0;
    ScanStartMenu(L"ProgramData");
    ScanStartMenu(L"APPDATA");
}

int AppSelectedIndex(void) {
    return g_selected;
}

int AppTopIndex(void) {
    return g_top;
}

const AppEntry *AppSelectedEntry(void) {
    return AppsFilteredAt(g_selected);
}

void AppsFilter(const wchar_t *query) {
    g_filtered_count = 0;
    for (int i = 0; i < g_app_count; i++) {
        if (query == NULL || ContainsNoCase(g_apps[i].name, query))
            g_filtered[g_filtered_count++] = i;
    }

    //resets pointer 
    g_selected = 0;
    g_top = 0;
}

int AppsFilteredCount(void) { return g_filtered_count; }

const AppEntry *AppsFilteredAt(int i) {
    if (i < 0 || i >= g_filtered_count) return NULL;
    return &g_apps[g_filtered[i]];
}

static void ScanStartMenu(const wchar_t *env_var) {
    wchar_t root[MAX_PATH], dir[MAX_PATH];

    DWORD n = GetEnvironmentVariable(env_var, root, MAX_PATH);

    if (n == 0 || n >= MAX_PATH) return;
    if (!JoinPath(dir, MAX_PATH, root, MAIN_DIR)) return;

    ScanDir(dir);
}

static int JoinPath(wchar_t *dst, size_t cap, const wchar_t *a, const wchar_t *b) {
    size_t la = wcslen(a), lb = wcslen(b);
    if (la + 1 + lb + 1 > cap) return 0;

    memcpy(dst, a, la * sizeof(wchar_t));
    dst[la] = L'\\';
    memcpy(dst + la + 1, b, lb * sizeof(wchar_t));
    dst[la + 1 + lb] = L'\0';

    return 1;
}

static int HasLnkExt(const wchar_t *name) {
    size_t n = wcslen(name);
    if (n < 5) return 0;

    const wchar_t *e = name + n - 4;
    return e[0] == L'.'
        && towlower(e[1]) == L'l'
        && towlower(e[2]) == L'n'
        && towlower(e[3]) == L'k';
    
}

static void ScanDir(const wchar_t *dir) {
    wchar_t pattern[MAX_PATH];
    if(!JoinPath(pattern, MAX_PATH, dir, L"*")) return;

    WIN32_FIND_DATA fd;
    HANDLE h = FindFirstFile(pattern, &fd);
    if (h == INVALID_HANDLE_VALUE) return;

    do {
        if (wcscmp(fd.cFileName, L".") == 0 || wcscmp(fd.cFileName, L"..") == 0)
            continue;
        
        if (g_app_count >= MAX_ITEMS) break;

        wchar_t child[MAX_PATH];
        if (!JoinPath(child, MAX_PATH, dir, fd.cFileName)) continue;

        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            ScanDir(child);
            continue;
        }

        if (!HasLnkExt(fd.cFileName)) continue;

        AppEntry *e = &g_apps[g_app_count];

        size_t stem = wcslen(fd.cFileName) - 4;
        if (stem > MAX_ITEMS_LENGTH - 1) stem = MAX_ITEMS_LENGTH - 1;
        memcpy(e -> name, fd.cFileName, stem * sizeof(wchar_t));
        e -> name[stem] = L'\0';

        memcpy(e -> path, child, (wcslen(child) + 1) * sizeof(wchar_t));
        g_app_count++;
    } while (FindNextFile(h, &fd));
    FindClose(h);
}

static int ContainsNoCase(const wchar_t *haystack, const wchar_t *needle) {
    if (!*needle) return 1;

    for (const wchar_t *h = haystack; *h; h++) {
        const wchar_t *a = h, *b = needle;

        while (*b && towlower(*a) == towlower(*b)) { a++; b++; }
        if (!*b) return 1;
    }
    return 0;
}

void AppSelectMove(int delta) {
    if (g_filtered_count = 0) {
        g_selected = 0; g_top = 0;
        return;
    }

    //clamp
    g_selected += delta;
    if (g_selected < 0) g_selected = 0;
    if (g_selected >= g_filtered_count) g_selected = g_filtered_count - 1;

    if (g_selected < g_top) g_top = g_selected;
    if (g_selected >= g_top + MAX_RESULTS) g_top = g_selected - MAX_RESULTS + 1;
}