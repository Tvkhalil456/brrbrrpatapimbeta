
#include <windows.h>
#include <gdiplus.h>
#include <mmsystem.h>
#include <shellapi.h>
#include <comdef.h>
#include <iostream>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "ole32.lib")

using namespace Gdiplus;

bool g_run = true;

LRESULT CALLBACK WndProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    static HGLOBAL hg = NULL;

    switch (m) {
    case WM_CREATE: {
        HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(101), L"IMAGE");
        if (!hRes) return 0;
        HGLOBAL hData = LoadResource(NULL, hRes);
        if (!hData) return 0;
        DWORD size = SizeofResource(NULL, hRes);
        void* pData = LockResource(hData);

        if (!pData) return 0;

        hg = GlobalAlloc(GMEM_MOVEABLE, size);
        if (!hg) return 0;

        void* pBuffer = GlobalLock(hg);
        memcpy(pBuffer, pData, size);
        GlobalUnlock(hg);

        IStream* ps = nullptr;
        if (CreateStreamOnHGlobal(hg, FALSE, &ps) != S_OK) return 0;

        Bitmap* b = Bitmap::FromStream(ps);
        HBITMAP hBmp = NULL;
        if (b && b->GetLastStatus() == Ok)
            b->GetHBITMAP(Color(0, 0, 0, 0), &hBmp);
        delete b;
        ps->Release();
        GlobalFree(hg);

        return (LRESULT)hBmp;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC dc = BeginPaint(h, &ps);
        // Dessin ou traitement ici
        EndPaint(h, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(h, m, w, l);
}

DWORD WINAPI GDIRectFloodThread(LPVOID) {
    while (g_run) {
        // traitement
    }
    return 0;
}

DWORD WINAPI GDILineFloodThread(LPVOID) {
    while (g_run) {
        // traitement
    }
    return 0;
}

// Point d'entrée WinMain (simplifié)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Code d'initialisation de la fenêtre
    return 0;
}
