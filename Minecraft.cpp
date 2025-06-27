#include <windows.h>
#include <gdiplus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <shellapi.h>
#include <wingdi.h>
#include <time.h>
#pragma comment(lib,"gdiplus.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"winmm.lib")
using namespace Gdiplus;

const char* b64 = "iVBORw0KGgoAAAANSUhEUgAAAqQAAAK9CAIAAAB9/emtAAAAAXNSR0IArs4c6QAAIABJREFUeJyEveuS3EiSLua3iACQWUWyL+zemdEeyfReeii9gN5IJjMdk5l0tHN2Z6anL2ySdclMABF+0Q9cMqvIXqU1q4uZQGQg4PDP/fPPg/i//m//[...]";

int v(char c) {
    return (c >= 'A' && c <= 'Z') ? c - 'A' :
           (c >= 'a' && c <= 'z') ? c - 'a' + 26 :
           (c >= '0' && c <= '9') ? c - '0' + 52 :
           c == '+' ? 62 :
           c == '/' ? 63 : -1;
}

unsigned char* dec(const char* i, int* o) {
    if (!i) return NULL;
    int l = (int)strlen(i), j = 0, b = 0, bc = 0;
    unsigned char* oP = (unsigned char*)malloc(l * 3 / 4 + 1);
    if (!oP) return NULL;
    for (int k = 0; k < l; k++) {
        int val = v(i[k]);
        if (val >= 0) {
            b = (b << 6) | val;
            bc += 6;
            if (bc >= 8) {
                bc -= 8;
                oP[j++] = (b >> bc) & 0xFF;
            }
        }
    }
    *o = j;
    return oP;
}

HBITMAP bS(IStream* p) {
    Bitmap* b = Bitmap::FromStream(p);
    if (!b || b->GetLastStatus() != Ok) {
        delete b;
        return NULL;
    }
    HBITMAP h = NULL;
    if (b->GetHBITMAP(Color(0, 0, 0, 0), &h) != Ok) {
        delete b;
        return NULL;
    }
    delete b;
    return h;
}

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

// 3. Overlay gris semi-transparent
void bL() {
    HDC dc = GetDC(NULL);
    RECT r;
    GetWindowRect(GetDesktopWindow(), &r);
    HBRUSH br = CreateSolidBrush(RGB(128, 128, 128));
    BLENDFUNCTION bl = { AC_SRC_OVER, 0, 100, 0 };
    HDC mdc = CreateCompatibleDC(dc);
    HBITMAP bm = CreateCompatibleBitmap(dc, r.right, r.bottom);
    HBITMAP old = (HBITMAP)SelectObject(mdc, bm);
    FillRect(mdc, &r, br);
    AlphaBlend(dc, 0, 0, r.right, r.bottom, mdc, 0, 0, r.right, r.bottom, bl);
    SelectObject(mdc, old);
    DeleteObject(bm);
    DeleteDC(mdc);
    DeleteObject(br);
    ReleaseDC(NULL, dc);
}

// 4. Spam MessageBox "BRR BRR PATAPIM"
void eM() {
    for (int i = 0; i < 35; i++) {
        MessageBoxW(NULL, L"BRR BRR PATAPIM", L"BRR BRR patapim", MB_ICONERROR | MB_OK | MB_TOPMOST);
        Sleep(20);
    }
}

