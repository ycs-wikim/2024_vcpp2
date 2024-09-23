// 0923.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0923.h"
#include <stdlib.h>
#include <time.h>

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
    LoadStringW(hInstance, IDC_MY0923, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0923));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0923));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0923);
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
//

/// 전역 변수로 선언된 변수는 모두 0으로 초기화
RECT g_me, g_you, g_item;       // 나의 객체, 상대의 객체, 아이템 객체
RECT g_bar;
/// 상대의 속도 제어를 위한 전역 변수
int g_timer = 1000;
/// 아이템이 보여지는 여부
int g_view = 1;
/// 게임 시간
int g_gametime = 10;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT a = { 0, 0, 0, 0 };    /// 겹침이 발생한 좌표값 획득.
    switch (message)
    {
    /// OS가 주기적으로 호출해주는 알람이 도착했다!
    case WM_TIMER:
    {
        /// wParam은 SetTimer에서 지정한 ID 값을 가지고 호출된다.
        if (1 == wParam)
        {
            /// 상대 객체가 이동하도록 구성
            /// X 좌표를 보고 추적하기
            if (g_me.left > g_you.left)
            {
                g_you.left += 10;
                g_you.right += 10;
            }
            else
            {
                g_you.left -= 10;
                g_you.right -= 10;
            }

            /// Y 좌표를 보고 추적하기
            if (g_me.top > g_you.top)
            {
                g_you.top += 10;
                g_you.bottom += 10;
            }
            else
            {
                g_you.top -= 10;
                g_you.bottom -= 10;
            }
            /// 상대가 나를 잡았는지 확인하는 단계
            /// 좌표 겹침이 발생했는가를 확인!
            if (IntersectRect(&a, &g_me, &g_you))
            {
                /// 타이머를 멈추는 API
                KillTimer(hWnd, 1);
                KillTimer(hWnd, 2);
                MessageBox(hWnd, L"잡았다", L"야호", MB_OK);
            }

            /// 상대의 좌표 값을 조정한 상태 ==> 화면 무효화를 요청!
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (2 == wParam)
        {
            /// 상대의 속도를 조절!
            g_timer -= 100;
            if (g_timer <= 100)
            {
//                KillTimer(hWnd, 2);
                g_timer = 100;
            }
            KillTimer(hWnd, 1);
            SetTimer(hWnd, 1, g_timer, NULL);
        }
        else if (3 == wParam)
        {
            g_gametime -= 1;
            if (0 == g_gametime)
            {
                KillTimer(hWnd, 1);
                KillTimer(hWnd, 2);
                KillTimer(hWnd, 3);
            }
            g_bar.right -= 50;
        }
    }
        break;
    /// 생성자와 동일한 역할 : 프로그램 실행 후 OS가 한번만! 호출해준다.
    case WM_CREATE:
        /// 랜덤 시드 결정. 프로그램 실행 시 단 한번만! 수행하면 됨.
        srand(time(NULL));
        /// 상대 객체의 이동을 위한 타이머의 설치
        /// OS가 지정한 시간이 되면, WM_TIMER를 호출해준다!
        SetTimer(hWnd, 1, g_timer, NULL);
        /// 상대가 1초마다 속도를 변경하기 위해 필요한 타이머
        SetTimer(hWnd, 2, 1000, NULL);
        /// 게임 시간 제어를 위한 타이머
        SetTimer(hWnd, 3, 1000, NULL);

        /// 내 객체의 초기 정보
        g_me.left = 10;
        g_me.top = 10;
        g_me.right = 100;
        g_me.bottom = 100;

        /// 상대 객체의 초기 정보
        g_you.left = 300;
        g_you.top = 300;
        g_you.right = 390;
        g_you.bottom = 390;

        /// 아이템 객체의 초기 정보
        g_item.left = rand() % 500;
        g_item.top = rand() % 500;
        g_item.right = g_item.left + 50;
        g_item.bottom = g_item.top + 50;

        /// 시간 바의 초기 값
        g_bar.left = 100;
        g_bar.top = 100;
        g_bar.right = 600;
        g_bar.bottom = 200;
        break;

    /// 나의 객체 이동을 위한 키보드 처리
    /// ==> 결론적으로 내 객체의 좌표값을 수정한다!
    case WM_KEYDOWN:
        switch (wParam)     /// 키보드 부가 정보는 WPARAM
        {
        case VK_LEFT:       /// 왼쪽 방향키를 눌렀을 때.
            g_me.left -= 10;    /// RECT 자료형의 x 좌표 이동은 동시
            g_me.right -= 10;
            break;
        case VK_RIGHT:      /// 오른족 방향키를 눌렀을 때.
            g_me.left += 10;
            g_me.right += 10;
            break;
        case VK_UP:         /// 위쪽 방향키를 눌렸을 때.
            g_me.top -= 10;     /// RECT 자료형의 y 좌표 이동도 동시
            g_me.bottom -= 10;
            break;
        case VK_DOWN:       /// 아래쪽 방향키를 눌렀을 때.
            g_me.top += 10;
            g_me.bottom += 10;
            break;
        }
        /// 좌표 이동 제한
        if (g_me.left > 500)
        {
            g_me.left = 500;
            g_me.right = 590;
        }

        /// 내가 이동한 좌표에 아이템이 존재하는지 확인
        if (IntersectRect(&a, &g_me, &g_item))
        {
            //g_view = 0;
            g_item.left = rand() % 500;
            g_item.top = rand() % 500;
            g_item.right = g_item.left + 50;
            g_item.bottom = g_item.top + 50;
        }
        /// 좌표 값의 수정 완료 ==> 화면을 다시 그릴 필요가 있다!
        /// 프로그래머가 WM_PAINT 호출을 OS에게 요청!
        InvalidateRect(hWnd, NULL, TRUE);
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            /// 나의 객체 그리기
            Rectangle(hdc, g_me.left, g_me.top, g_me.right, g_me.bottom);

            /// 상대의 객체 그리기
            Rectangle(hdc, g_you.left, g_you.top, g_you.right, g_you.bottom);

            /// 아이템 객체 그리기
            if( 1 == g_view )
                Rectangle(hdc, g_item.left, g_item.top, g_item.right, g_item.bottom);

            /// 타이머 바
            Rectangle(hdc, g_bar.left, g_bar.top, g_bar.right, g_bar.bottom);


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
