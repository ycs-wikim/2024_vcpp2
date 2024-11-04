#include "new.h"


CRITICAL_SECTION g_cs;

DWORD WINAPI thread_cs(LPVOID param)
{
    LPARAM lParam = (LPARAM)param;
    int x, y, i;
    x = LOWORD(lParam);
    y = HIWORD(lParam);

    HDC hdc = GetDC(g_hWnd);

    for (i = 0; i < y; i++)
    {
        /// 3. 임계영역 진입
        EnterCriticalSection(&g_cs);
        ///////////////////////////// 임계 영역 시작
        /*
        g_x = x;
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);
        */
        /// 4. 임계영역 해제
        LeaveCriticalSection(&g_cs);
        ///////////////////////////// 임계 영역 종료
        Sleep(30);          /// Context Switching
    }

    ReleaseDC(g_hWnd, hdc);
    ExitThread(0);
    return 0;
}