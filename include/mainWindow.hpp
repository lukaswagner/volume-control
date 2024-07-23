#include <combaseapi.h>

// copied from https://stackoverflow.com/a/21767578, names changed

struct HandleData {
    unsigned long process_id;
    HWND window_handle;
};

BOOL IsMainWindow(HWND handle)
{
    return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
}

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
    HandleData& data = *(HandleData*)lParam;
    unsigned long process_id = 0;
    GetWindowThreadProcessId(handle, &process_id);
    if (data.process_id != process_id || !IsMainWindow(handle))
        return TRUE;
    data.window_handle = handle;
    return FALSE;
}

HWND FindMainWindow(unsigned long process_id)
{
    HandleData data;
    data.process_id = process_id;
    data.window_handle = 0;
    EnumWindows(EnumWindowsCallback, (LPARAM)&data);
    return data.window_handle;
}
