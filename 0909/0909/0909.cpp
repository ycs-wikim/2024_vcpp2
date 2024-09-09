// 0909.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0909.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY0909, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0909));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0909));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0909);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
/// WndProc의 인수 정리
/// 1. hWnd - 메시지를 받는 윈도우의 핸들
/// 2. UINT - 사용자의 입력을 숫자로 변환한 값
/// 3. *Param(lParam, wParam) : 메시지를 처리하기 위한 부가 정보
///     lParam - 마우스 정보
///     wParam - 키보드 정보

// 나와 상대의 변수 및 초기 좌표 설정
RECT g_me = { 10, 10, 100, 100 };
RECT g_you = { 500, 500, 590, 590 };

/*
g_me.left = 10;
g_me.top = 10;
g_me.right = 100;
g_me.bottom = 100;
*/

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    /// 클래스의 생성자와 완전히 동일한 형태로 동작
    /// 프로그램이 실행된 이후에 단 한번만 OS에 의해 호출된다.
    /// 프로그램의 초기값이나 한번만 실행하는 함수들을 작성하면된다.
    case WM_CREATE:
        SetTimer(hWnd, 1, 1000, NULL);
        break;
    case WM_TIMER:
        /// 상대가 나의 좌표를 보고 상대의 X 좌표를 변경하는 코드
        if (g_me.left > g_you.left)     // 나는 상대의 오른쪽에 있다.
        {
            g_you.left += 10;
            g_you.right += 10;
        }
        else     /// 나는 상대의 왼쪽에 있다.
        {
            g_you.left -= 10;
            g_you.right -= 10;
        }

        /// 상대가 나의 좌표를 보고 상대의 Y 좌표를 변경하는 코드
        if (g_me.top > g_you.top)            // 나는 상대의 아래에 있다.
        {
            g_you.top += 10;
            g_you.bottom += 10;
        }
        else    // 나는 상대의 위에 있다.
        {
            g_you.top -= 10;
            g_you.bottom -= 10;
        }

        InvalidateRect(hWnd, NULL, TRUE);
        break;
    /// 키보드 입력 처리
    case WM_KEYDOWN:
    {
        // 나의 키 입력을 통해 나의 좌표 값을 변경하는 코드
        switch (wParam)
        {
        case VK_UP:
            g_me.top -= 10;
            g_me.bottom -= 10;
            break;
        case VK_DOWN:
            g_me.top += 10;
            g_me.bottom += 10;
            break;
        case VK_LEFT:
            g_me.left -= 10;
            g_me.right -= 10;
            break;
        case VK_RIGHT:
            g_me.left += 10;
            g_me.right += 10;
            break;
        }

        InvalidateRect(hWnd, NULL, TRUE);
    }
        break;

    case WM_LBUTTONDOWN:
        /// 화면을 다시 그려라~ OS에게 요청하는 API
        /// 2번째 인수에 NULL을 입력하면 전체 윈도우가 선택됨.
        /// RECT 자료형을 이용하여 좌표를 설정하면 해당 좌표만 선택됨.
        /// 3번째 인수  : TRUE 설정 - 화면을 모두 지우고 WM_PAINT 호출
        ///             : FALSE 설정 - 화면은 내버려두고 WM_PAINT 호출
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_MOUSEMOVE:
    {
        int x, y;
        x = LOWORD(lParam);
        y = HIWORD(lParam);

        HDC hdc = GetDC(hWnd);

        MoveToEx(hdc, 0, 0, NULL);
        LineTo(hdc, x, y);

        ReleaseDC(hWnd, hdc);
    }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    /// 임시 그리기가 아닌 내용은 모두 WM_PAINT 내부에서 그려야 한다.
    /// 임시 그리기가 지워지는 4가지 경우 ==> OS는 WM_PAINT를 호출
    /* 1. 창을 최대화
       2. 창을 원래 크기로 되돌린 경우
       3. 창을 최소화한 경우
       (다른 창이 앞을 가리는 경우 -> 윈도우 10 이후에는 사라짐)
       4. 프로그래머가 요청한 경우
    */
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            Rectangle(hdc, g_me.left, g_me.top, g_me.right, g_me.bottom);
            Rectangle(hdc, g_you.left, g_you.top, g_you.right, g_you.bottom);

            //MoveToEx(hdc, 0, 0, NULL);
            //LineTo(hdc, 500, 500);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
