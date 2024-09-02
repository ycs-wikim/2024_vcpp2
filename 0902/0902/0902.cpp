// 0902.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0902.h"

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
    LoadStringW(hInstance, IDC_MY0902, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0902));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0902));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0902);
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

/// 이전 마우스 위치의 x, y 좌표
int g_x = 0, g_y = 0;


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    /// 이전 마우스 위치의 x, y 좌표 ==> 처리 완료 후 모두 삭제
    //int px = 100, py = 100;

    switch (message)
    {
    /// 마우스를 누른 시점에 발생하는 윈도우 메시지
    //case WM_LBUTTONDOWN:
    case WM_MOUSEMOVE:
    {
        int x, y;
        RECT re;
        x = LOWORD(lParam);     // 마우스 정보로부터 하위 16bit를 얻어내는 매크로
        y = HIWORD(lParam);     // 마우스 정보로부터 상위 16bit를 얻어내는 매크로
        re.left = x;
        re.right = g_x;
        re.top = y;
        re.bottom = g_y;
        

        /// 그리기를 시작하도록 하지....
        /// GetDC는 WM_PAINT 이외의 영역에서 DC를 획득할 수 있는 API.
        HDC hdc = GetDC(hWnd);
        Rectangle(hdc, re.left, re.top, re.right, re.bottom);
        /// HDC : Handle of Device(화면) Context
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        /// * 선그리기
        /// 1. MoveToEx는 기준점을 잡는 API
        MoveToEx(hdc, g_x, g_y, NULL);
        /// 2. 기준점으로부터 선그리기를 하는 API
        LineTo(hdc, x, y);
        /*
        /// * 사각형 그리기
        Rectangle(hdc, 10, 10, x, y);

        /// * 타원 그리기
        Ellipse(hdc, 10, 10, x, y);
        */
        /// WM_PAINT 이외의 영역에서 사용된 DC를 해제할 때 사용하는 API.
        ReleaseDC(hWnd, hdc);
        g_x = x;     // 이전 x = 현재 x
        g_y = y;     // 이전 y = 현재 y
    }
        break;
        /*
    case WM_LBUTTONUP:
        break;
    case WM_RBUTTONDOWN:
        break;
    case WM_RBUTTONUP:
        break;
        */
    /// 키보드를 누른 시점에 발생하는 윈도우 메시지
    //case WM_KEYDOWN:
    case WM_KEYUP:
        /// 메시지 박스를 출력하는 API
        /// hWnd : 메시지 박스의 부모 객체
        /// LPCWSTR, LPCWSTR : 메시지 박스의 내용
        /// UINT : 메시지 박스의 버튼 종류
        //MessageBox(hWnd, L"메시지가 도착하였습니다.", L"경고!", MB_OK);
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
            /// 그리기를 시작하도록 하지....
            /// BeginPaint, EndPaint API는 반드시 WM_PAINT에서만 사용해야 한다.
            HDC hdc = BeginPaint(hWnd, &ps);
            /// HDC : Handle of Device(화면) Context
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            /// * 선그리기
            /// 1. MoveToEx는 기준점을 잡는 API
            MoveToEx(hdc, 0, 0, NULL);
            /// 2. 기준점으로부터 선그리기를 하는 API
            LineTo(hdc, 100, 100);

            /// * 사각형 그리기
            Rectangle(hdc, 10, 10, 100, 100);

            /// * 타원 그리기
            Ellipse(hdc, 10, 10, 100, 100);

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
