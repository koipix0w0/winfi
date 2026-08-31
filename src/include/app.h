#ifndef APPS_H
#define APPS_H

#include <windows.h>
#include "config.h"

typedef struct {
    wchar_t name[MAX_ITEMS_LENGTH];
    wchar_t path[MAX_PATH];
} AppEntry;

void AppsInit(void);

void AppsFilter(const wchar_t *query);

int AppsFilteredCount(void);
const AppEntry *AppsFilteredAt(int i);

#endif