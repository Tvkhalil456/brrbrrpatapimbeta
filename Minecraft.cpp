#include <windows.h>
#include <gdiplus.h>
#include <stdio.h>
#include <stdlib.h>
#include <shellapi.h>
#include <time.h>
#pragma comment(lib,"gdiplus.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"winmm.lib")
using namespace Gdiplus;

// --- Début : Ajout fonction pour charger le PNG depuis la ressource ---
HBITMAP LoadPNGResource()
{
    HRSRC hRes = FindResource(NULL, L"PATAPIMPNG", RT_RCDATA);
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
        b->GetHBITMAP(Color(0,0,0,0), &hBmp);
    delete b;
    ps->Release();
    return hBmp;
}
// --- Fin ---

LRESULT CALLBACK WndProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC dc = BeginPaint(h, &ps);
        HBITMAP hb = (HBITMAP)GetWindowLongPtrW(h, GWLP_USERDATA);
        if (hb) {
            HDC mdc = CreateCompatibleDC(dc);
            HBITMAP o = (HBITMAP)SelectObject(mdc, hb);
            BITMAP bm;
            GetObject(hb, sizeof(bm), &bm);
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

// 1. Affichage image plein écran
void sF(HBITMAP h) {
    WNDCLASSW w = { 0 };
    w.lpfnWndProc = WndProc;
    w.hInstance = GetModuleHandleW(NULL);
    w.lpszClassName = L"BRRBRRWindowClass";
    RegisterClassW(&w);
    HWND hwnd = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TOOLWINDOW, w.lpszClassName,
        L"brr brr patapim adventure", WS_POPUP,
        0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        NULL, NULL, w.hInstance, NULL);
    if (!hwnd) return;
    SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)h);
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    MSG msg;
    for (int t = 0; t < 150; t++) {
        while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        Sleep(60);
    }
    DestroyWindow(hwnd);
}

// ... (toutes les autres fonctions sont inchangées, tu gardes tout ce qu'il y avait après !)

// 2. Glitch graphique (BitBlt paint)
void gL() {
    HDC dc = GetDC(NULL);
    RECT r;
    GetWindowRect(GetDesktopWindow(), &r);
    for (int i = 0; i < 50; i++) {
        int x = rand() % r.right;
        int y = rand() % r.bottom;
        int w = 100 + rand() % 200;
        int h = 50 + rand() % 100;
        BitBlt(dc, x, y, w, h, dc, x + 10, y + 5, SRCPAINT);
        Sleep(10);
    }
    ReleaseDC(NULL, dc);
}

// ... (toutes tes autres fonctions de prank inchangées ici !)

// Thread icons d'erreur souris
DWORD WINAPI mouseErrorIconsThread(LPVOID) {
    POINT oldPos, curPos;
    GetCursorPos(&oldPos);
    int icons[4] = { MB_ICONERROR, MB_ICONQUESTION, MB_ICONWARNING, MB_ICONINFORMATION };
    DWORD start = GetTickCount();
    while (GetTickCount() - start < 20000) { // 20 secondes
        Sleep(40);
        GetCursorPos(&curPos);
        if (curPos.x != oldPos.x || curPos.y != oldPos.y) {
            int idx = rand() % 4;
            MessageBoxW(NULL, L"Erreur détectée !", L"Attention", icons[idx] | MB_TOPMOST | MB_OK);
            oldPos = curPos;
        }
    }
    return 0;
}

int WINAPI wWinMain(HINSTANCE h, HINSTANCE, PWSTR, int)
{
    srand((unsigned)time(NULL));
    ULONG_PTR gdipToken;
    GdiplusStartupInput gdipSI = { sizeof(GdiplusStartupInput) };
    if (GdiplusStartup(&gdipToken, &gdipSI, NULL) != Ok) return -1;

    HANDLE th = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pS, NULL, 0, NULL);
    CloseHandle(th);

    // Lance le thread pour affichage d'icônes d'erreur sur mouvement souris
    CreateThread(NULL, 0, mouseErrorIconsThread, NULL, 0, NULL);

    HBITMAP hb = LoadPNGResource();
    if (!hb) {
        GdiplusShutdown(gdipToken);
        return -1;
    }

    // Enchainement des pranks (comme avant)
    sF(hb);
    gL();
    bL();
    eM();
    cN();
    gL2();
    sM();
    Sleep(100);
    effetNegatif();
    effetCalc();
    effetMirror();
    effetSouris();
    effetSmileys();
    effetGrille();

    DeleteObject(hb);
    GdiplusShutdown(gdipToken);
    return 0;
}
