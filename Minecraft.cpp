#define NOMINMAX
#define _WIN32_WINNT 0x0601

#include <windows.h>
#include <gdiplus.h>
#include <mmsystem.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <shellapi.h>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "ole32.lib")

using namespace Gdiplus;

HBITMAP g_hBmp = nullptr;
bool g_run = true;

// Helper to load bitmap from resource
HBITMAP LoadBitmapFromResource()
{
    HRSRC hRes = FindResourceA(NULL, MAKEINTRESOURCEA(101), "IMAGE");
    if (!hRes) return NULL;

    HGLOBAL hData = LoadResource(NULL, hRes);
    if (!hData) return NULL;

    DWORD size = SizeofResource(NULL, hRes);
    void* pData = LockResource(hData);
    if (!pData) return NULL;

    HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, size);
    if (!hBuffer) return NULL;

    void* pBuffer = GlobalLock(hBuffer);
    CopyMemory(pBuffer, pData, size);
    GlobalUnlock(hBuffer);

    IStream* pStream = NULL;
    if (CreateStreamOnHGlobal(hBuffer, TRUE, &pStream) != S_OK) {
        GlobalFree(hBuffer);
        return NULL;
    }

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    Bitmap* bmp = Bitmap::FromStream(pStream);
    HBITMAP hBitmap;
    bmp->GetHBITMAP(0, &hBitmap);

    delete bmp;
    pStream->Release();
    GdiplusShutdown(gdiplusToken);

    return hBitmap;
}

// Thread for GDI animation
DWORD WINAPI GDIRectFloodThread(LPVOID)
{
    HDC hdc = GetDC(NULL);
    srand((unsigned)time(NULL));
    while (g_run) {
        int x = rand() % GetSystemMetrics(SM_CXSCREEN);
        int y = rand() % GetSystemMetrics(SM_CYSCREEN);
        int w = rand() % 400 + 20;
        int h = rand() % 400 + 20;
        COLORREF color = RGB(rand() % 256, rand() % 256, rand() % 256);
        HBRUSH hBrush = CreateSolidBrush(color);
        FillRect(hdc, new RECT{ x, y, x + w, y + h }, hBrush);
        DeleteObject(hBrush);
        Sleep(10);
    }
    ReleaseDC(NULL, hdc);
    return 0;
}

// Window procedure
LRESULT CALLBACK WndProc(HWND h, UINT m, WPARAM w, LPARAM l)
{
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
    }

    default:
        return DefWindowProc(h, m, w, l);
    }
}

// Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MinecraftWindowClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    HWND hwnd = CreateWindow("MinecraftWindowClass", "BRR BRR PATAPIM",
                             WS_POPUP | WS_VISIBLE,
                             0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
                             NULL, NULL, hInstance, NULL);

    g_hBmp = LoadBitmapFromResource();

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    CreateThread(NULL, 0, GDIRectFloodThread, NULL, 0, NULL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    g_run = false;
    DeleteObject(g_hBmp);

    return 0;
}#define NOMINMAX
#define _WIN32_WINNT 0x0601

#include <windows.h>
#include <gdiplus.h>
#include <mmsystem.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <shellapi.h>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "ole32.lib")

using namespace Gdiplus;

HBITMAP g_hBmp = nullptr;
bool g_run = true;

// Helper to load bitmap from resource
HBITMAP LoadBitmapFromResource()
{
    HRSRC hRes = FindResourceA(NULL, MAKEINTRESOURCEA(101), "IMAGE");
    if (!hRes) return NULL;

    HGLOBAL hData = LoadResource(NULL, hRes);
    if (!hData) return NULL;

    DWORD size = SizeofResource(NULL, hRes);
    void* pData = LockResource(hData);
    if (!pData) return NULL;

    HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, size);
    if (!hBuffer) return NULL;

    void* pBuffer = GlobalLock(hBuffer);
    CopyMemory(pBuffer, pData, size);
    GlobalUnlock(hBuffer);

    IStream* pStream = NULL;
    if (CreateStreamOnHGlobal(hBuffer, TRUE, &pStream) != S_OK) {
        GlobalFree(hBuffer);
        return NULL;
    }

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    Bitmap* bmp = Bitmap::FromStream(pStream);
    HBITMAP hBitmap;
    bmp->GetHBITMAP(0, &hBitmap);

    delete bmp;
    pStream->Release();
    GdiplusShutdown(gdiplusToken);

    return hBitmap;
}

// Thread for GDI animation
DWORD WINAPI GDIRectFloodThread(LPVOID)
{
    HDC hdc = GetDC(NULL);
    srand((unsigned)time(NULL));
    while (g_run) {
        int x = rand() % GetSystemMetrics(SM_CXSCREEN);
        int y = rand() % GetSystemMetrics(SM_CYSCREEN);
        int w = rand() % 400 + 20;
        int h = rand() % 400 + 20;
        COLORREF color = RGB(rand() % 256, rand() % 256, rand() % 256);
        HBRUSH hBrush = CreateSolidBrush(color);
        FillRect(hdc, new RECT{ x, y, x + w, y + h }, hBrush);
        DeleteObject(hBrush);
        Sleep(10);
    }
    ReleaseDC(NULL, hdc);
    return 0;
}

// Window procedure
LRESULT CALLBACK WndProc(HWND h, UINT m, WPARAM w, LPARAM l)
{
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
    }

    default:
        return DefWindowProc(h, m, w, l);
    }
}

// Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "PatapimWindowClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    HWND hwnd = CreateWindow("PatapimWindowClass", "brr brr patapim",
                             WS_POPUP | WS_VISIBLE,
                             0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
                             NULL, NULL, hInstance, NULL);

    g_hBmp = LoadBitmapFromResource();

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    CreateThread(NULL, 0, GDIRectFloodThread, NULL, 0, NULL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    g_run = false;
    DeleteObject(g_hBmp);

    return 0;
}