// 5. Ouverture de notepad en boucle
void cN() {
    for (int i = 0; i < 7; i++) {
        STARTUPINFO si = { 0 };
        si.cb = sizeof(si);
        PROCESS_INFORMATION pi;
        if (CreateProcessW(L"C:\\Windows\\System32\\notepad.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        Sleep(60);
    }
}

// 6. Glitch graphique (BitBlt paint 2)
void gL2() {
    HDC dc = GetDC(NULL);
    RECT r;
    GetWindowRect(GetDesktopWindow(), &r);
    for (int i = 0; i < 35; i++) {
        int x = rand() % r.right;
        int y = rand() % r.bottom;
        int w = 60 + rand() % 120;
        int h = 32 + rand() % 80;
        BitBlt(dc, x, y, w, h, dc, x + 5, y + 3, SRCPAINT);
        Sleep(13);
    }
    ReleaseDC(NULL, dc);
}

// 7. Spam MessageBox "BRR BRR TRALALA ?"
void sM() {
    for (int i = 0; i < 35; i++) {
        UINT t = (i % 2 == 0) ? MB_ICONERROR | MB_OK : MB_ICONQUESTION | MB_YESNO | MB_TOPMOST;
        int r = MessageBoxW(NULL, L"BRR BRR TRALALA ?", L"Attention", t);
        Sleep(10);
    }
}

// 8. Beep sons
void pS() {
    for (int i = 0; i < 10; i++) {
        Beep(1000 + (i * 200), 200);
        Sleep(100);
    }
}

// 9. Mode négatif clignotant
void effetNegatif() {
    HDC hdc = GetDC(NULL);
    int sx = GetSystemMetrics(SM_CXSCREEN);
    int sy = GetSystemMetrics(SM_CYSCREEN);
    for (int k = 0; k < 7; ++k) {
        BitBlt(hdc, 0, 0, sx, sy, hdc, 0, 0, NOTSRCCOPY);
        Sleep(800);
        BitBlt(hdc, 0, 0, sx, sy, hdc, 0, 0, NOTSRCCOPY);
        Sleep(800);
    }
    ReleaseDC(NULL, hdc);
}

// 10. Ouvre la calculatrice
void effetCalc() {
    for (int i = 0; i < 3; i++) {
        ShellExecuteW(0, 0, L"calc.exe", 0, 0, SW_SHOW);
        Sleep(300);
    }
}

// 11. Inversion horizontale écran
void effetMirror() {
    HDC hdc = GetDC(NULL);
    int sx = GetSystemMetrics(SM_CXSCREEN), sy = GetSystemMetrics(SM_CYSCREEN);
    StretchBlt(hdc, sx, 0, -sx, sy, hdc, 0, 0, sx, sy, SRCCOPY);
    ReleaseDC(NULL, hdc);
}

// 12. Déplacement rapide souris
void effetSouris() {
    POINT pt;
    for (int i = 0; i < 20; i++) {
        pt.x = rand() % GetSystemMetrics(SM_CXSCREEN);
        pt.y = rand() % GetSystemMetrics(SM_CYSCREEN);
        SetCursorPos(pt.x, pt.y);
        Sleep(70);
    }
}

// 13. Affichage smileys partout
void effetSmileys() {
    HDC dc = GetDC(NULL);
    for (int i = 0; i < 24; i++) {
        TextOutW(dc, rand() % GetSystemMetrics(SM_CXSCREEN), rand() % GetSystemMetrics(SM_CYSCREEN), L":)", 2);
        Sleep(25);
    }
    ReleaseDC(NULL, dc);
}

// 14. Superposition grille rouge
void effetGrille() {
    HDC dc = GetDC(NULL);
    int sx = GetSystemMetrics(SM_CXSCREEN), sy = GetSystemMetrics(SM_CYSCREEN);
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    HGDIOBJ old = SelectObject(dc, pen);
    for (int x = 0; x < sx; x += 70) {
        MoveToEx(dc, x, 0, NULL); LineTo(dc, x, sy);
    }
    for (int y = 0; y < sy; y += 70) {
        MoveToEx(dc, 0, y, NULL); LineTo(dc, sx, y);
    }
    SelectObject(dc, old);
    DeleteObject(pen);
    ReleaseDC(NULL, dc);
}

// 15. Thread : Error icons tant que la souris bouge
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

int WINAPI wWinMain(HINSTANCE h, HINSTANCE p, PWSTR c, int n) {
    srand((unsigned)time(NULL));
    ULONG_PTR t;
    GdiplusStartupInput i = { sizeof(GdiplusStartupInput) };
    if (GdiplusStartup(&t, &i, NULL) != Ok) return -1;
    HANDLE th = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pS, NULL, 0, NULL);
    CloseHandle(th);

    // Lance le thread pour affichage d'icônes d'erreur sur mouvement souris
    CreateThread(NULL, 0, mouseErrorIconsThread, NULL, 0, NULL);

    int sz = 0;
    unsigned char* d = dec(b64, &sz);
    if (!d) {
        GdiplusShutdown(t);
        return -1;
    }
    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, sz);
    if (!hg) {
        free(d);
        GdiplusShutdown(t);
        return -1;
    }
    void* pm = GlobalLock(hg);
    memcpy(pm, d, sz);
    GlobalUnlock(hg);
    IStream* ps = NULL;
    if (CreateStreamOnHGlobal(hg, TRUE, &ps) != S_OK) {
        GlobalFree(hg);
        free(d);
        GdiplusShutdown(t);
        return -1;
    }
    HBITMAP hb = bS(ps);
    ps->Release();
    free(d);
    if (!hb) {
        GdiplusShutdown(t);
        return -1;
    }

    // Enchainement des pranks
    sF(hb);            // 1
    gL();              // 2
    bL();              // 3
    eM();              // 4
    cN();              // 5
    gL2();             // 6
    sM();              // 7
    Sleep(100);
    effetNegatif();    // 9
    effetCalc();       // 10
    effetMirror();     // 11
    effetSouris();     // 12
    effetSmileys();    // 13
    effetGrille();     // 14

    DeleteObject(hb);
    GdiplusShutdown(t);
    return 0;
}
